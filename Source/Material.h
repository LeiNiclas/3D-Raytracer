#ifndef MATERIAL_H
#define MATERIAL_H

#include "Hittable.h"
#include "Texture.h"

class Material
{
    public:
        virtual ~Material() = default;

        virtual bool scatter
        (
            const Ray& ray,
            const HitRecord& record,
            Color& attenuation,
            Ray& scattered
        ) const
        {
            return false;
        }

        virtual Color emitted(float u, float v, const Point3& p) const
        {
            return Color(0);
        }
};


class LambertianMaterial : public Material
{
    private:
        shared_ptr<Texture> tex;


    public:
        LambertianMaterial(const Color& albedo) : tex(make_shared<SolidColorTexture>(albedo)) {}
        LambertianMaterial(shared_ptr<Texture> texture) : tex(texture) {}

        bool scatter(const Ray& ray, const HitRecord& record, Color& attenuation, Ray& scattered) const override
        {
            Vector3 scatterDirection = record.normal + randomUnitVector();

            if (scatterDirection.nearZero())
                scatterDirection = record.normal;

            scattered = Ray(record.p, scatterDirection, ray.time());
            attenuation = tex->value(record.u, record.v, record.p);
            return true;
        }
};


class MetalMaterial : public Material
{
    private:
        float fuzz;
        shared_ptr<Texture> tex;
    

    public:
        MetalMaterial(const Color& albedo, float fuzz) : fuzz(fuzz), tex(make_shared<SolidColorTexture>(albedo)) {}
        MetalMaterial(shared_ptr<Texture> texture, float fuzz) : tex(texture), fuzz(fuzz) {}

        bool scatter(const Ray& ray, const HitRecord& record, Color& attenuation, Ray& scattered) const override
        {
            Vector3 reflected = reflect(ray.direction(), record.normal);
            reflected = normalized(reflected) + (fuzz * randomUnitVector());
            
            scattered = Ray(record.p, reflected, ray.time());
            attenuation = tex->value(record.u, record.v, record.p);
            
            return true;
        }
};


class DielectricMaterial : public Material
{
    private:
        float refractionIndex;
        Color tint;

        static float reflectance(float cos, float refractionIndex)
        {
            float r0 = (1.0f - refractionIndex) / (1.0f + refractionIndex);
            r0 = r0 * r0;

            return r0 + (1.0f - r0) * std::pow((1.0f - cos), 5.0f);
        }

    
    public:
        DielectricMaterial(float refractionIndex) : refractionIndex(refractionIndex), tint(Color(1)) {}
        DielectricMaterial(float refractionIndex, Color tint) : refractionIndex(refractionIndex), tint(tint) {}

        bool scatter(const Ray& ray, const HitRecord& record, Color& attenuation, Ray& scattered) const override
        {
            attenuation = tint;
            
            float ri = record.isFrontFace ? (1.0f / refractionIndex) : refractionIndex;

            Vector3 unitDirection = normalized(ray.direction());
            float cosTheta = std::fmin(dotP(-unitDirection, record.normal), 1.0f);
            float sinTheta = std::sqrt(1.0f - cosTheta * cosTheta);

            bool cannotRefract = ri * sinTheta > 1.0f;

            Vector3 direction;

            if (cannotRefract || reflectance(cosTheta, ri) > randomFloat())
                direction = reflect(unitDirection, record.normal);
            else
                direction = refract(unitDirection, -record.normal, ri);

            scattered = Ray(record.p, direction, ray.time());

            return true;
        }
};


class DiffuseLightMaterial : public Material
{
    private:
        shared_ptr<Texture> tex;


    public:
        DiffuseLightMaterial(shared_ptr<Texture> texture) : tex(texture) {}
        DiffuseLightMaterial(const Color& emit) : tex(make_shared<SolidColorTexture>(emit)) {}

        Color emitted(float u, float v, const Point3& p) const override
        {
            return tex->value(u, v, p);
        }
};


class Isotropic : public Material
{
    private:
        shared_ptr<Texture> tex;
    
    
    public:
        Isotropic(const Color& albedo) : tex(make_shared<SolidColorTexture>(albedo)) {}
        Isotropic(shared_ptr<Texture> texture) : tex(texture) {}

        bool scatter(const Ray& ray, const HitRecord& record, Color& attenuation, Ray& scattered) const override
        {
            scattered = Ray(record.p, randomUnitVector(), ray.time());
            attenuation = tex->value(record.u, record.v, record.p);

            return true;
        }
};

#endif
