#ifndef TEXTURE_H
#define TEXTURE_H

#include "Image.h"


class Texture
{
    public:
        virtual ~Texture() = default;

        virtual Color value(float u, float v, const Point3& p) const = 0;
};


class SolidColorTexture : public Texture
{
    private:
        Color albedo;
    

    public:
        SolidColorTexture(const Color& albedo) : albedo(albedo) {}
        SolidColorTexture(float r, float g, float b) : SolidColorTexture(Color(r, g, b)) {}

        Color value(float u, float v, const Point3& p) const override { return albedo; }
};


class CheckerTexture : public Texture
{
    private:
        float invScale;
        shared_ptr<Texture> evenTex;
        shared_ptr<Texture> oddTex;
    
    
    public:
        CheckerTexture(float scale, shared_ptr<Texture> evenTexture, shared_ptr<Texture> oddTexture)
        : invScale(1.0f / scale), evenTex(evenTexture), oddTex(oddTexture) {}

        CheckerTexture(float scale, const Color& c1, const Color& c2)
        : CheckerTexture(scale, make_shared<SolidColorTexture>(c1), make_shared<SolidColorTexture>(c2)) {}

        Color value(float u, float v, const Point3& p) const override
        {
            int x = int(std::floor(invScale * p.x()));
            int y = int(std::floor(invScale * p.y()));
            int z = int(std::floor(invScale * p.z()));

            bool isEven = (x + y + z) % 2 == 0;

            return isEven ? evenTex->value(u, v, p) : oddTex->value(u, v, p);
        }
};


class ImageTexture : public Texture
{
    private:
        Image img;


    public:
        ImageTexture(const char* filename) : img(filename) {}

        Color value(float u, float v, const Point3& p) const override
        {
            if (img.height() <= 0) return Color(0, 1, 1);

            u = Interval(0, 1).clamp(u);
            v = 1.0f - Interval(0, 1).clamp(v);

            int i = int(u * img.width());
            int j = int(v * img.height());
            const unsigned char* pixel = img.pixelData(i, j);

            float colorScale = 1.0f / 255.0f;
            
            return Color(colorScale * pixel[0], colorScale * pixel[1], colorScale * pixel[2]);
        }
};


#endif
