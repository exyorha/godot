#include "filament/filament_proxy_texture_object.h"
#include "filament/filament_texture_object.h"

FilamentProxyTextureObject::FilamentProxyTextureObject() = default;

FilamentProxyTextureObject::~FilamentProxyTextureObject() = default;

filament::Texture *FilamentProxyTextureObject::texture() const {
	auto texture = target();

	if(texture) {
		return texture->texture();
	} else {
		return nullptr;
	}
}

std::shared_ptr<FilamentTextureObject> FilamentProxyTextureObject::target() const {
	return m_target.lock();
}

void FilamentProxyTextureObject::setTarget(const std::shared_ptr<FilamentTextureObject> &target) {
	m_target = target;
}
