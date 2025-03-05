#include <iostream>
#include "Vector3.h"
#include "Color.h"

// After building, execute the output file using
// build\Debug\outDebug.exe | set-content image.ppm -encoding String

int main()
{
    // Image properties
    const int imageWidth = 256;
    const int imageHeight = 256;

    // Rendering
    std::cout << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

    
    for (int y = 0; y < imageHeight; y++)
    {
        std::clog << "\rProcessing... (row " << y+1 << " of " << imageHeight << ")"
                  << std::flush; 

        for (int x = 0; x < imageWidth; x++)
        {
            Color pixelColor = Color
            (
                float(x) / (imageWidth - 1),
                float(y) / (imageHeight - 1),
                0
            );

            writeColor(std::cout, pixelColor);
        }
    }

    std::clog << "\rDone.\n";
}