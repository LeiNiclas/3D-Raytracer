#ifndef BVH_H
#define BVH_H

#include "AAlignedBBox.h"
#include "Hittable.h"
#include "HittableList.h"

#include <algorithm>


class BVHNode : public Hittable
{
    private:
        shared_ptr<Hittable> left;
        shared_ptr<Hittable> right;
        AAlignedBBox bbox;

        
        static bool boxCompare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b, int axisID)
        {
            Interval axisIntervalA = a->boundingBox().axisInterval(axisID);
            Interval axisIntervalB = b->boundingBox().axisInterval(axisID);

            return axisIntervalA.min < axisIntervalB.min;
        }

        static bool boxCompareX(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b)
        {
            return boxCompare(a, b, 0);
        }

        static bool boxCompareY(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b)
        {
            return boxCompare(a, b, 1);
        }

        static bool boxCompareZ(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b)
        {
            return boxCompare(a, b, 2);
        }

    
    public:
        BVHNode(HittableList list) : BVHNode(list.hittableObjects, 0, list.hittableObjects.size()) {}

        BVHNode(std::vector<shared_ptr<Hittable>>& hittableObjects, size_t start, size_t end)
        {
            bbox = AAlignedBBox::empty;

            for (size_t objectID = start; objectID < end; objectID++)
                bbox = AAlignedBBox(bbox, hittableObjects[objectID]->boundingBox());

            int axis = randomInt(0, 2);

            auto comparator = (axis == 0) ? boxCompareX
                            : (axis == 1) ? boxCompareY
                                          : boxCompareZ;
            
            size_t span = end - start;
            
            if (span == 1)
                left = right = hittableObjects[start];
            else if (span == 2)
            {
                left = hittableObjects[start];
                right = hittableObjects[start + 1];
            }
            else
            {
                std::sort(std::begin(hittableObjects) + start, std::begin(hittableObjects) + end, comparator);

                size_t middle = start + span / 2;
                
                left = make_shared<BVHNode>(hittableObjects, start, middle);
                right = make_shared<BVHNode>(hittableObjects, middle, end);
            }
        }

        bool hit(const Ray& ray, Interval rayT, HitRecord& record) const override
        {
            if (!bbox.hit(ray, rayT)) return false;

            bool hitLeft = left->hit(ray, rayT, record);
            bool hitRight = right->hit(ray, Interval(rayT.min, hitLeft ? record.t : rayT.max), record);

            return hitLeft || hitRight;
        }

        AAlignedBBox boundingBox() const override { return bbox; }
};

#endif
