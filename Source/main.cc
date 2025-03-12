#include "Utilities.h"

#include "BVH.h"
#include "Camera.h"
#include "ConstantMedium.h"
#include "Hittable.h"
#include "HittableList.h"
#include "Material.h"
#include "Primitives.h"
#include "Texture.h"


using Scene = HittableList;


void finalRenderBook1()
{
    Scene finalRender;

    shared_ptr<LambertianMaterial> groundMat = make_shared<LambertianMaterial>(Color(0.4f));
    shared_ptr<MetalMaterial> metalMat = make_shared<MetalMaterial>(Color(0.8f), 0.05f);
    shared_ptr<DielectricMaterial> glassMat = make_shared<DielectricMaterial>(1.5f);
    shared_ptr<DielectricMaterial> glassBubbleMat = make_shared<DielectricMaterial>(1.0f / 1.5f);

    Sphere groundSphere = Sphere(Point3(0, -1000, 0), 1000, groundMat);
    Sphere metalSphere = Sphere(Point3(-3, 1, 0), 1, metalMat);
    Sphere pureGlassSphere = Sphere(Point3(3, 1, 0), 1, glassMat);
    Sphere outerGlassSphere = Sphere(Point3(0, 1, 0), 1, glassMat);
    Sphere innerGlassSphere = Sphere(Point3(0, 1, 0), 0.8, glassBubbleMat);

    finalRender.add(make_shared<Sphere>(groundSphere));
    finalRender.add(make_shared<Sphere>(metalSphere));
    finalRender.add(make_shared<Sphere>(pureGlassSphere));
    finalRender.add(make_shared<Sphere>(outerGlassSphere));
    finalRender.add(make_shared<Sphere>(innerGlassSphere));


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
            
            finalRender.add(make_shared<Sphere>(center, 0.25f, sphereMat));
        }
    }

    Camera cam;

    cam.aspectRatio = 16.0f / 9.0f;
    cam.imageWidth = 320;
    cam.samplesPerPixel = 200;
    cam.maxDepth = 20;

    cam.verticalFOV = 40.0f;
    cam.lookfrom = Point3(5.0f, 6.0f, 0);
    cam.lookat = Point3(0, 1, 0);
    cam.vup = Vector3(0, 1, 0);

    cam.defocusAngle = 0;
    cam.focusDistance = 15.0f;

    cam.backgroundColor = Color(0.75f, 0.8f, 1);
    
    finalRender = HittableList(make_shared<BVHNode>(finalRender));

    cam.render(finalRender);
}


void experimentalScene()
{

    // Materials
    shared_ptr<LambertianMaterial> groundMat = make_shared<LambertianMaterial>(Color(0.2f, 0.9f, 0.1f));
    shared_ptr<LambertianMaterial> whiteMat = make_shared<LambertianMaterial>(Color(1));
    shared_ptr<MetalMaterial> mirrorMat = make_shared<MetalMaterial>(Color(0.6f), 0.01f);
    shared_ptr<MetalMaterial> goldMetalMat = make_shared<MetalMaterial>(Color(0.85f, 0.6f, 0.2f), 0.05f);
    shared_ptr<DielectricMaterial> orangeGlassMat = make_shared<DielectricMaterial>(1.5f, Color(1.0f, 0.8f, 0.65f));
    shared_ptr<DielectricMaterial> glassBubbleMat = make_shared<DielectricMaterial>(1.0f / 1.5f);
    shared_ptr<DielectricMaterial> blueGlassMat = make_shared<DielectricMaterial>(1.5f, Color(0.7f, 0.95f, 1.0f));
    auto checkerTex = make_shared<CheckerTexture>(0.75f, Color(0.1f), Color(0.9f));

    // World properties
    Scene world;

    world.add(make_shared<Sphere>(Point3(0, 0, -1.25f), 0.5f, goldMetalMat));
    world.add(make_shared<Sphere>(Point3(0, 0, 0), 0.5f, blueGlassMat));
    world.add(make_shared<Sphere>(Point3(0, 0, 0), 0.45f, glassBubbleMat));
    world.add(make_shared<Sphere>(Point3(0, -250.5f, -1), 250, make_shared<LambertianMaterial>(checkerTex)));
    // world.add(make_shared<Sphere>(Point3(-0.5f, 0.75f, -1), Point3(0.5f, 0.75f, -1), 0.25f, redMetalMat));
    world.add(make_shared<Sphere>(Point3(-1, 0, -1), 0.5f, mirrorMat));
    world.add(make_shared<Sphere>(Point3(1, 0, -1), 0.5f, orangeGlassMat));

    world = Scene(make_shared<BVHNode>(world));

    Camera cam;

    // Adjust camera settings
    cam.aspectRatio = 16.0f / 9.0f;
    cam.imageWidth = 640;
    cam.samplesPerPixel = 300;
    cam.maxDepth = 50;

    cam.verticalFOV = 21.25f;
    cam.lookfrom = Point3(3, 2.25f, 6);
    cam.lookat = Point3(0, 0, -1);
    cam.vup = Vector3(0, 1, 0);

    cam.defocusAngle = 0.0f;
    cam.focusDistance = (cam.lookfrom - cam.lookat).magnitude() + 0.25f;

    cam.backgroundColor = Color(0.75f, 0.8f, 1);

    cam.render(world);
}


void checkeredSpheres()
{
    Scene world;

    auto checkerTex = make_shared<CheckerTexture>(0.5f, Color(0.1f), Color(0.9f));

    world.add(make_shared<Sphere>(Point3(0, -10, 0), 10, make_shared<LambertianMaterial>(checkerTex)));
    world.add(make_shared<Sphere>(Point3(0, 10, 0), 10, make_shared<LambertianMaterial>(checkerTex)));

    Camera cam;

    cam.aspectRatio = 16.0f / 9.0f;
    cam.imageWidth = 320;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;

    cam.verticalFOV = 20;
    cam.lookfrom = Point3(13, 2, 3);
    cam.lookat = Point3(0, 0, 0);
    cam.vup = Vector3(0, 1, 0);

    cam.defocusAngle = 0;

    cam.backgroundColor = Color(0.75f, 0.8f, 1);

    world = Scene(make_shared<BVHNode>(world));

    cam.render(world);
}


void earthSphere()
{
    auto earthTexture = make_shared<ImageTexture>("EarthUV.png");
    auto earthSurface = make_shared<LambertianMaterial>(earthTexture);
    auto globe = make_shared<Sphere>(Point3(0), 2, earthSurface);

    Camera cam;

    cam.aspectRatio = 16.0f / 9.0f;
    cam.imageWidth = 640;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;

    cam.verticalFOV = 25;
    cam.lookfrom = Point3(0, 0, 10);
    cam.lookat = Point3(0);
    cam.vup = Vector3(0, 1, 0);

    cam.defocusAngle = 0;

    cam.render(Scene(globe));
}


void perlinSpheres()
{
    Scene world;

    auto perlinTexture = make_shared<NoiseTexture>(5, 7);

    world.add(make_shared<Sphere>(Point3(0, -250, 0), 250, make_shared<MetalMaterial>(perlinTexture, 0.35f)));
    world.add(make_shared<Sphere>(Point3(0, 2, 0), 2, make_shared<MetalMaterial>(perlinTexture, 0.35f)));

    Camera cam;

    cam.aspectRatio = 16.0f / 9.0f;
    cam.imageWidth = 640;
    cam.samplesPerPixel = 200;
    cam.maxDepth = 50;

    cam.verticalFOV = 30;
    cam.lookfrom = Point3(13, 4, 3);
    cam.lookat = Point3(0);
    cam.vup = Point3(0, 1, 0);

    cam.defocusAngle = 0;

    cam.backgroundColor = Color(0.75f, 0.8f, 1);

    cam.render(world);
}


void quads()
{
    Scene world;

    auto redMat = make_shared<LambertianMaterial>(Color(1.0f, 0.3f, 0.3f));
    auto yellowMat = make_shared<LambertianMaterial>(Color(0.8f, 0.8f, 0.4f));
    auto greenMat = make_shared<LambertianMaterial>(Color(0.3f, 1.0f, 0.3f));
    auto blueMat = make_shared<LambertianMaterial>(Color(0.2f, 0.2f, 1.0f));
    auto mirrorMat = make_shared<MetalMaterial>(Color(1), 0.2f);

    world.add(make_shared<Quad>(Point3(-3,-2, 5), Vector3(0, 0, -4), Vector3(0, 4, 0), redMat));
    world.add(make_shared<Quad>(Point3(-2,-2, 0), Vector3(4, 0, 0), Vector3(0, 4, 0), mirrorMat));
    world.add(make_shared<Quad>(Point3( 3,-2, 1), Vector3(0, 0, 4), Vector3(0, 4, 0), greenMat));
    world.add(make_shared<Quad>(Point3(-2, 3, 1), Vector3(4, 0, 0), Vector3(0, 0, 4), yellowMat));
    world.add(make_shared<Quad>(Point3(-2,-3, 5), Vector3(4, 0, 0), Vector3(0, 0,-4), blueMat));

    world = Scene(make_shared<BVHNode>(world));

    Camera cam;

    cam.aspectRatio = 16.0f / 9.0f;
    cam.imageWidth = 320;
    cam.samplesPerPixel = 300;
    cam.maxDepth = 50;

    cam.verticalFOV = 80;
    cam.lookfrom = Point3(0,0,9);
    cam.lookat = Point3(0,0,0);
    cam.vup = Vector3(0,1,0);

    cam.defocusAngle = 0;

    cam.backgroundColor = Color(0.75f, 0.8f, 1);

    cam.render(world);
}


void tris()
{
    Scene world;

    auto redMat = make_shared<LambertianMaterial>(Color(1.0f, 0.3f, 0.3f));
    auto yellowMat = make_shared<LambertianMaterial>(Color(0.8f, 0.8f, 0.4f));
    auto greenMat = make_shared<LambertianMaterial>(Color(0.3f, 1.0f, 0.3f));
    auto blueMat = make_shared<LambertianMaterial>(Color(0.2f, 0.2f, 1.0f));
    auto mirrorMat = make_shared<MetalMaterial>(Color(1), 0.2f);

    world.add(make_shared<Triangle>(Point3(-3,-2, 5), Vector3(0, 0, -4), Vector3(0, 4, 0), redMat));
    world.add(make_shared<Triangle>(Point3(-2,-2, 0), Vector3(4, 0, 0), Vector3(0, 4, 0), mirrorMat));
    world.add(make_shared<Triangle>(Point3( 3,-2, 1), Vector3(0, 0, 4), Vector3(0, 4, 0), greenMat));
    world.add(make_shared<Triangle>(Point3(-2, 3, 1), Vector3(4, 0, 0), Vector3(0, 0, 4), yellowMat));
    world.add(make_shared<Triangle>(Point3(-2,-3, 5), Vector3(4, 0, 0), Vector3(0, 0,-4), blueMat));

    world = Scene(make_shared<BVHNode>(world));

    Camera cam;

    cam.aspectRatio = 16.0f / 9.0f;
    cam.imageWidth = 320;
    cam.samplesPerPixel = 300;
    cam.maxDepth = 50;

    cam.verticalFOV = 80;
    cam.lookfrom = Point3(0,0,9);
    cam.lookat = Point3(0,0,0);
    cam.vup = Vector3(0,1,0);

    cam.defocusAngle = 0;

    cam.backgroundColor = Color(0.75f, 0.8f, 1);

    cam.render(world);
}


void simpleLight()
{
    Scene world;

    auto perlinTexture = make_shared<NoiseTexture>(4, 5);
    
    world.add(make_shared<Sphere>(Point3(0, 2, 0), 2, make_shared<LambertianMaterial>(perlinTexture)));
    world.add(make_shared<Quad>(Point3(-200, 0, -200), Vector3(400, 0, 0), Vector3(0, 0, 400), make_shared<LambertianMaterial>(perlinTexture)));
    
    auto orangeDiffuseLight = make_shared<DiffuseLightMaterial>(Color(2, 1, 0.5f));
    auto blueDiffuseLight = make_shared<DiffuseLightMaterial>(Color(0.5f, 1, 2));
    auto purpleDiffuseLight = make_shared<DiffuseLightMaterial>(Color(1.5f, 0.5f, 1.5f));
    
    world.add(make_shared<Quad>(Point3(3, 1, -5), Vector3(2, 0, 0), Vector3(0, 2, 0), orangeDiffuseLight));
    world.add(make_shared<Quad>(Point3(3, 1, 5), Vector3(2, 0, 0), Vector3(0, 2, 0), purpleDiffuseLight));
    world.add(make_shared<Sphere>(Point3(0, 7, 0), 2, blueDiffuseLight));

    world = Scene(make_shared<BVHNode>(world));

    Camera cam;

    cam.aspectRatio = 16.0f / 9.0f;
    cam.imageWidth = 640;
    cam.samplesPerPixel = 400;
    cam.maxDepth = 50;
    
    cam.verticalFOV = 20;
    cam.lookfrom = Point3(26, 3, 0);
    cam.lookat = Point3(0, 2, 0);
    cam.vup = Vector3(0, 1, 0);

    cam.defocusAngle = 0;

    cam.render(world);
}


void cornellBox()
{
    Scene world;

    auto redMaterial = make_shared<LambertianMaterial>(Color(0.75f, 0.05f, 0.05f));
    auto greenMaterial = make_shared<LambertianMaterial>(Color(0.05f, 0.75f, 0.05f));
    auto whiteMaterial = make_shared<LambertianMaterial>(Color(0.75f));
    auto lightMaterial = make_shared<DiffuseLightMaterial>(Color(10));

    // Green wall (left)
    world.add(make_shared<Quad>(Point3(55.5f, 0, 0), Vector3(0, 55.5f, 0), Vector3(0, 0, 55.5f), greenMaterial));
    // Red wall (right)
    world.add(make_shared<Quad>(Point3(0), Vector3(0, 55.5f, 0), Vector3(0, 0, 55.5f), redMaterial));
    // White wall (bottom)
    world.add(make_shared<Quad>(Point3(0), Vector3(55.5f, 0, 0), Vector3(0, 0, 55.5f), whiteMaterial));
    // White wall (top)
    world.add(make_shared<Quad>(Point3(55.5f), Vector3(-55.5f, 0, 0), Vector3(0, 0, -55.5f), whiteMaterial));
    // White wall (back)
    world.add(make_shared<Quad>(Point3(0, 0, 55.5f), Vector3(55.5f, 0, 0), Vector3(0, 55.5f, 0), whiteMaterial));
    // Emissive light
    world.add(make_shared<Quad>(Point3(34.3f, 55.4f, 34.3f), Vector3(-13.0f, 0, 0), Vector3(0, 0, -10.5f), lightMaterial));
    // Small Box
    shared_ptr<Hittable> smallBox = Box(Point3(0), Point3(16.5f, 33.0f, 16.5f), whiteMaterial);
    smallBox = make_shared<RotateY>(smallBox, 15);
    smallBox = make_shared<Translate>(smallBox, Vector3(26.5f, 0, 29.5f));
    world.add(smallBox);
    // Bigger Box
    shared_ptr<Hittable> bigBox = Box(Point3(0), Point3(16.5f), whiteMaterial);
    bigBox = make_shared<RotateY>(bigBox, -20);
    bigBox = make_shared<Translate>(bigBox, Vector3(13.0f, 0, 6.5f));
    world.add(bigBox);

    world = Scene(make_shared<BVHNode>(world));

    Camera cam;

    cam.aspectRatio = 1;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 200;
    cam.maxDepth = 100;
    
    cam.verticalFOV = 40;
    cam.lookfrom = Point3(27.8f, 27.8f, -80);
    cam.lookat = Point3(27.8f, 27.8f, 0);
    cam.vup = Vector3(0, 1, 0);

    cam.backgroundColor = Color(0);

    cam.defocusAngle = 0;

    cam.render(world);
}


void cornellBoxSmoke()
{
    Scene world;
    
    auto red   = make_shared<LambertianMaterial>(Color(.65, .05, .05));
    auto white = make_shared<LambertianMaterial>(Color(.73, .73, .73));
    auto green = make_shared<LambertianMaterial>(Color(.12, .45, .15));
    auto light = make_shared<DiffuseLightMaterial>(Color(7, 7, 7));
    
    world.add(make_shared<Quad>(Point3(555,0,0), Vector3(0,555,0), Vector3(0,0,555), green));
    world.add(make_shared<Quad>(Point3(0,0,0), Vector3(0,555,0), Vector3(0,0,555), red));
    world.add(make_shared<Quad>(Point3(113,554,127), Vector3(330,0,0), Vector3(0,0,305), light));
    world.add(make_shared<Quad>(Point3(0,555,0), Vector3(555,0,0), Vector3(0,0,555), white));
    world.add(make_shared<Quad>(Point3(0,0,0), Vector3(555,0,0), Vector3(0,0,555), white));
    world.add(make_shared<Quad>(Point3(0,0,555), Vector3(555,0,0), Vector3(0,555,0), white));
    
    shared_ptr<Hittable> box1 = Box(Point3(0,0,0), Point3(165,330,165), white);
    box1 = make_shared<RotateY>(box1, 15);
    box1 = make_shared<Translate>(box1, Vector3(265,0,295));
    
    shared_ptr<Hittable> box2 = Box(Point3(0,0,0), Point3(165,165,165), white);
    box2 = make_shared<RotateY>(box2, -18);
    box2 = make_shared<Translate>(box2, Vector3(130,0,65));
    
    world.add(make_shared<ConstantMedium>(box1, Color(0,0,0), 0.01));
    world.add(make_shared<ConstantMedium>(box2, Color(1,1,1), 0.01));
    
    Camera cam;
    
    cam.aspectRatio      = 1.0;
    cam.imageWidth       = 320;
    cam.samplesPerPixel = 1000;
    cam.maxDepth         = 50;
    cam.backgroundColor        = Color(0,0,0);
    
    cam.verticalFOV     = 40;
    cam.lookfrom = Point3(278, 278, -800);
    cam.lookat   = Point3(278, 278, 0);
    cam.vup      = Vector3(0,1,0);
    
    cam.defocusAngle = 0;
    
    cam.render(world);
}


int main()
{
    //experimentalScene();
    //earthSphere();
    //perlinSpheres();
    //quads();
    //tris();
    //simpleLight();
    //cornellBox();
    cornellBoxSmoke();
}

