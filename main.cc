#include <iostream>

// After building, execute the output file using
// build\Debug\outDebug.exe | set-content image.ppm -encoding String

int main()
{
    const int imageWidth = 256;
    const int imageHeight = 256;


    std::cout << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

    
    for (int y = 0; y < imageHeight; y++)
    {
        std::clog << "\rProcessing... (row " << y+1 << " of " << imageHeight << ")"
                  << std::flush; 

        for (int x = 0; x < imageWidth; x++)
        {
            float r = float(x) / (imageWidth - 1);
            float g = float(y) / (imageHeight - 1);
            float b = 0.0;

            int ir = int(255 * r);
            int ig = int(255 * g);
            int ib = int(255 * b);

            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }

    std::clog << "\rDone.\n";
}