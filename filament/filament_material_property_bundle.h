#ifndef FILAMENT_FILAMENT_MATERIAL_PROPERTY_BUNDLE_H
#define FILAMENT_FILAMENT_MATERIAL_PROPERTY_BUNDLE_H

#include "core/variant/variant.h"

namespace filament {
	class MaterialInstance;
}

class FilamentMaterialPropertyBundle {
public:
	FilamentMaterialPropertyBundle();
	~FilamentMaterialPropertyBundle();

	FilamentMaterialPropertyBundle(const FilamentMaterialPropertyBundle &other) = delete;
	FilamentMaterialPropertyBundle &operator =(const FilamentMaterialPropertyBundle &other) = delete;

	FilamentMaterialPropertyBundle(FilamentMaterialPropertyBundle &&other) noexcept;
	FilamentMaterialPropertyBundle &operator =(FilamentMaterialPropertyBundle &&other) noexcept;

	void apply(filament::MaterialInstance *materialInstance) const;

	Variant getParam(const StringName &name) const;
	void setParam(const StringName &name, const Variant &value);

private:
	Dictionary m_params;
};

#endif
