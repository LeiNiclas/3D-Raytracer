#ifndef HITTABLE_H
#define HITTABLE_H

#include "Utilities.h"


class Material;

struct HitRecord
{
    Point3 p;
    Vector3 normal;
    shared_ptr<Material> mat;
    float t;
    bool isFrontFace;

    void setFaceNormal(const Ray& ray, const Vector3& outwardNormal)
    {
        // outwarNormal is required to have unit length.
        isFrontFace = dotP(ray.direction(), outwardNormal) < 0;
        normal = isFrontFace ? outwardNormal : -outwardNormal;
    }
};


class Hittable
{
    public:
        virtual ~Hittable() = default;

        virtual bool hit(const Ray& ray, Interval rayT, HitRecord& record) const = 0;
};


#endif
