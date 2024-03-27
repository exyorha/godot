#include "filament/filament_light_object.h"
#include "filament/filament_rendering_server_backend.h"

#include <filament/Engine.h>

FilamentLightObject::FilamentLightObject(filament::LightManager::Type type) : m_type(type) {

}

FilamentLightObject::~FilamentLightObject() = default;

void FilamentLightObject::constructInstance(const utils::Entity &entity) {
	filament::LightManager::Builder builder(m_type);
	builder.direction(filament::math::float3(0.0, -1.0, 0.0));

	auto result = builder.build(*FilamentRenderingServerBackend::filamentEngine(), entity);
	if(result != decltype(result)::Success) {
		throw std::runtime_error("FilamentLightObject::constructInstance: failed to construct the instance");
	}
}

void FilamentLightObject::setColor(const Color &color) {

}
