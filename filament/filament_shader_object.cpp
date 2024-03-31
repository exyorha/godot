#include "filament/filament_shader_object.h"

#include "filament/filament_rendering_server_backend.h"

#include <filament/Engine.h>
#include <filament/Material.h>

FilamentShaderObject::FilamentShaderObject() {

}

FilamentShaderObject::~FilamentShaderObject() = default;

void FilamentShaderObject::createBuiltin(const unsigned char *filamentMaterialData, size_t size) {
	objectAboutToInvalidate();

	m_material = FilamentEngineObject<filament::Material>(
		filament::Material::Builder()
			.package(filamentMaterialData, size)
			.build(*FilamentRenderingServerBackend::filamentEngine())
	);
	if(!m_material) {
		throw std::runtime_error("failed to construct a built-in material");
	}
}

FilamentEngineObject<filament::MaterialInstance> FilamentShaderObject::instantiate(const char *name) const {
	if(!m_material) {
		return FilamentEngineObject<filament::MaterialInstance>();
	}

	FilamentEngineObject<filament::MaterialInstance> result(m_material->createInstance(name));

	if(!result) {
		throw std::runtime_error("failed to create the material instance");
	}

	return result;
}
