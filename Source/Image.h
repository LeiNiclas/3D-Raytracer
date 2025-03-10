#ifndef IMAGE_H
#define IMAGE_H

#ifdef _MSC_VER
    #pragma warning (push, 0)
#endif

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG

#define PATH_TO_IMAGES_FROM_CWD "External\\Images"

#include "../External/stb_image.h"
#include <cstdlib>
#include <iostream>
#include <filesystem>


class Image
{
    private:
        const int       bytesPerPixel = 3;
        float*          fileData = nullptr;
        unsigned char*  byteData = nullptr;
        int             imageWidth = 0;
        int             imageHeight = 0;
        int             bytesPerScanline = 0;


        static int clamp(int x, int min, int max)
        {
            if (x < min) return min;
            if (x > max) return max - 1;
            return x; 
        }

        static unsigned char floatToByte(float val)
        {
            if (val <= 0.0f) return 0;
            if (val >= 1.0f) return 255;
            return static_cast<unsigned char>(256.0f * val);
        }

        void convertToBytes()
        {
            int totalBytes = imageWidth * imageHeight * bytesPerPixel;
            byteData = new unsigned char[totalBytes];

            unsigned char* bytePtr = byteData;
            float* filePtr = fileData;

            for (int i = 0; i < totalBytes; i++, filePtr++, bytePtr++)
                *bytePtr = floatToByte(*filePtr);
        }

    public:
        Image() {}
        Image(const char* imageFilename)
        {
            std::string filename = std::string(imageFilename);
            auto imageDir = PATH_TO_IMAGES_FROM_CWD;
            
            std::filesystem::path cwd = std::filesystem::current_path();
            std::filesystem::path fullPath = cwd / imageDir / filename;


            if (load(fullPath.string())) return;

            std::cerr << "Error: could not load image '" << imageFilename << "'\n";
            std::cerr << "cwd: " << cwd << "\n";
            std::cerr << "imageDir: " << fullPath << "\n";
            std::cerr << "STB Error: " << stbi_failure_reason() << "\n"; // Print STB error message
        }

        ~Image()
        {
            delete[] byteData;
            STBI_FREE(fileData);
        }
        

        bool load(const std::string& filename)
        {
            int n = bytesPerPixel;
            
            fileData = stbi_loadf(filename.c_str(), &imageWidth, &imageHeight, &n, bytesPerPixel);

            if (fileData == nullptr) return false;

            bytesPerScanline = imageWidth * bytesPerPixel;

            convertToBytes();

            return true;
        }

        const unsigned char* pixelData(int x, int y) const
        {
            static unsigned char magenta[] = { 255, 0, 255 };

            if (byteData == nullptr) return magenta;

            x = clamp(x, 0, imageWidth);
            y = clamp(y, 0, imageHeight);

            return byteData + y * bytesPerScanline + x * bytesPerPixel;
        }

        int width() const { return (fileData == nullptr) ? 0 : imageWidth; }
        int height() const { return (fileData == nullptr) ? 0 : imageHeight; }

};


#endif
