#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "Hittable.h"
#include "HittableList.h"


class Quad : public Hittable
{
    private:
    Point3  Q;
    Vector3 u;
    Vector3 v;
    Vector3 w;
    Vector3 normal;
    float   d;
    shared_ptr<Material>    mat;
    AAlignedBBox            bbox;


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


class Triangle : public Hittable
{
    private:
        Point3  Q;
        Vector3 u;
        Vector3 v;
        Vector3 w;
        Vector3 normal;
        float   d;
        shared_ptr<Material>    mat;
        AAlignedBBox            bbox;


    public:
        Triangle(const Point3& Q, const Vector3& u, const Vector3& v, shared_ptr<Material> mat)
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
            AAlignedBBox bboxU = AAlignedBBox(Q, Q + u);
            AAlignedBBox bboxV = AAlignedBBox(Q, Q + v);

            bbox = AAlignedBBox(bboxU, bboxV);
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

            if (a < 0 || b < 0 || !unitInterval.contains(a + b)) return false;

            record.u = a;
            record.v = b;

            return true;
        }
};


class Sphere : public Hittable
{
    private:
        Ray center;
        float radius;
        shared_ptr<Material> mat;
        AAlignedBBox bbox;

        static void getSphereUV(const Point3& p, float& u, float& v)
        {
            float theta = std::acos(-p.y());
            float phi = std::atan2(-p.z(), p.x()) + pi;

            u = phi / (2 * pi);
            v = theta / pi;
        }

    public:
        Sphere(const Point3& center, float radius, shared_ptr<Material> mat)
        : center(center, Vector3(0)), radius(radius), mat(mat)
        {
            bbox = AAlignedBBox(center - Vector3(radius), center + Vector3(radius));
        }

        Sphere(const Point3& startCenter, const Point3& endCenter, float radius, shared_ptr<Material> mat)
        : center(startCenter, endCenter - startCenter), radius(radius), mat(mat)
        {
            AAlignedBBox bbox1 = AAlignedBBox(center.at(0) - Vector3(radius), center.at(0) + Vector3(radius));
            AAlignedBBox bbox2 = AAlignedBBox(center.at(1) - Vector3(radius), center.at(1) + Vector3(radius));

            bbox = AAlignedBBox(bbox1, bbox2);
        }

        bool hit(const Ray& ray, Interval rayT, HitRecord& record) const override
        {
            Point3 currentCenter = center.at(ray.time());
            Vector3 oc = currentCenter - ray.origin();

            float a = dotP(ray.direction(), ray.direction());
            float h = dotP(ray.direction(), oc);
            float c = dotP(oc, oc) - radius * radius;

            float discriminant = h * h - a * c;

            if (discriminant < 0)
                return false;
            
            discriminant = std::sqrt(discriminant);

            // Calculate the nearest root in accepted range
            float solution = (h - discriminant) / a;

            if (!rayT.surrounds(solution))
            {
                solution = (h + discriminant) / a;
                
                if (!rayT.surrounds(solution))
                    return false;
            }

            record.t = solution;
            record.p = ray.at(record.t);
            Vector3 outwardNormal = (record.p - currentCenter) / radius;
            record.setFaceNormal(ray, outwardNormal);
            record.mat = mat;
            getSphereUV(outwardNormal, record.u, record.v);

            return true;
        }

        AAlignedBBox boundingBox() const override { return bbox; }
};


// COMPOSITES
// Cube
inline shared_ptr<HittableList> Cube(const Point3& center, float sideLength, shared_ptr<Material> mat)
{
    auto faces = make_shared<HittableList>();

    float halfSideLength = 0.5f * sideLength;
            
    // Front-Bottom-Left
    Point3 boxCorner1 = center - Vector3(halfSideLength, halfSideLength, halfSideLength);
    // Back-Upper-Right
    Point3 boxCorner2 = center + Vector3(halfSideLength, halfSideLength, halfSideLength);
            
    Vector3 widthVector = Vector3(sideLength, 0, 0);
    Vector3 heightVector = Vector3(0, sideLength, 0);
    Vector3 depthVector = Vector3(0, 0, sideLength);

    // Front-Bottom-Left - Front-Upper-Right
    faces->add(make_shared<Quad>(boxCorner1, widthVector, heightVector, mat));
    // Front-Bottom-Left - Back-Upper-Left
    faces->add(make_shared<Quad>(boxCorner1, heightVector, depthVector, mat));
    // Front-Bottom-Left - Back-Bottom-Right
    faces->add(make_shared<Quad>(boxCorner1, widthVector, depthVector, mat));
    // Back-Upper-Right - Front-Upper-Left
    faces->add(make_shared<Quad>(boxCorner2, -widthVector, -depthVector, mat));
    // Back-Upper-Right - Back-Bottom-Left
    faces->add(make_shared<Quad>(boxCorner2, -widthVector, -heightVector, mat));
    // Back-Upper-Right - Front-Bottom-Right
    faces->add(make_shared<Quad>(boxCorner2, -heightVector, -depthVector, mat));

    return faces;
}
        
// Box
inline shared_ptr<HittableList> Box(const Point3& boxCorner1, const Point3& boxCorner2, shared_ptr<Material> mat)
{
    auto faces = make_shared<HittableList>();

    Vector3 widthVector = Vector3(boxCorner2.x() - boxCorner1.x(), 0, 0);
    Vector3 heightVector = Vector3(0, boxCorner2.y() - boxCorner1.y(), 0);
    Vector3 depthVector = Vector3(0, 0, boxCorner2.z() - boxCorner1.z());

    // Front-Bottom-Left - Front-Upper-Right
    faces->add(make_shared<Quad>(boxCorner1, widthVector, heightVector, mat));
    // Front-Bottom-Left - Back-Upper-Left
    faces->add(make_shared<Quad>(boxCorner1, heightVector, depthVector, mat));
    // Front-Bottom-Left - Back-Bottom-Right
    faces->add(make_shared<Quad>(boxCorner1, widthVector, depthVector, mat));
    // Back-Upper-Right - Front-Upper-Left
    faces->add(make_shared<Quad>(boxCorner2, -widthVector, -depthVector, mat));
    // Back-Upper-Right - Back-Bottom-Left
    faces->add(make_shared<Quad>(boxCorner2, -widthVector, -heightVector, mat));
    // Back-Upper-Right - Front-Bottom-Right
    faces->add(make_shared<Quad>(boxCorner2, -heightVector, -depthVector, mat));

    return faces;
}

// Pyramid
inline shared_ptr<HittableList> Pyramid(const Point3& baseCenter, float sidelength, float height, shared_ptr<Material> mat)
{
    auto faces = make_shared<HittableList>();

    // Corner vertices
    Point3 A = baseCenter + Vector3(sidelength / 2.0f, 0, sidelength / 2.0f);
    Point3 B = baseCenter + Vector3(sidelength / 2.0f, 0, -sidelength / 2.0f);
    Point3 C = baseCenter + Vector3(-sidelength / 2.0f, 0, -sidelength / 2.0f);
    Point3 D = baseCenter + Vector3(-sidelength / 2.0f, 0, sidelength / 2.0f);
    
    // Top point
    Point3 E = baseCenter + Vector3(0, height, 0);

    faces->add(make_shared<Triangle>(A, B - A, E - A, mat));
    faces->add(make_shared<Triangle>(A, D - A, E - A, mat));
    faces->add(make_shared<Triangle>(C, B - C, E - C, mat));
    faces->add(make_shared<Triangle>(C, D - C, E - C, mat));
    faces->add(make_shared<Quad>(A, B - A, D - A, mat));

    return faces;
}

// N-gon (2D flat on y-Plane)
inline shared_ptr<HittableList> NGon(int n, const Point3& C, float r, shared_ptr<Material> mat)
{
    auto faces = make_shared<HittableList>();

    float deltaTheta = 360.0f / n;
    Vector3 basisVector = (r, 0, r);
    Vector3 A = (r, C.y(), 0);
    Vector3 B;
    Vector3 tmp;

    for (int i = 1; i < n + 2; i++)
    {
        tmp = A;
        A = basisVector * Vector3(std::cosf(deg2rad(i * deltaTheta)), 0, std::sinf(deg2rad(i * deltaTheta)));
        B = tmp;

        faces->add(make_shared<Triangle>(C, A, B, mat));
    }

    return faces;
}

// Hexagon (2D flat on y-Plane)
inline shared_ptr<HittableList> Hexagon(const Point3& baseCenter, float r, shared_ptr<Material> mat)
{
    return NGon(6, baseCenter, r, mat);
}

// Disk (2D flat on y-Plane)
inline shared_ptr<HittableList> Disk(const Point3& baseCenter, float r, shared_ptr<Material> mat)
{
    return NGon(100, baseCenter, r, mat);
}

// N-Prism
inline shared_ptr<HittableList> NPrism(int n, const Point3& C, float r, float h, shared_ptr<Material> mat)
{
    auto faces = make_shared<HittableList>();

    auto bottomFace = NGon(n, C, r, mat);
    auto topFace = NGon(n, C + Vector3(0, h, 0), r, mat);
    
    faces->add(bottomFace);
    faces->add(topFace);

    float deltaTheta = 360.0f / n;

    for (int i = 0; i < n + 1; i++)
    {
        Vector3 A = C + Vector3(std::cosf(deg2rad(i * deltaTheta)), 0, std::sinf(deg2rad(i * deltaTheta)));
        Vector3 B = C + Vector3(std::cosf(deg2rad((i + 1) * deltaTheta)), 0, std::sinf(deg2rad((i + 1) * deltaTheta)));

        faces->add(make_shared<Quad>(A, B - A, Vector3(0, h, 0), mat));
    }

    return faces;
}


#endif
