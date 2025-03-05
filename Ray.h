#ifndef RAY_H
#define RAY_H

#include "Vector3.h"

using Point3 = Vector3;

class Ray
{
    private:
        Point3 orig;
        Vector3 dir;
    
    public:
        Ray() : orig(Point3()), dir(Vector3()) {}
        Ray(const Point3& origin, const Vector3& direction) : orig(origin), dir(direction) {}

        const Point3& origin() const { return orig; }
        const Vector3& direction() const { return dir; }

        Point3 at(float t) const { return orig + t * dir; }
};

#endif
