#include "Hittable.h"
#include "HittableList.h"
#include "Interval.h"
#include "Sphere.h"
#include "Utilities.h"

// After building, execute the output file using
// build\Debug\outDebug.exe | set-content image.ppm -encoding String


Color calculateRayColor(const Ray& ray, const Hittable& world)
{
    HitRecord record;

    if (world.hit(ray, Interval(0, infinity), record))
        return 0.5 * (record.normal + Color(1, 1, 1));

    Vector3 unitRay = normalized(ray.direction());

    float a = 0.5f * (unitRay.y() + 1.0f);
    return (1.0f - a) * Color(1.0f, 1.0f, 1.0f) + a * Color(0.5f, 0.7f, 1.0f);
}

int main()
{
    // Image properties
    const float aspectRatio = 16.0f / 9.0f;
    const int imageWidth = 1024;
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
    Point3 centerPixelPosition = viewportUpperLeft + 0.5f * (pixelDeltaX + pixelDeltaY);

    // World properties
    HittableList world;
    world.add(make_shared<Sphere>(Point3(0, 0, -1), 0.5f));
    world.add(make_shared<Sphere>(Point3(0, -100.5f, -1), 100));
    world.add(make_shared<Sphere>(Point3(1, 0.25f, -1), 0.05f));


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

            // Create the ray and calculate the resulting color.
            Ray ray = Ray(cameraCenter, rayDirection);
            Color pixelColor = calculateRayColor(ray, world);

            writeColor(std::cout, pixelColor);
        }
    }

    std::clog << "Done.\n";
}
