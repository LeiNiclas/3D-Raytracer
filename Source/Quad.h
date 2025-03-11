#ifndef QUAD_H
#define QUAD_H

#include "Hittable.h"


class Quad : public Hittable
{
    private:
        Point3 Q;
        Vector3 u;
        Vector3 v;
        Vector3 w;
        Vector3 normal;
        float d;
        shared_ptr<Material> mat;
        AAlignedBBox bbox;


    public:
        Quad(const Point3& Q, const Vector3& u, const Vector3& v, shared_ptr<Material> mat)
        : Q(Q), u(u), v(v), mat(mat)
        {
            Vector3 n = crossP(u, v);
            normal = normalized(n);
            d = dotP(normal, Q);
            w = n / dotP(n, n);

            setBoundingBox();
        }

        virtual void setBoundingBox()
        {
            AAlignedBBox bboxDiagonal1 = AAlignedBBox(Q, Q + u + v);
            AAlignedBBox bboxDiagonal2 = AAlignedBBox(Q + u, Q + v);
            bbox = AAlignedBBox(bboxDiagonal1, bboxDiagonal2);
        }

        AAlignedBBox boundingBox() const override { return bbox; }

        bool hit(const Ray& ray, Interval rayT, HitRecord& record) const override
        {
            float denominator = dotP(normal, ray.direction());

            if (std::fabs(denominator) < 1e-9) return false;

            float t = (d - dotP(normal, ray.origin())) / denominator;

            if (!rayT.contains(t)) return false;

            Point3 planeIntersection = ray.at(t);
            Vector3 planarHitpointVector = planeIntersection - Q;
            float alpha = dotP(w, crossP(planarHitpointVector, v));
            float beta = dotP(w, crossP(u, planarHitpointVector));

            if (!interiorHit(alpha, beta, record)) return false;

            record.t = t;
            record.p = planeIntersection;
            record.mat = mat;
            record.setFaceNormal(ray, normal);

            return true;
        }

        virtual bool interiorHit(float a, float b, HitRecord& record) const
        {
            Interval unitInterval = Interval(0, 1);

            if (!unitInterval.contains(a) || !unitInterval.contains(b)) return false;

            record.u = a;
            record.v = b;

            return true;
        }
};


#endif
