#ifndef COLOR_H
#define COLOR_H

#include "Vector3.h"
#include "Interval.h"

using Color = Vector3;


inline float linear2gamma(float linearValue)
{
    if (linearValue > 0.0f)
        return std::sqrt(linearValue);
    
    return 0.0f;
}


void writeColor(std::ostream& out, const Color& pixelColor)
{
    float r = pixelColor.x();
    float g = pixelColor.y();
    float b = pixelColor.z();

    // Linear to gamma transform
    r = linear2gamma(r);
    g = linear2gamma(g);
    b = linear2gamma(b);

    // Translation of [0, 1] values to [0, 255] space
    static const Interval intensity = Interval(0.0f, 0.999f);
    int rByte = int(256 * intensity.clamp(r));
    int gByte = int(256 * intensity.clamp(g));
    int bByte = int(256 * intensity.clamp(b));

    out << rByte << " " << gByte << " " << bByte << "\n";
}

#endif
