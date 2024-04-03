#include "filament/filament_proxy_texture_object.h"

FilamentProxyTextureObject::FilamentProxyTextureObject() : m_target(this) {

}

FilamentProxyTextureObject::~FilamentProxyTextureObject() = default;

filament::Texture *FilamentProxyTextureObject::texture() {
	auto texture = target();

	if(texture) {
		return texture->texture();
	} else {
		return nullptr;
	}
}

void FilamentProxyTextureObject::setTarget(const std::shared_ptr<FilamentTextureReferenceObject> &target) {
	objectAboutToInvalidate();

	m_target = target;
}

void FilamentProxyTextureObject::controlledObjectAboutToInvalidate(FilamentControlledObjectReferenceBase *linkedViaReference) {
	FilamentTextureReferenceObject::controlledObjectAboutToInvalidate(linkedViaReference);

	if(linkedViaReference == &m_target) {
		objectAboutToInvalidate();
	}
}
