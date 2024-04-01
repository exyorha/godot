#ifndef FILAMENT_MATERIAL_OBJECT_H
#define FILAMENT_MATERIAL_OBJECT_H

#include "filament/filament_material_instance_source.h"
#include "filament/filament_controlled_object_reference.h"
#include "filament/filament_material_property_bundle.h"

namespace filament {
	class MaterialInstance;
}

class FilamentShaderObject;

class FilamentMaterialObject final : public FilamentMaterialInstanceSource {
public:
	FilamentMaterialObject();
	~FilamentMaterialObject() override;

	void setParent(const std::shared_ptr<FilamentMaterialInstanceSource> &parent);
	void setFallbackParent(const std::shared_ptr<FilamentMaterialInstanceSource> &parent);

	filament::MaterialInstance *materialInstance();

	FilamentEngineObject<filament::MaterialInstance> createNewMaterialInstance() const override;
	void applyOntoExistingMaterialInstance(filament::MaterialInstance  *instance) const override;

	Variant getParam(const StringName &name) override;
	void setParam(const StringName &name, const Variant &value) override;

protected:
	void doClean() override;
	void controlledObjectAboutToInvalidate(FilamentControlledObjectReferenceBase *linkedViaReference) override;

private:
	void resetMaterialInstance();

	FilamentControlledObjectReference<FilamentMaterialInstanceSource> m_parent;
	FilamentControlledObjectReference<FilamentMaterialInstanceSource> m_fallbackParent;
	bool m_createdFromFallback;
	FilamentMaterialPropertyBundle m_properties;
	FilamentEngineObject<filament::MaterialInstance> m_material;
};

#endif


