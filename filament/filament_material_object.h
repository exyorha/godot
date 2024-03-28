#ifndef FILAMENT_MATERIAL_OBJECT_H
#define FILAMENT_MATERIAL_OBJECT_H

#include "filament_object.h"

#include "filament/filament_engine_object.h"
#include "filament/filament_controlled_object_reference.h"

namespace filament {
	class MaterialInstance;
}

class FilamentShaderObject;

class FilamentMaterialObject final : public FilamentObject {
public:
	FilamentMaterialObject();
	~FilamentMaterialObject() override;

	void setShader(const std::shared_ptr<FilamentShaderObject> &shader);

	filament::MaterialInstance *materialInstance();

protected:
	void doClean() override;
	void controlledObjectAboutToInvalidate(FilamentControlledObjectReferenceBase *linkedViaReference) override;

private:
	void resetMaterialInstance();

	FilamentControlledObjectReference<FilamentShaderObject> m_shader;
	FilamentEngineObject<filament::MaterialInstance> m_material;
};

#endif


