#ifndef FILAMENT_FILAMENT_PROXY_TEXTURE_OBJECT_H
#define FILAMENT_FILAMENT_PROXY_TEXTURE_OBJECT_H

#include "filament/filament_texture_reference_object.h"

class FilamentTextureObject;

class FilamentProxyTextureObject final : public FilamentTextureReferenceObject {
public:
	FilamentProxyTextureObject();
	~FilamentProxyTextureObject() override;

	filament::Texture *texture() const override;

	std::shared_ptr<FilamentTextureObject> target() const;
	void setTarget(const std::shared_ptr<FilamentTextureObject> &target);

private:
	std::weak_ptr<FilamentTextureObject> m_target;
};

#endif
