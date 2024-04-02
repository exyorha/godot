#ifndef FILAMENT_CANVAS_ELEMENT_TRIANGLE_ARRAY_H
#define FILAMENT_CANVAS_ELEMENT_TRIANGLE_ARRAY_H

#include <core/templates/vector.h>
#include <core/math/vector2.h>

#include "filament/filament_engine_object.h"
#include "filament/filament_canvas_element.h"

struct Color;

class FilamentCanvasElementTriangleArray final : public FilamentCanvasElement {
public:
	FilamentCanvasElementTriangleArray(
		FilamentControlledObjectReferenceOwner *owner, RID texture,
		const Vector<int> & p_indices,
		const Vector<Point2> & p_points,
		const Vector<Color> & p_colors,
		const Vector<Point2> & p_uvs,
		const Vector<int> & p_bones,
		const Vector<float> & p_weights
	);

	~FilamentCanvasElementTriangleArray();

	Type type() const override;

	void build(filament::RenderableManager::Builder &builder, size_t index) override;

private:
	FilamentEngineObject<filament::IndexBuffer> m_indexBuffer;
	FilamentEngineObject<filament::VertexBuffer> m_vertexBuffer;
};

#endif
