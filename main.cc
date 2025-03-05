#include <iostream>
#include "Color.h"
#include "Ray.h"
#include "Vector3.h"

// After building, execute the output file using
// build\Debug\outDebug.exe | set-content image.ppm -encoding String

Color calculateRayColor(const Ray& ray)
{
    float a = 0.5f * (ray.direction().y() + 1.0f);
    return (1.0f - a) * Color(1.0f, 1.0f, 1.0f) + a * Color(0.5f, 0.7f, 1.0f);
}

int main()
{
    // Image properties
    const float aspectRatio = 16.0f / 9.0f;
    const int imageWidth = 256;
    const int imageHeight = int(imageWidth / aspectRatio);


    // Viewport properties
    float viewportHeight = 2.0f;
    float viewportWidth = viewportHeight * (float(imageWidth) / imageHeight);

    Vector3 viewportWidthVector = Vector3(viewportWidth, 0, 0);
    Vector3 viewportHeightVector = Vector3(0, -viewportHeight, 0);

    // Camera & pixel properties
    float focalLength = 1.0f;
    Point3 cameraCenter = Point3(0,0,0);

    Point3 pixelDeltaX = viewportWidthVector / imageWidth;
    Point3 pixelDeltaY = viewportHeightVector / imageHeight;

    Point3 viewportUpperLeft =  cameraCenter -
                                Vector3(0, 0, focalLength) -
                                viewportHeightVector / 2 -
                                viewportWidthVector / 2;
    Point3 centerPixelPosition = viewportUpperLeft + 0.5 * (pixelDeltaX + pixelDeltaY);




    // Rendering
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
            Vector3 rayDirection = pixelCenter - cameraCenter;
            rayDirection = normalized(rayDirection);

            // Create the ray and calculate the resulting color.
            Ray ray = Ray(cameraCenter, rayDirection);
            Color rayColor = calculateRayColor(ray);
            Color pixelColor = rayColor;

            writeColor(std::cout, pixelColor);
        }
    }

    std::clog << "\rDone.              \n";
}
