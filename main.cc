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
    HittableList world;

    shared_ptr<LambertianMaterial> groundMat = make_shared<LambertianMaterial>(Color(0.4f));
    shared_ptr<MetalMaterial> metalMat = make_shared<MetalMaterial>(Color(0.8f), 0.05f);
    shared_ptr<DielectricMaterial> glassMat = make_shared<DielectricMaterial>(1.5f);
    shared_ptr<DielectricMaterial> glassBubbleMat = make_shared<DielectricMaterial>(1.0f / 1.5f);

    Sphere groundSphere = Sphere(Point3(0, -1000, 0), 1000, groundMat);
    Sphere metalSphere = Sphere(Point3(-3, 1, 0), 1, metalMat);
    Sphere pureGlassSphere = Sphere(Point3(3, 1, 0), 1, glassMat);
    Sphere outerGlassSphere = Sphere(Point3(0, 1, 0), 1, glassMat);
    Sphere innerGlassSphere = Sphere(Point3(0, 1, 0), 0.8, glassBubbleMat);

    world.add(make_shared<Sphere>(groundSphere));
    world.add(make_shared<Sphere>(metalSphere));
    world.add(make_shared<Sphere>(pureGlassSphere));
    world.add(make_shared<Sphere>(outerGlassSphere));
    world.add(make_shared<Sphere>(innerGlassSphere));


    int sphereCount = 100;
    
    for (int x = 0; x < sphereCount / 10; x++)
    {
        for (int z = 0; z < sphereCount / 10; z++)
        {
            float randomMat = randomFloat();
            
            Point3 center = Point3(x - (sphereCount / 10) / 2 + 0.75f * randomFloat(), 0.25f, z - (sphereCount / 10) / 2 + 0.75f * randomFloat());
            center = Point3(center.x() * 1.25f, center.y(), center.z() * 1.25f);

            shared_ptr<Material> sphereMat;

            if (randomMat < 0.5f)
            {
                Color albedo = Color::randomVector() * Color::randomVector();
                sphereMat = make_shared<LambertianMaterial>(albedo);
            }
            else if (randomMat < 0.8)
            {
                Color albedo = Color::randomVector(0.5f, 1.0f);
                float fuzz = randomFloat(0.0f, 0.5f);
                sphereMat = make_shared<MetalMaterial>(albedo, fuzz);
            }
            else
            {
                sphereMat = make_shared<DielectricMaterial>(1.5f);
            }
            
            world.add(make_shared<Sphere>(center, 0.25f, sphereMat));
        }
    }

    Camera cam;

    cam.aspectRatio = 16.0f / 9.0f;
    cam.imageWidth = 1280;
    cam.samplesPerPixel = 300;
    cam.maxDepth = 50;

    cam.verticalFOV = 20.0f;
    cam.lookfrom = Point3(10.0f, 4.0f, 7.0f);
    cam.lookat = Point3(0);
    cam.vup = Vector3(0, 1, 0);

    cam.defocusAngle = 0.6f;
    cam.focusDistance = 15.0f;
    
    cam.render(world);

    //// Materials
    //shared_ptr<LambertianMaterial> groundMat = make_shared<LambertianMaterial>(Color(0.2f, 0.9f, 0.1f));
    //shared_ptr<LambertianMaterial> whiteMat = make_shared<LambertianMaterial>(Color(1));
    //shared_ptr<LambertianMaterial> grayMat = make_shared<LambertianMaterial>(Color(0.75));
    //shared_ptr<MetalMaterial> greyMetalMat = make_shared<MetalMaterial>(Color(0.8f), 0.1f);
    //shared_ptr<MetalMaterial> blueMetalMat = make_shared<MetalMaterial>(Color(0.1f, 0.2f, 0.9f), 0.1f);
    //shared_ptr<MetalMaterial> redMetalMat = make_shared<MetalMaterial>(Color(0.9f, 0.2f, 0.1f), 0.2f);
    //shared_ptr<DielectricMaterial> glassMat = make_shared<DielectricMaterial>(1.5f);
    //shared_ptr<DielectricMaterial> glassBubbleMat = make_shared<DielectricMaterial>(1.0f / 1.5f);

    //// World properties
    //HittableList world;
    //world.add(make_shared<Sphere>(Point3(0, 0, -1.25f), 0.5f, blueMetalMat));
    //world.add(make_shared<Sphere>(Point3(0, -100.5f, -1), 100, grayMat));
    //world.add(make_shared<Sphere>(Point3(0, 0.75f, -1), 0.25f, glassMat));
    //world.add(make_shared<Sphere>(Point3(0, 0.75f, -1), 0.175f, glassBubbleMat));
    //world.add(make_shared<Sphere>(Point3(-1, 0, -1), 0.5f, whiteMat));
    //world.add(make_shared<Sphere>(Point3(1, 0, -1), 0.5f, glassMat));

    //Camera cam;

    //// Adjust camera settings
    //cam.aspectRatio = 16.0f / 9.0f;
    //cam.imageWidth = 400;
    //cam.samplesPerPixel = 100;
    //cam.maxDepth = 50;

    //cam.verticalFOV = 20.0f;
    //cam.lookfrom = Point3(-2, 2, 1);
    //cam.lookat = Point3(0, 0.5f, -1);
    //cam.vup = Vector3(0, 1, 0);

    //cam.defocusAngle = 0.0f;
    //cam.focusDistance = (cam.lookfrom - cam.lookat).magnitude() + 0.25f;

    //cam.render(world);
}
