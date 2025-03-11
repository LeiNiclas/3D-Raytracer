#ifndef AALIGNEDBBOX_H
#define AALIGNEDBBOX_H

#include "Interval.h"
#include "Vector3.h"
#include "Ray.h"

class AAlignedBBox
{
    private:
        void padToMinimum()
        {
            float delta = 0.0001f;
        
            if (x.size() < delta) x = x.expand(delta);
            if (y.size() < delta) y = y.expand(delta);
            if (z.size() < delta) z = z.expand(delta); 
        }


    public:
        static const AAlignedBBox empty;
        static const AAlignedBBox universe;

        Interval x;
        Interval y;
        Interval z;

        AAlignedBBox() {}

        AAlignedBBox(const Interval& x, const Interval& y, const Interval& z)
        : x(x), y(y), z(z) { padToMinimum(); }

        AAlignedBBox(const Point3& a, const Point3& b)
        {
            x = (a.x() <= b.x()) ? Interval(a.x(), b.x()) : Interval(b.x(), a.x());
            y = (a.y() <= b.y()) ? Interval(a.y(), b.y()) : Interval(b.y(), a.y());
            z = (a.z() <= b.z()) ? Interval(a.z(), b.z()) : Interval(b.z(), a.z());

            padToMinimum();
        }

        AAlignedBBox(const AAlignedBBox& bbox1, const AAlignedBBox& bbox2)
        {
            x = Interval(bbox1.x, bbox2.x);
            y = Interval(bbox1.y, bbox2.y);
            z = Interval(bbox1.z, bbox2.z);

            padToMinimum();
        }

        const Interval& axisInterval(int n) const
        {
            if (n == 0) return x;
            if (n == 1) return y;
            return z;
        }

        bool hit(const Ray& ray, Interval rayT) const
        {
            const Point3& rayOrigin = ray.origin();
            const Vector3& rayDirection = ray.direction();

            for (int axis = 0; axis < 3; axis++)
            {
                const Interval& ax = axisInterval(axis);
                const float adinv = 1.0f / rayDirection[axis];

                float t0 = (ax.min - rayOrigin[axis]) * adinv;
                float t1 = (ax.max - rayOrigin[axis]) * adinv;

                if (t0 < t1)
                {
                    if (t0 > rayT.min) rayT.min = t0;
                    if (t1 < rayT.max) rayT.max = t1;
                }
                else
                {
                    if (t0 < rayT.max) rayT.max = t0;
                    if (t1 > rayT.min) rayT.min = t1;
                }

                if (rayT.max <= rayT.min) return false;
            }

            return true;
        }

        int longestAxis() const
        {
            if (x.size() > y.size()) return x.size() > z.size() ? 0 : 2;
            else return y.size() > z.size() ? 1 : 2;
        }
};

const AAlignedBBox AAlignedBBox::empty = AAlignedBBox(Interval::empty, Interval::empty, Interval::empty);
const AAlignedBBox AAlignedBBox::universe = AAlignedBBox(Interval::universe, Interval::universe, Interval::universe);

#endif
