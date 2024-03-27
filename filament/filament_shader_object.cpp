#include "filament/filament_shader_object.h"

#include "filament/filament_rendering_server_backend.h"

#include <filament/Engine.h>
#include <filament/Material.h>

static const unsigned char fallbackMaterial[] = {
#include "builtin_fallback.h"
};

FilamentShaderObject::FilamentShaderObject() {

	m_material = FilamentEngineObject<filament::Material>(filament::Material::Builder().package(fallbackMaterial, sizeof(fallbackMaterial)).build(
		*FilamentRenderingServerBackend::filamentEngine()
	));
	if(!m_material) {
		throw std::runtime_error("failed to construct the fallback material");
	}
}

FilamentShaderObject::~FilamentShaderObject() = default;

FilamentEngineObject<filament::MaterialInstance> FilamentShaderObject::instantiate(const char *name) const {
	FilamentEngineObject<filament::MaterialInstance> result(m_material->createInstance(name));

	if(!result) {
		throw std::runtime_error("failed to create the material instance");
	}

	return result;
}
