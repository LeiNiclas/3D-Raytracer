#ifndef VECTOR3_H
#define VECTOR3_H

#include <cmath>
#include <iostream>


class Vector3
{
    public:
        float values[3];

        Vector3() : values{0, 0, 0} {}
        Vector3(float x, float y, float z) : values{x, y, z} {}

        float x() const { return values[0]; }
        float y() const { return values[1]; }
        float z() const { return values[2]; }

        Vector3 operator-() const { return Vector3(-values[0], -values[1], -values[2]); }
        float operator[](int i) const { return values[i]; }
        float& operator[](int i) { return values[i]; }

        Vector3& operator+=(const Vector3 &other)
        {
            values[0] += other[0];
            values[1] += other[1];
            values[2] += other[2];

            return *this;
        }

        Vector3& operator*=(float s)
        {
            values[0] *= s;
            values[1] *= s;
            values[2] *= s;

            return *this;
        }

        Vector3& operator/=(float s)
        {
            return *this *= 1.0f / s;
        }

        float magnitude() const
        {
            return std::sqrt
            (
                values[0] * values[0] +
                values[1] * values[1] +
                values[2] * values[2]
            );
        }
};


inline std::ostream& operator<<(std::ostream& out, const Vector3 &v)
{
    return out << v.values[0] << " " << v.values[1] << " " << v.values[2];
}

inline Vector3 operator+(const Vector3& v1, const Vector3& v2)
{
    return Vector3
    (
        v1.values[0] + v2.values[0],
        v1.values[1] + v2.values[1],
        v1.values[2] + v2.values[2]
    );
}

inline Vector3 operator-(const Vector3& v1, const Vector3& v2)
{
    return v1 + (-v2);
}

inline Vector3 operator*(float s, const Vector3& v)
{
    return Vector3
    (
        s * v.values[0],
        s * v.values[1],
        s * v.values[2]
    );
}

inline Vector3 operator*(const Vector3& v, float s)
{
    return s * v;
}

inline Vector3 operator/(const Vector3& v, float s)
{
    return (1.0f / s) * v; 
}

inline float dotP(const Vector3& v1, const Vector3& v2)
{
    return  v1.values[0] * v2.values[1] +
            v1.values[1] * v2.values[1] +
            v1.values[2] * v2.values[2];
}

inline Vector3 crossP(const Vector3& v1, const Vector3& v2)
{
    return Vector3
    (
        v1.values[1] * v2.values[2] - v1.values[2] * v2.values[1],
        v1.values[2] * v2.values[0] - v1.values[0] * v2.values[2],
        v1.values[0] * v2.values[1] - v1.values[1] * v2.values[0]
    );
}

inline Vector3 normalized(const Vector3& v)
{
    return v / v.magnitude();
}

#endif
