#ifndef SPHERE_H
#define SPHERE_H

#include "Hittable.h"


class Sphere : public Hittable
{
    private:
        Ray center;
        float radius;
        shared_ptr<Material> mat;
        AAlignedBBox bbox;

    public:
        Sphere(const Point3& center, float radius, shared_ptr<Material> mat)
        : center(center, Vector3(0)), radius(radius), mat(mat)
        {
            bbox = AAlignedBBox(center - Vector3(radius), center + Vector3(radius));
        }

        Sphere(const Point3& startCenter, const Point3& endCenter, float radius, shared_ptr<Material> mat)
        : center(startCenter, endCenter - startCenter), radius(radius), mat(mat)
        {
            AAlignedBBox bbox1 = AAlignedBBox(center.at(0) - Vector3(radius), center.at(0) + Vector3(radius));
            AAlignedBBox bbox2 = AAlignedBBox(center.at(1) - Vector3(radius), center.at(1) + Vector3(radius));

            bbox = AAlignedBBox(bbox1, bbox2);
        }

        bool hit(const Ray& ray, Interval rayT, HitRecord& record) const override
        {
            Point3 currentCenter = center.at(ray.time());
            Vector3 oc = currentCenter - ray.origin();

            float a = dotP(ray.direction(), ray.direction());
            float h = dotP(ray.direction(), oc);
            float c = dotP(oc, oc) - radius * radius;

            float discriminant = h * h - a * c;

            if (discriminant < 0)
                return false;
            
            discriminant = std::sqrt(discriminant);

            // Calculate the nearest root in accepted range
            float solution = (h - discriminant) / a;

            if (!rayT.surrounds(solution))
            {
                solution = (h + discriminant) / a;
                
                if (!rayT.surrounds(solution))
                    return false;
            }

            record.t = solution;
            record.p = ray.at(record.t);
            Vector3 outwardNormal = (record.p - currentCenter) / radius;
            record.setFaceNormal(ray, outwardNormal);
            record.mat = mat;

            return true;
        }

        AAlignedBBox boundingBox() const override { return bbox; }
};



#endif
