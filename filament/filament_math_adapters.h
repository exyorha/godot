#ifndef FILAMENT_MATH_ADAPTERS_H
#define FILAMENT_MATH_ADAPTERS_H

#include <core/math/vector2.h>
#include <core/math/color.h>

#include <math/mathfwd.h>

struct Transform3D;
struct Vector2;

template<typename T = double>
filament::math::details::TMat44<T> filamentTransformFromGodotTransform(const Transform3D &transform);

template<typename T>
Transform3D godotTransformFromFilamentTransform(const filament::math::details::TMat44<T> &transform);

template<typename T = float>
inline filament::math::details::TVec2<T> filamentVectorFromGodotVector(const Vector2 &vector) {
	return filament::math::details::TVec2<T>(vector.x, vector.y);
}

template<typename T = float>
inline filament::math::details::TVec4<T> filamentVectorFromGodotVector(const Color &color) {
	return filament::math::details::TVec4<T>(color.r, color.g, color.b, color.a);
}

#endif
