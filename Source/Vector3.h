#ifndef VECTOR3_H
#define VECTOR3_H


class Vector3
{
    public:
        float values[3];

        Vector3() : values{0, 0, 0} {}
        Vector3(float rgb) : values{rgb, rgb, rgb} {}
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

        /// @brief Calculate the length of a vector.
        /// @return The length of the given vector.
        float magnitude() const
        {
            return std::sqrt
            (
                values[0] * values[0] +
                values[1] * values[1] +
                values[2] * values[2]
            );
        }

        bool nearZero() const
        {
            float threshold = 1e-8;

            return  std::fabs(values[0]) < threshold &&
                    std::fabs(values[1]) < threshold &&
                    std::fabs(values[2]) < threshold;
        }

        /// @brief Generate a vector with random components between 0 and 1.
        /// @return A vector with random components between 0 and 1.
        static Vector3 randomVector()
        {
            return Vector3(randomFloat(), randomFloat(), randomFloat());
        }

        /// @brief Generate a vector with random components between min and max.
        /// @param min The minimum value for the vector components.
        /// @param max The maximum value for the vector components.
        /// @return A vector with random components.
        static Vector3 randomVector(float min, float max)
        {
            return Vector3(randomFloat(min, max), randomFloat(min, max), randomFloat(min, max));
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

inline Vector3 operator*(const Vector3& v1, const Vector3& v2)
{
    return Vector3
    (
        v1.values[0] * v2.values[0],
        v1.values[1] * v2.values[1],
        v1.values[2] * v2.values[2]
    );
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

/// @brief Calculate the dot product of two vectors.
/// @return Dot product of v1 * v2.
inline float dotP(const Vector3& v1, const Vector3& v2)
{
    return  v1.values[0] * v2.values[0] +
            v1.values[1] * v2.values[1] +
            v1.values[2] * v2.values[2];
}

/// @brief Calculate the cross product of two vectors.
/// @return Cross product of v1 x v2.
inline Vector3 crossP(const Vector3& v1, const Vector3& v2)
{
    return Vector3
    (
        v1.values[1] * v2.values[2] - v1.values[2] * v2.values[1],
        v1.values[2] * v2.values[0] - v1.values[0] * v2.values[2],
        v1.values[0] * v2.values[1] - v1.values[1] * v2.values[0]
    );
}

/// @brief Calculate the unit vector of vector v.
/// @param v The vector to normalize.
/// @return Unit vector of v.
inline Vector3 normalized(const Vector3& v)
{
    return v / v.magnitude();
}

/// @brief Generate a random unit vector.
/// @return New vector with random direction and magnitude of 1.
inline Vector3 randomUnitVector()
{
    while (true)
    {
        Vector3 v = Vector3::randomVector(-1.0f, 1.0f);
        float vSquared = dotP(v, v);
        
        if (1e-38 < vSquared && vSquared <= 1)
            return v / sqrt(vSquared);
    }
}


/// @brief Generate a random vector pointing away from a surface.
/// @param normal The normal of the surface.
/// @return new vector where its dot product with the normal is > 0. 
inline Vector3 randomOnHemisphere(const Vector3& normal)
{
    Vector3 onSurfaceVector = randomUnitVector();

    if (dotP(onSurfaceVector, normal) > 0.0f)
        return onSurfaceVector;
    else
        return onSurfaceVector;
}

/// @brief Generate a random point inside a unit disk.
/// @return new vector inside a uinit disk.
inline Vector3 randomInUnitDisk()
{
    while (true)
    {
        Vector3 p = Vector3(randomFloat(-1, 1), randomFloat(-1, 1), 0);

        if (dotP(p, p) < 1)
            return p;
    }
}


inline Vector3 reflect(const Vector3& v, const Vector3& normal)
{
    return v - 2.0f * dotP(v, normal) * normal;
}

inline Vector3 refract(const Vector3& v, const Vector3& normal, float etaInOverEtaOut)
{
    float cosTheta = std::fmin(dotP(-v, normal), 1.0f);
    
    Vector3 perpendicular = etaInOverEtaOut * (v + cosTheta * normal);
    Vector3 parallel = std::sqrt(std::fabs(1.0f - dotP(perpendicular, perpendicular))) * normal;

    return perpendicular + parallel;
}

#endif
