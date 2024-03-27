#include "filament/filament_material_object.h"
#include "filament/filament_shader_object.h"

#include <filament/MaterialInstance.h>

#include <cstdio>

FilamentMaterialObject::FilamentMaterialObject() = default;

FilamentMaterialObject::~FilamentMaterialObject() = default;

void FilamentMaterialObject::setShader(const std::shared_ptr<FilamentShaderObject> &shader) {
	m_material = FilamentEngineObject<filament::MaterialInstance>();

	m_shader = shader;

	printf("FilamentMaterialObject(%p): setting shader to %p\n", this, shader.get());

	if(shader) {
		m_material = shader->instantiate();
	}
}
