#include "filament_math_adapters.h"

#include <core/math/transform_3d.h>
#include <math/mat4.h>

template<typename T>
filament::math::details::TMat44<T> filamentTransformFromGodotTransform(const Transform3D &transform) {
	filament::math::details::TMat33<T> basisMatrix(
		filament::math::details::TVec3<T>(transform.basis.rows[0].x, transform.basis.rows[1].x, transform.basis.rows[2].x),
		filament::math::details::TVec3<T>(transform.basis.rows[0].y, transform.basis.rows[1].y, transform.basis.rows[2].y),
		filament::math::details::TVec3<T>(transform.basis.rows[0].z, transform.basis.rows[1].z, transform.basis.rows[2].z)
	);
	filament::math::details::TVec3<T> translationVector(transform.origin.x, transform.origin.y, transform.origin.z);

	return filament::math::details::TMat44<T>(basisMatrix, translationVector);
}
template<typename T>
Transform3D godotTransformFromFilamentTransform(const filament::math::details::TMat44<T> &transform) {
	Basis basis(
		transform[0][0], transform[1][0], transform[2][0],
		transform[0][1], transform[1][1], transform[2][1],
		transform[0][2], transform[1][2], transform[2][2]);

	return Transform3D(basis, Vector3(transform[3][0], transform[3][1], transform[3][2]));

}

template filament::math::details::TMat44<float> filamentTransformFromGodotTransform(const Transform3D &transform);
template filament::math::details::TMat44<double> filamentTransformFromGodotTransform(const Transform3D &transform);

template Transform3D godotTransformFromFilamentTransform<float>(const filament::math::details::TMat44<float> &transform);
template Transform3D godotTransformFromFilamentTransform<double>(const filament::math::details::TMat44<double> &transform);
