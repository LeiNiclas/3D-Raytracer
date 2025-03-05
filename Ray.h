#ifndef RAY_H
#define RAY_H

#include "Vector3.h"

using Point3 = Vector3;

class Ray
{
    private:
        Point3 origin;
        Vector3 direction;
    
    public:
        Ray() {}
        Ray(const Point3& origin, const Vector3& direction) {}

        const Point3& origin() const { return origin; }
        const Vector3& direction() const { return direction; }

        Point3 at(float t) const { return origin + t * direction; }
};

#endif
