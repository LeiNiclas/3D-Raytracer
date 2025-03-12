#ifndef CONSTANTMEDIUM_H
#define CONSTANTMEDIUM_H

#include "Hittable.h"
#include "Material.h"
#include "Texture.h"


class ConstantMedium : public Hittable
{
    private:
        shared_ptr<Hittable> boundary;
        shared_ptr<Material> phaseFunction;
        float negativeInverseDensity;
    
    
    public:
        ConstantMedium(shared_ptr<Hittable> boundary, shared_ptr<Texture> tex, float density)
        : boundary(boundary), negativeInverseDensity(-1.0f / density),
          phaseFunction(make_shared<Isotropic>(tex)) {}
        
        ConstantMedium(shared_ptr<Hittable> boundary, const Color& albedo, float density)
        : boundary(boundary), negativeInverseDensity(-1.0f / density),
          phaseFunction(make_shared<Isotropic>(albedo)) {}

        
        bool hit(const Ray& ray, Interval rayT, HitRecord& record) const override
        {
            HitRecord record1;
            HitRecord record2;

            if (!boundary->hit(ray, Interval::universe, record1))
                return false;

            if (!boundary->hit(ray, Interval(record1.t + 0.0001f, infinity), record2))
                return false;
            
            if (record1.t < rayT.min) record1.t = rayT.min;
            if (record2.t > rayT.max) record2.t = rayT.max;

            if (record1.t >= record2.t)
                return false;
            
            if (record1.t < 0) record1.t = 0;


            float rayLength = ray.direction().magnitude();
            float distanceInsideBounds = (record2.t - record1.t) * rayLength;
            float hitDistance = negativeInverseDensity * std::log(randomFloat());

            if (hitDistance > distanceInsideBounds)
                return false;
            
            record.t = record1.t + hitDistance / rayLength;
            record.p = ray.at(record.t);
            record.normal = Vector3(1, 0, 0);
            record.isFrontFace = true;
            record.mat = phaseFunction;

            return true;
        }

        AAlignedBBox boundingBox() const override { return boundary->boundingBox(); }
};


#endif
