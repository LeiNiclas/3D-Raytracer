#ifndef UTILITIES_H
#define UTILITIES_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>

// std usings

using std::make_shared;
using std::shared_ptr;

// Universal constants

const float infinity = std::numeric_limits<float>::infinity();
const float pi = 3.1415926535;

// Utils

inline float deg2rad(float deg) { return deg * pi / 180.0f; }

// Common headers

#include "Color.h"
#include "Ray.h"
#include "Vector3.h"

#endif
