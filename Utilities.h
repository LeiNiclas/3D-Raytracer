#ifndef UTILITIES_H
#define UTILITIES_H

#include <cmath>
#include <cstdlib>
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

inline float randomFloat() { return std::rand() / (RAND_MAX + 1.0f); }

inline float randomFloat(float min, float max) { return min + (max-min) * randomFloat(); }

// Common headers

#include "Color.h"
#include "Interval.h"
#include "Ray.h"
#include "Vector3.h"

#endif
