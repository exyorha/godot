#ifndef FILAMENT_RENDERABLE_BASE_H
#define FILAMENT_RENDERABLE_BASE_H

#include "filament/filament_object.h"

namespace utils {
	class Entity;
}

namespace filament {
	class SkinningBuffer;
}

class FilamentRenderableBase : public FilamentObject {
protected:
	FilamentRenderableBase();

public:
	~FilamentRenderableBase() override;

	virtual void constructInstance(const utils::Entity &entity, filament::SkinningBuffer *skinWith) = 0;
};

#endif
