#ifndef FILAMENT_LIGHT_OBJECT_H
#define FILAMENT_LIGHT_OBJECT_H

#include "filament/filament_renderable_base.h"

#include <filament/LightManager.h>

struct Color;

class FilamentLightObject final : public FilamentRenderableBase {
public:
	explicit FilamentLightObject(filament::LightManager::Type type);
	~FilamentLightObject();

	void constructInstance(const utils::Entity &entity, filament::SkinningBuffer *skinWith) override;

	void setColor(const Color &color);

private:
	filament::LightManager::Type m_type;

};

#endif

