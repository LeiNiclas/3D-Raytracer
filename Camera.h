#ifndef CAMERA_H
#define CAMERA_H

#include "Hittable.h"
#include "Material.h"


class Camera
{
    private:
        int     imageHeight;
        float   pixelSampleScale;
        Point3  center;
        Point3  centerPixelPosition;
        Vector3 pixelDeltaX;
        Vector3 pixelDeltaY;
        Vector3 u;
        Vector3 v;
        Vector3 w;


        void init()
        {
            imageHeight = int(imageWidth / aspectRatio);
            pixelSampleScale = 1.0f / samplesPerPixel;
            
            center = lookfrom;

            // Viewport properties
            float focalLength = (lookfrom - lookat).magnitude();
            float theta = deg2rad(verticalFOV);
            float h = std::tan(theta / 2.0f);
            float viewportHeight = 2.0f * h * focalLength;
            float viewportWidth = viewportHeight * (float(imageWidth) / imageHeight);
            
            w = normalized(lookfrom - lookat);
            u = normalized(crossP(vup, w));
            v = crossP(w, u);

            Vector3 viewportWidthVector = viewportWidth * u;
            Vector3 viewportHeightVector = viewportHeight * -v;

            pixelDeltaX = viewportWidthVector / imageWidth;
            pixelDeltaY = viewportHeightVector / imageHeight;
            
            Vector3 viewportUpperLeft = center -
                                        (focalLength * w) -
                                        viewportWidthVector / 2.0f -
                                        viewportHeightVector / 2.0f;
            
            centerPixelPosition = viewportUpperLeft + 0.5f * (pixelDeltaX + pixelDeltaY);
        }


        Ray getRay(int x, int y) const
        {
            Vector3 offset = sampleSquare();
            Vector3 pixelSample =   centerPixelPosition +
                                    ((x + offset.x()) * pixelDeltaX) +
                                    ((y + offset.y()) * pixelDeltaY);
            
            Point3 rayOrigin = center;
            Vector3 rayDirection = pixelSample - rayOrigin;

            return Ray(rayOrigin, rayDirection);
        }

        Vector3 sampleSquare() const { return Vector3(randomFloat() - 0.5f, randomFloat() - 0.5f, 0); }


        Color rayColor(const Ray& ray, int depth, const Hittable& world) const
        {
            if (depth <= 0)
                return Color(0.0f);

            HitRecord record;

            // Objects of the world
            if (world.hit(ray, Interval(0.0001, infinity), record))
            {
                Ray scattered;
                Color attenuation;

                if (record.mat->scatter(ray, record, attenuation, scattered))
                    return attenuation * rayColor(scattered, depth-1, world);
                
                return Color(0.0f);
            }

            Vector3 unitDirection = normalized(ray.direction());

            // Background / skybox
            float a = 0.5f * (unitDirection.y() + 1.0f);

            return (1.0f - a) * Color(1) + a * Color(0.5f, 0.7f, 1.0f);
        }
    
    
    public:
        float   aspectRatio     = 1.0f;
        float   verticalFOV     = 90.0f;
        int     imageWidth      = 100;
        int     samplesPerPixel = 10;
        int     maxDepth        = 10;
        Point3  lookfrom        = Point3(0);
        Point3  lookat          = Point3(0, 0, -1);
        Vector3 vup             = Vector3(0, 1, 0);

        void render(const Hittable& world)
        {
            init();

            std::cout << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

    
            for (int y = 0; y < imageHeight; y++)
            {
                std::clog << "\rProcessing... (row " << y+1 << " of " << imageHeight << ")"
                          << std::flush; 
            
                for (int x = 0; x < imageWidth; x++)
                {
                    Color pixelColor = Color(0.0f);

                    for (int i = 0; i < samplesPerPixel; i++)
                    {
                        Ray ray = getRay(x, y);
                        pixelColor += rayColor(ray, maxDepth, world);
                    }

                    writeColor(std::cout, pixelSampleScale * pixelColor);
                }
            }
        
            std::clog << "\rDone.                           \n";
        }

};


#endif
