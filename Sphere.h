#ifndef SPHERE_H
#define SPHERE_H

#include "Hittable.h"


class Sphere : public Hittable
{
    private:
        Point3 center;
        float radius;
    
    public:
        Sphere(const Point3& center, float radius) : center(center), radius(radius) {}

        bool hit(const Ray& ray, Interval rayT, HitRecord& record) const override
        {
            Vector3 oc = center - ray.origin();

            float a = dotP(ray.direction(), ray.direction());
            float h = dotP(ray.direction(), oc);
            float c = dotP(oc, oc) - radius * radius;

            float discriminant = h * h - a * c;

            if (discriminant < 0)
                return false;
            
            discriminant = std::sqrt(discriminant);

            // Calculate the nearest root in accepted range
            float solution = (h - discriminant) / a;

            if (!rayT.surronds(solution))
            {
                solution = (h + discriminant);
                
                if (!rayT.surronds(solution))
                    return false;
            }

            record.t = solution;
            record.p = ray.at(record.t);
            Vector3 outwardNormal = (record.p - center) / radius;
            record.setFaceNormal(ray, outwardNormal);

            return true;
        }
};



#endif
