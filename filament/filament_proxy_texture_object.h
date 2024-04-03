#ifndef FILAMENT_FILAMENT_PROXY_TEXTURE_OBJECT_H
#define FILAMENT_FILAMENT_PROXY_TEXTURE_OBJECT_H

#include "filament/filament_texture_reference_object.h"
#include "filament/filament_controlled_object_reference.h"

class FilamentTextureObject;

class FilamentProxyTextureObject final : public FilamentTextureReferenceObject {
public:
	FilamentProxyTextureObject();
	~FilamentProxyTextureObject() override;

	filament::Texture *texture() override;

	inline std::shared_ptr<FilamentTextureReferenceObject> target() const {
		return m_target;
	}

	void setTarget(const std::shared_ptr<FilamentTextureReferenceObject> &target);

protected:
	void controlledObjectAboutToInvalidate(FilamentControlledObjectReferenceBase *linkedViaReference) override;

private:
	FilamentControlledObjectReference<FilamentTextureReferenceObject> m_target;
};

#endif
