#ifndef COLOR_H
#define COLOR_H

#include "Vector3.h"
#include "Interval.h"

using Color = Vector3;

void writeColor(std::ostream& out, const Color& pixelColor)
{
    float r = pixelColor.x();
    float g = pixelColor.y();
    float b = pixelColor.z();

    static const Interval intensity = Interval(0.0f, 0.999f);
    int rByte = int(256 * intensity.clamp(r));
    int gByte = int(256 * intensity.clamp(g));
    int bByte = int(256 * intensity.clamp(b));

    out << rByte << " " << gByte << " " << bByte << "\n";
}

#endif
