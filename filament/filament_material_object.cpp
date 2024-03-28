#include "filament/filament_material_object.h"
#include "filament/filament_shader_object.h"

#include <filament/MaterialInstance.h>

FilamentMaterialObject::FilamentMaterialObject() : m_shader(this) {

}

FilamentMaterialObject::~FilamentMaterialObject() = default;

void FilamentMaterialObject::resetMaterialInstance() {

	if(m_material) {
		objectAboutToInvalidate();
		m_material = FilamentEngineObject<filament::MaterialInstance>();
	}

	markDirty();
}

void FilamentMaterialObject::setShader(const std::shared_ptr<FilamentShaderObject> &shader) {
	resetMaterialInstance();

	m_shader = shader;
}

filament::MaterialInstance *FilamentMaterialObject::materialInstance() {
	clean();

	return m_material.get();
}

void FilamentMaterialObject::doClean() {
	if(m_shader) {
		m_material = m_shader->instantiate();
		m_material->setCullingMode(filament::MaterialInstance::CullingMode::FRONT);
	}
}

void FilamentMaterialObject::controlledObjectAboutToInvalidate(FilamentControlledObjectReferenceBase *linkedViaReference) {
	FilamentObject::controlledObjectAboutToInvalidate(linkedViaReference);

	if(linkedViaReference == &m_shader) {
		resetMaterialInstance();
	}
}
