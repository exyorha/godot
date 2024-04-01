#ifndef FILAMENT_MATERIAL_INSTANCE_SOURCE_H
#define FILAMENT_MATERIAL_INSTANCE_SOURCE_H

#include "filament/filament_object.h"
#include "filament/filament_engine_object.h"

#include "core/variant/variant.h"

namespace filament {
	class MaterialInstance;
};

class FilamentMaterialInstanceSource : public FilamentObject {
protected:
	FilamentMaterialInstanceSource();

public:
	~FilamentMaterialInstanceSource() override;

	virtual FilamentEngineObject<filament::MaterialInstance> createNewMaterialInstance() const = 0;
	virtual void applyOntoExistingMaterialInstance(filament::MaterialInstance  *instance) const = 0;

	virtual Variant getParam(const StringName &name) = 0;
	virtual void setParam(const StringName &name, const Variant &value) = 0;

protected:
	void recalculateDerivedMaterials();
};

#endif
