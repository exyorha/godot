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
FilamentEngineObject<filament::MaterialInstance> FilamentShaderObject::createNewMaterialInstance() const {
	if(!m_material) {
		return FilamentEngineObject<filament::MaterialInstance>();
	}

	FilamentEngineObject<filament::MaterialInstance> result(m_material->createInstance());

	if(!result) {
		throw std::runtime_error("failed to create the material instance");
	}

	m_properties.apply(result.get());

	return result;
}

void FilamentShaderObject::applyOntoExistingMaterialInstance(filament::MaterialInstance *instance) const {
	m_properties.apply(instance);
}

Variant FilamentShaderObject::getParam(const StringName &name) {
	return m_properties.getParam(name);
}

void FilamentShaderObject::setParam(const StringName &name, const Variant &value) {
	m_properties.setParam(name, value);
	recalculateDerivedMaterials();
}

