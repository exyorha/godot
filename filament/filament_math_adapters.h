#ifndef FILAMENT_MATH_ADAPTERS_H
#define FILAMENT_MATH_ADAPTERS_H

#include <math/mathfwd.h>

struct Transform3D;

template<typename T = double>
filament::math::details::TMat44<T> filamentTransformFromGodotTransform(const Transform3D &transform);

template<typename T>
Transform3D godotTransformFromFilamentTransform(const filament::math::details::TMat44<T> &transform);

#endif
