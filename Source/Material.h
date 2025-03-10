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
        Color albedo;
        float fuzz;
    

    public:
        MetalMaterial(const Color& albedo, float fuzz) : albedo(albedo), fuzz(fuzz) {}

        bool scatter(const Ray& ray, const HitRecord& record, Color& attenuation, Ray& scattered) const override
        {
            Vector3 reflected = reflect(ray.direction(), record.normal);
            reflected = normalized(reflected) + (fuzz * randomUnitVector());
            
            scattered = Ray(record.p, reflected, ray.time());
            attenuation = albedo;
            
            return true;
        }
};


class DielectricMaterial : public Material
{
    private:
        float refractionIndex;

        static float reflectance(float cos, float refractionIndex)
        {
            float r0 = (1.0f - refractionIndex) / (1.0f + refractionIndex);
            r0 = r0 * r0;

            return r0 + (1.0f - r0) * std::pow((1.0f - cos), 5.0f);
        }

    
    public:
        DielectricMaterial(float refractionIndex) : refractionIndex(refractionIndex) {}

        bool scatter(const Ray& ray, const HitRecord& record, Color& attenuation, Ray& scattered) const override
        {
            attenuation = Color(1.0f);
            
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

#endif
