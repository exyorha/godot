#ifndef FILAMENT_CANVAS_ITEM_MATERIAL_GROUP_H
#define FILAMENT_CANVAS_ITEM_MATERIAL_GROUP_H

#include <memory>
#include <vector>
#include <optional>

#include "core/templates/vector.h"
#include "core/math/vector2.h"
#include "core/templates/rid.h"

#include "filament/filament_canvas_element_triangle_array.h"
#include "filament/filament_canvas_element_texture_rect.h"
#include "filament/filament_controlled_object_reference.h"

#include <filament/RenderableManager.h>

struct Rect2;
struct Color;

class FilamentTextureReferenceObject;
class FilamentCanvasElementTriangleArray;
class FilamentMaterialObject;

class FilamentCanvasItemMaterialGroup {
public:
	FilamentCanvasItemMaterialGroup(FilamentControlledObjectReferenceOwner *owner, RID texture);
	~FilamentCanvasItemMaterialGroup();

	FilamentCanvasItemMaterialGroup(const FilamentCanvasItemMaterialGroup &other) = delete;
	FilamentCanvasItemMaterialGroup &operator =(const FilamentCanvasItemMaterialGroup &other) = delete;

	inline const RID &texture() const {
		return m_texture;
	}

	void addTriangleArray(
		const Vector<int> & p_indices,
		const Vector<Point2> & p_points,
		const Vector<Color> & p_colors,
		const Vector<Point2> & p_uvs,
		const Vector<int> & p_bones,
		const Vector<float> & p_weights
	);

	void addTextureRect(
		const Rect2 & p_rect,
		bool p_tile,
		const Color & p_modulate,
		bool p_transpose);

	void addTextureRectRegion(
		const Rect2 & p_rect,
		const Rect2 & p_src_rect,
		const Color & p_modulate,
		bool p_transpose,
		bool p_clip_uv);

	size_t numberOfPrimitives() const;
	size_t build(filament::RenderableManager::Builder &builder, size_t index);

private:
	RID m_texture;
	FilamentControlledObjectReference<FilamentMaterialObject> m_material;
	std::vector<FilamentCanvasElementTriangleArray> m_triangleArrays;
	std::optional<FilamentCanvasElementTextureRect> m_textureRect;
};

#endif
