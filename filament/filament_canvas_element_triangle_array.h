#ifndef FILAMENT_CANVAS_ELEMENT_TRIANGLE_ARRAY_H
#define FILAMENT_CANVAS_ELEMENT_TRIANGLE_ARRAY_H

#include <core/templates/vector.h>
#include <core/math/vector2.h>

#include <filament/RenderableManager.h>

#include "filament/filament_engine_object.h"

struct Color;

class FilamentCanvasElementTriangleArray {
public:
	FilamentCanvasElementTriangleArray(
		const Vector<int> & p_indices,
		const Vector<Point2> & p_points,
		const Vector<Color> & p_colors,
		const Vector<Point2> & p_uvs,
		const Vector<int> & p_bones,
		const Vector<float> & p_weights
	);

	~FilamentCanvasElementTriangleArray();

	FilamentCanvasElementTriangleArray(const FilamentCanvasElementTriangleArray &other) = delete;
	FilamentCanvasElementTriangleArray &operator =(const FilamentCanvasElementTriangleArray &other) = delete;

	FilamentCanvasElementTriangleArray(FilamentCanvasElementTriangleArray &&other) noexcept;
	FilamentCanvasElementTriangleArray &operator =(FilamentCanvasElementTriangleArray &&other) noexcept;

	void build(filament::RenderableManager::Builder &builder, size_t index);

private:
	FilamentEngineObject<filament::IndexBuffer> m_indexBuffer;
	FilamentEngineObject<filament::VertexBuffer> m_vertexBuffer;
};

#endif
