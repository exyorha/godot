#ifndef FILAMENT_FILAMENT_TEXTURE_REFERENCE_OBJECT_H
#define FILAMENT_FILAMENT_TEXTURE_REFERENCE_OBJECT_H

#include "filament/filament_object.h"

namespace filament {
	class Texture;
}

class FilamentTextureReferenceObject : public FilamentObject {
protected:
	FilamentTextureReferenceObject();

public:
	~FilamentTextureReferenceObject() override;

	virtual filament::Texture *texture() = 0;
};

#endif
