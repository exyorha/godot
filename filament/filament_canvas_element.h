#ifndef FILAMENT_CANVAS_ELEMENT_H
#define FILAMENT_CANVAS_ELEMENT_H

#include "core/templates/rid.h"

#include "filament/filament_controlled_object_reference.h"

#include <filament/RenderableManager.h>

#include <optional>

class FilamentTextureReferenceObject;
class FilamentMaterialObject;
struct Rect2i;

namespace filament {
	class MaterialInstance;
}

class FilamentCanvasElement {
protected:
	explicit FilamentCanvasElement(FilamentControlledObjectReferenceOwner *owner, RID texture);

public:
	virtual ~FilamentCanvasElement();

	FilamentCanvasElement(const FilamentCanvasElement &other) = delete;
	FilamentCanvasElement &operator =(const FilamentCanvasElement &other) = delete;

	enum class Type {
		TextureRect,
		TriangleArray
	};

	virtual Type type() const = 0;
	inline RID texture() const {
		return m_texture;
	}

	virtual void build(filament::RenderableManager::Builder &builder, size_t index) = 0;

	void setScissor(const std::optional<Rect2i> &scissor);

protected:
	filament::MaterialInstance *material() const;

private:
	RID m_texture;
	FilamentControlledObjectReference<FilamentMaterialObject> m_material;
};

#endif
