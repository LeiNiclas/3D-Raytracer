#include "Camera.h"
#include "Hittable.h"
#include "HittableList.h"
#include "Sphere.h"
#include "Utilities.h"

// After building, execute the output file using
// build\Debug\outDebug.exe | set-content image.ppm -encoding String


int main()
{
    // World properties
    HittableList world;
    world.add(make_shared<Sphere>(Point3(0, 0, -1), 0.5f));
    world.add(make_shared<Sphere>(Point3(0, -100.5f, -1), 100));
    world.add(make_shared<Sphere>(Point3(0, 0.75f, -1), 0.25f));

    Camera cam;

    // Adjust camera settings
    cam.aspectRatio = 16.0f / 9.0f;
    cam.imageWidth = 512;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;

    cam.render(world);
}
