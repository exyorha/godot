#include "filament/filament_light_object.h"
#include "filament/filament_rendering_server_backend.h"

#include <filament/Engine.h>

FilamentLightObject::FilamentLightObject(filament::LightManager::Type type) : m_type(type) {

}

FilamentLightObject::~FilamentLightObject() = default;

void FilamentLightObject::constructInstance(const utils::Entity &entity, filament::SkinningBuffer *skinWith) {
	(void)skinWith;

	filament::LightManager::Builder builder(m_type == filament::LightManager::Type::DIRECTIONAL ? filament::LightManager::Type::SUN : m_type);
	builder.direction(filament::math::float3(0.7, -1, -0.8));
	if(m_type == filament::LightManager::Type::DIRECTIONAL) {
		builder.intensity(2.0f);
	}
	builder.color(filament::LinearColor(1.0f, 1.0f, 1.0f));

	auto result = builder.build(*FilamentRenderingServerBackend::filamentEngine(), entity);
	if(result != decltype(result)::Success) {
		throw std::runtime_error("FilamentLightObject::constructInstance: failed to construct the instance");
	}
}

void FilamentLightObject::setColor(const Color &color) {

}
