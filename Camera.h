#ifndef CAMERA_H
#define CAMERA_H

#include "Hittable.h"


class Camera
{
    private:
        int     imageHeight;
        Point3  center;
        Point3  centerPixelPosition;
        Vector3 pixelDeltaX;
        Vector3 pixelDeltaY;


        void init()
        {
            imageHeight = int(imageWidth / aspectRatio);
            center = Point3(0, 0, 0);

            // Viewport properties
            float focalLength = 1.0f;
            float viewportHeight = 2.0f;
            float viewportWidth = viewportHeight * (float(imageWidth) / imageHeight);

            Vector3 viewportWidthVector = Vector3(viewportWidth, 0, 0);
            Vector3 viewportHeightVector = Vector3(0, -viewportHeight, 0);

            pixelDeltaX = viewportWidthVector / imageWidth;
            pixelDeltaY = viewportHeightVector / imageHeight;
            
            Vector3 viewportUpperLeft = center -
                                        Vector3(0, 0, focalLength) -
                                        viewportWidthVector / 2.0f -
                                        viewportHeightVector / 2.0f;
            
            centerPixelPosition = viewportUpperLeft + 0.5f * (pixelDeltaX + pixelDeltaY);
        }


        Color rayColor(const Ray& ray, const Hittable& world) const
        {
            HitRecord record;

            // Objects of the world
            if (world.hit(ray, Interval(0, infinity), record))
                return 0.5f * (record.normal + Color(1, 1, 1));
            
            Vector3 unitDirection = normalized(ray.direction());

            // Background / skybox
            float a = 0.5f * (unitDirection.y() + 1.0f);

            return (1.0f - a) * Color(1.0f, 1.0f, 1.0f) + a * Color(0.5f, 0.7f, 1.0f);
        }

    public:
        float   aspectRatio = 1.0f;
        int     imageWidth = 100;

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
                    // Calculate the center position of the current pixel.
                    Point3 pixelCenter = centerPixelPosition + (float(x) * pixelDeltaX) + (float(y) * pixelDeltaY);
                
                    // Calculate the direction of the outgoing ray from the camera.
                    Vector3 rayDirection = pixelCenter - center;
                
                    // Create the ray and calculate the resulting color.
                    Ray ray = Ray(center, rayDirection);
                    Color pixelColor = rayColor(ray, world);
                
                    writeColor(std::cout, pixelColor);
                }
            }
        
            std::clog << "\rDone.                           \n";
        }

};


#endif
