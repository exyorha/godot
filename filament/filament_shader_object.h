#ifndef FILAMENT_SHADER_OBJECT_H
#define FILAMENT_SHADER_OBJECT_H

#include "filament/filament_material_instance_source.h"
#include "filament/filament_material_property_bundle.h"

namespace filament {
	class Material;
	class MaterialInstance;
}

class String;

class FilamentShaderObject final : public FilamentMaterialInstanceSource {
public:
	FilamentShaderObject();
	~FilamentShaderObject() override;

	void createBuiltin(const unsigned char *filamentMaterialData, size_t size);

	inline void createFromPackage(const Vector<uint8_t> & p_package) {
		createBuiltin(p_package.ptr(), p_package.size());
	}

	FilamentEngineObject<filament::MaterialInstance> createNewMaterialInstance() const override;
	void applyOntoExistingMaterialInstance(filament::MaterialInstance  *instance) const override;

	Variant getParam(const StringName &name) override;
	void setParam(const StringName &name, const Variant &value) override;

	void getShaderParameterList(List<PropertyInfo> &parameterList);

private:
	FilamentEngineObject<filament::Material> m_material;
	FilamentMaterialPropertyBundle m_properties;
};

#endif

