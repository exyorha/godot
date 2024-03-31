#include "filament/filament_entity_object.h"
#include "filament/filament_rendering_server_backend.h"
#include "filament/filament_math_adapters.h"

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
		transformManager.setTransform(instance, filamentTransformFromGodotTransform(transform));
	}
}

void FilamentEntityObject::setParent(const std::shared_ptr<FilamentEntityObject> &parentEntity) {
	auto &transformManager = FilamentRenderingServerBackend::filamentEngine()->getTransformManager();

	auto instance = transformManager.getInstance(m_entity);
	if(instance) {
		filament::TransformManager::Instance parentInstance;
		if(parentEntity) {
			parentInstance = transformManager.getInstance(parentEntity->entity());
		}

		transformManager.setParent(instance, parentInstance);
	}
}
