#include "Camera.h"
#include "Hittable.h"
#include "HittableList.h"
#include "Material.h"
#include "Sphere.h"
#include "Utilities.h"

// After building, execute the output file using
// build\Debug\outDebug.exe | set-content image.ppm -encoding String


int main()
{
    // Materials
    shared_ptr<LambertianMaterial> groundMat = make_shared<LambertianMaterial>(Color(0.2f, 0.9f, 0.1f));
    shared_ptr<LambertianMaterial> whiteMat = make_shared<LambertianMaterial>(Color(1));

    shared_ptr<MetalMaterial> greyMetalMat = make_shared<MetalMaterial>(Color(0.8f), 0.1f);
    shared_ptr<MetalMaterial> blueMetalMat = make_shared<MetalMaterial>(Color(0.1f, 0.2f, 0.9f), 0.5f);
    shared_ptr<MetalMaterial> redMetalMat = make_shared<MetalMaterial>(Color(0.9f, 0.2f, 0.1f), 0.2f);

    shared_ptr<DielectricMaterial> glassMat = make_shared<DielectricMaterial>(1.5f);
    shared_ptr<DielectricMaterial> glassBubbleMat = make_shared<DielectricMaterial>(1.0f / 1.5f);

    // World properties
    HittableList world;
    world.add(make_shared<Sphere>(Point3(0, 0, -1.25f), 0.5f, whiteMat));
    world.add(make_shared<Sphere>(Point3(0, -100.5f, -1), 100, groundMat));
    world.add(make_shared<Sphere>(Point3(0, 0.75f, -1), 0.25f, glassMat));
    world.add(make_shared<Sphere>(Point3(0, 0.75f, -1), 0.15f, glassBubbleMat));
    world.add(make_shared<Sphere>(Point3(-1, 0, -1), 0.5f, redMetalMat));
    world.add(make_shared<Sphere>(Point3(1, 0, -1), 0.5f, blueMetalMat));


    Camera cam;

    // Adjust camera settings
    cam.aspectRatio = 16.0f / 9.0f;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;

    cam.render(world);
}
