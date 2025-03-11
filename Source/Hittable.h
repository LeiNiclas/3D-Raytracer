#ifndef HITTABLE_H
#define HITTABLE_H

#include "AAlignedBBox.h"


class Material;

class HitRecord
{
    public:
        Point3 p;
        Vector3 normal;
        shared_ptr<Material> mat;
        float t;
        float u;
        float v;
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

        virtual AAlignedBBox boundingBox() const = 0;
};


class Translate : public Hittable
{
    private:
        shared_ptr<Hittable> hittableObject;
        Vector3 offset;
        AAlignedBBox bbox;


    public:
        Translate(shared_ptr<Hittable> obj, const Vector3& offset)
        : hittableObject(obj), offset(offset) { bbox = hittableObject->boundingBox() + offset; }

        bool hit(const Ray& ray, Interval rayT, HitRecord& record) const override
        {
            Ray rayOffset = Ray(ray.origin() - offset, ray.direction(), ray.time());

            if (!hittableObject->hit(rayOffset, rayT, record))
                return false;
            
            record.p += offset;

            return true;
        }

        AAlignedBBox boundingBox() const override { return bbox; }
};


class RotateY : public Hittable
{
    private:
        shared_ptr<Hittable> hittableObject;
        float sinTheta;
        float cosTheta;
        AAlignedBBox bbox;
    

    public:
        RotateY(shared_ptr<Hittable> obj, float angle) : hittableObject(obj)
        {
            float radians = deg2rad(angle);

            sinTheta = std::sinf(radians);
            cosTheta = std::cosf(radians);

            bbox = hittableObject->boundingBox();

            Point3 min = Point3(infinity);
            Point3 max = Point3(-infinity);

            for (int i = 0; i < 2; i++)
                for (int j = 0; j < 2; j++)
                    for (int k = 0; k < 2; k++)
                    {
                        float x = i * bbox.x.max + (1 - i) * bbox.x.min;
                        float y = j * bbox.y.max + (1 - j) * bbox.y.min;
                        float z = k * bbox.z.max + (1 - j) * bbox.z.min;

                        float rotatedX = cosTheta * x + sinTheta * z;
                        float rotatedZ = -sinTheta * x + cosTheta * z;

                        Vector3 rotatedVector = Vector3(rotatedX, y, rotatedZ);

                        for (int l = 0; l < 3; l++)
                        {
                            min[l] = std::fmin(min[l], rotatedVector[l]);
                            max[l] = std::fmax(max[l], rotatedVector[l]);
                        }
                    }
            
            bbox = AAlignedBBox(min, max);
        }

        bool hit(const Ray& ray, Interval rayT, HitRecord& record) const override
        {
            Point3 origin = Point3
            (
                (cosTheta * ray.origin().x()) - (sinTheta * ray.origin().z()),
                ray.origin().y(),
                (sinTheta * ray.origin().x()) + (cosTheta * ray.origin().z())
            );

            Vector3 direction = Vector3
            (
                (cosTheta * ray.direction().x()) - (sinTheta * ray.direction().z()),
                ray.direction().y(),
                (sinTheta * ray.direction().x()) + (cosTheta * ray.direction().z())
            );

            Ray rotatedRay = Ray(origin, direction, ray.time());

            if (!hittableObject->hit(rotatedRay, rayT, record))
                return false;
            

            record.p = Point3
            (
                (cosTheta * record.p.x()) + (sinTheta * record.p.z()),
                record.p.y(),
                (-sinTheta * record.p.x()) + (cosTheta * record.p.z())
            );

            record.normal = Vector3
            (
                (cosTheta * record.normal.x()) + (sinTheta * record.normal.z()),
                record.normal.y(),
                (-sinTheta * record.normal.x()) + (cosTheta * record.normal.z())
            );

            return true;
        }

        AAlignedBBox boundingBox() const override { return bbox; }
};


#endif
