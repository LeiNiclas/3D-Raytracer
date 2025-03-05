#ifndef COLOR_H
#define COLOR_H

#include "Vector3.h"

#include <iostream>

using Color = Vector3;

void writeColor(std::ostream& out, const Color& pixelColor)
{
    float r = pixelColor.x();
    float g = pixelColor.y();
    float b = pixelColor.z();

    int rByte = int(255 * r);
    int gByte = int(255 * g);
    int bByte = int(255 * b);

    out << rByte << " " << gByte << " " << bByte << "\n";
}

#endif