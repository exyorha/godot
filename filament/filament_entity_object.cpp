#include "filament/filament_entity_object.h"
#include "filament/filament_rendering_server_backend.h"

#include <filament/Engine.h>
#include <filament/TransformManager.h>
#include <utils/EntityManager.h>
#include <math/mat4.h>

FilamentEntityObject::FilamentEntityObject() {
	m_entity = utils::EntityManager::get().create();
	if(m_entity.isNull()) {
		throw std::bad_alloc();
	}

	FilamentRenderingServerBackend::filamentEngine()->getTransformManager().create(m_entity);
}

FilamentEntityObject::~FilamentEntityObject() {
	FilamentRenderingServerBackend::filamentEngine()->destroy(m_entity);
	utils::EntityManager::get().destroy(m_entity);
}

void FilamentEntityObject::setTransform(const Transform3D &transform) {
	auto &transformManager = FilamentRenderingServerBackend::filamentEngine()->getTransformManager();

	auto instance = transformManager.getInstance(m_entity);
	if(instance) {
		filament::math::mat3 basisMatrix(
			filament::math::double3(transform.basis.rows[0].x, transform.basis.rows[1].x, transform.basis.rows[2].x),
			filament::math::double3(transform.basis.rows[0].y, transform.basis.rows[1].y, transform.basis.rows[2].y),
			filament::math::double3(transform.basis.rows[0].z, transform.basis.rows[1].z, transform.basis.rows[2].z)
		);
		filament::math::double3 translationVector(transform.origin.x, transform.origin.y, transform.origin.z);

		transformManager.setTransform(instance, filament::math::mat4(basisMatrix, translationVector));
	}
}
