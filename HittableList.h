#ifndef HITTABLELIST_H
#define HITTABLELIST_H

#include "Hittable.h"
#include <vector>


class HittableList: public Hittable
{
    public:
        std::vector<shared_ptr<Hittable>> hittableObjects;

        HittableList() {}
        HittableList(shared_ptr<Hittable> hittableObject) { add(hittableObject); }

        void clear() { hittableObjects.clear(); }
        
        void add(shared_ptr<Hittable> hittableObject) { hittableObjects.push_back(hittableObject); }

        bool hit(const Ray& ray, float rayMinT, float rayMaxT, HitRecord& record) const override
        {
            HitRecord tmpRecord;
            bool hit = false;

            float closestHit = rayMaxT;

            for (const shared_ptr<Hittable>& hittableObject : hittableObjects)
            {
                if (hittableObject->hit(ray, rayMinT, closestHit, tmpRecord))
                {
                    hit = true;
                    closestHit = tmpRecord.t;
                    record = tmpRecord;
                }
            }

            return hit;
        }
};


#endif