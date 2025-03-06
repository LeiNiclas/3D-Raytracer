#ifndef MATERIAL_H
#define MATERIAL_H

#include "Hittable.h"


class Material
{
    public:
        virtual ~Material() = default;

        virtual bool scatter
        (
            const Ray& ray,
            const HitRecord record,
            Color& attenuation,
            Ray& scattered
        ) const
        {
            return false;
        }
};


class LambertianMaterial : public Material
{
    private:
        Color albedo;


    public:
        LambertianMaterial(const Color& albedo) : albedo(albedo) {}

        bool scatter ( const Ray& ray, const HitRecord record, Color& attenuation, Ray& scattered) const override
        {
            Vector3 scatterDirection = record.normal + randomUnitVector();

            if (scatterDirection.nearZero())
                scatterDirection = record.normal;

            scattered = Ray(record.p, scatterDirection);
            attenuation = albedo;
            return true;
        }
};


class MetalMaterial : public Material
{
    private:
        Color albedo;
        float fuzz;
    

    public:
        MetalMaterial(const Color& albedo, float fuzz) : albedo(albedo), fuzz(fuzz) {}

        bool scatter ( const Ray& ray, const HitRecord record, Color& attenuation, Ray& scattered) const override
        {
            Vector3 reflected = reflect(ray.direction(), record.normal);
            reflected = normalized(reflected) + (fuzz * randomUnitVector());
            
            scattered = Ray(record.p, reflected);
            attenuation = albedo;
            
            return true;
        }
};

#endif
