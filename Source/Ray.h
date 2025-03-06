#ifndef RAY_H
#define RAY_H

#include "Vector3.h"

using Point3 = Vector3;

class Ray
{
    private:
        Point3 orig;
        Vector3 dir;
        float tm;
    
    public:
        Ray() : orig(Point3()), dir(Vector3()) {}
        Ray(const Point3& origin, const Vector3& direction, float time)
        : orig(origin), dir(direction), tm(time) {} 
        Ray(const Point3& origin, const Vector3& direction) : Ray(origin, direction, 0) {}

        const Point3& origin() const { return orig; }
        const Vector3& direction() const { return dir; }
        const float time() const { return tm; }

        Point3 at(float t) const { return orig + t * dir; }
};

#endif
