#ifndef HITTABLE_H
#define HITTABLE_H

#include "Utilities.h"

struct HitRecord
{
    Point3 p;
    Vector3 normal;
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

        virtual bool hit(const Ray& ray, float rayMinT, float rayMaxT, HitRecord& record) const = 0;
};


#endif
