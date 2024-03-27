#ifndef FILAMENT_SHADER_OBJECT_H
#define FILAMENT_SHADER_OBJECT_H

#include "filament_object.h"

#include "filament/filament_engine_object.h"

namespace filament {
	class Material;
	class MaterialInstance;
}

class FilamentShaderObject final : public FilamentObject {
public:
	FilamentShaderObject();
	~FilamentShaderObject() override;

	FilamentEngineObject<filament::MaterialInstance> instantiate(const char *name = nullptr) const;

private:
	FilamentEngineObject<filament::Material> m_material;
};

#endif

