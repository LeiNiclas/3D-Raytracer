#ifndef CAMERA_H
#define CAMERA_H

#include "Hittable.h"
#include "Material.h"

#include <chrono>
#include <iomanip>


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
        Vector3 defocusDiskHorizontal;
        Vector3 defocusDiskVertical;


        void init()
        {
            imageHeight = int(imageWidth / aspectRatio);
            pixelSampleScale = 1.0f / samplesPerPixel;
            
            center = lookfrom;

            // Viewport properties
            float theta = deg2rad(verticalFOV);
            float h = std::tan(theta / 2.0f);
            float viewportHeight = 2.0f * h * focusDistance;
            float viewportWidth = viewportHeight * (float(imageWidth) / imageHeight);
            
            w = normalized(lookfrom - lookat);
            u = normalized(crossP(vup, w));
            v = crossP(w, u);

            Vector3 viewportWidthVector = viewportWidth * u;
            Vector3 viewportHeightVector = viewportHeight * -v;

            pixelDeltaX = viewportWidthVector / imageWidth;
            pixelDeltaY = viewportHeightVector / imageHeight;
            
            Vector3 viewportUpperLeft = center -
                                        (focusDistance * w) -
                                        viewportWidthVector / 2.0f -
                                        viewportHeightVector / 2.0f;
            
            centerPixelPosition = viewportUpperLeft + 0.5f * (pixelDeltaX + pixelDeltaY);

            float defocusRadius = focusDistance * std::tan(deg2rad(defocusAngle / 2));
            defocusDiskHorizontal = u * defocusRadius;
            defocusDiskVertical = v * defocusRadius;
        }


        Ray getRay(int x, int y) const
        {
            Vector3 offset = sampleSquare();
            Vector3 pixelSample =   centerPixelPosition +
                                    ((x + offset.x()) * pixelDeltaX) +
                                    ((y + offset.y()) * pixelDeltaY);
            
            Point3 rayOrigin = (defocusAngle <= 0) ? center : defocusDiskSample();
            Vector3 rayDirection = pixelSample - rayOrigin;
            float rayTime = randomFloat();

            return Ray(rayOrigin, rayDirection, rayTime);
        }

        Vector3 sampleSquare() const { return Vector3(randomFloat() - 0.5f, randomFloat() - 0.5f, 0); }

        Point3 defocusDiskSample() const
        {
            Vector3 p = randomInUnitDisk();
            return center + (p.x() * defocusDiskHorizontal) + (p.y() * defocusDiskVertical);
        }

        Color rayColor(const Ray& ray, int depth, const Hittable& world) const
        {
            if (depth <= 0)
                return Color(0.0f);

            HitRecord record;

            if (!world.hit(ray, Interval(0.0001f, infinity), record))
                return backgroundColor;

            // Objects of the world
            Ray scattered;
            Color attenuation;
            Color emissionColor = record.mat->emitted(record.u, record.v, record.p);

            if (!record.mat->scatter(ray, record, attenuation, scattered))
                return emissionColor;
            
            Color scatterColor = attenuation * rayColor(scattered, depth - 1, world);

            return emissionColor + scatterColor;
        }
    
    
    public:
        float   aspectRatio     = 1.0f;
        float   verticalFOV     = 90.0f;
        float   defocusAngle    = 0.0f;
        float   focusDistance   = 10.0f;
        int     imageWidth      = 100;
        int     samplesPerPixel = 10;
        int     maxDepth        = 10;
        Point3  lookfrom        = Point3(0);
        Point3  lookat          = Point3(0, 0, -1);
        Vector3 vup             = Vector3(0, 1, 0);
        Color   backgroundColor;

        void render(const Hittable& world)
        {
            init();

            auto renderStartTime = std::chrono::high_resolution_clock::now();

            std::cout << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

    
            for (int y = 0; y < imageHeight; y++)
            {
                auto renderCurrentTime = std::chrono::high_resolution_clock::now();
                std::chrono::duration<float> renderElapsedTime = renderCurrentTime - renderStartTime;
                int renderMinutes = static_cast<int>(renderElapsedTime.count()) / 60;
                int renderSeconds = static_cast<int>(renderElapsedTime.count()) % 60;

                std::clog << "\rProcessing... (row " << y+1 << " of " << imageHeight << ") "
                          << std::setw(2) << std::setfill('0') << renderMinutes << ":"
                          << std::setw(2) << std::setfill('0') << renderSeconds << " elapsed." << std::flush; 
            
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

            auto renderEndTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<float> renderTotalTime = renderEndTime - renderStartTime;
            int renderTotalMinutes = static_cast<int>(renderTotalTime.count()) / 60;
            int renderTotalSeconds = static_cast<int>(renderTotalTime.count()) % 60;

        
            std::clog << "\rDone. Render time: "
            << std::setw(2) << std::setfill('0') << renderTotalMinutes << ":"
            << std::setw(2) << std::setfill('0') << renderTotalSeconds << ".                   ";
        }
};


#endif
