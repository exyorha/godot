#ifndef FILAMENT_MATERIAL_OBJECT_H
#define FILAMENT_MATERIAL_OBJECT_H

#include "filament_object.h"

#include "filament/filament_engine_object.h"

namespace filament {
	class MaterialInstance;
}

class FilamentShaderObject;

class FilamentMaterialObject final : public FilamentObject {
public:
	FilamentMaterialObject();
	~FilamentMaterialObject() override;

	void setShader(const std::shared_ptr<FilamentShaderObject> &shader);

	inline filament::MaterialInstance *materialInstance() const {
		return m_material.get();
	}

private:
	std::shared_ptr<FilamentShaderObject> m_shader;
	FilamentEngineObject<filament::MaterialInstance> m_material;
};

#endif


