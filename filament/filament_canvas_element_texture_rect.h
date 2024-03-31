#ifndef FILAMENT_CANVAS_ELEMENT_TEXTURE_RECT_H
#define FILAMENT_CANVAS_ELEMENT_TEXTURE_RECT_H

#include <filament/RenderableManager.h>

#include "filament/filament_engine_object.h"

#include "core/templates/vector.h"

struct Rect2;
struct Color;

class FilamentCanvasElementTextureRect {
public:
	FilamentCanvasElementTextureRect();
	~FilamentCanvasElementTextureRect();

	FilamentCanvasElementTextureRect(const FilamentCanvasElementTextureRect &other) = delete;
	FilamentCanvasElementTextureRect &operator =(const FilamentCanvasElementTextureRect &other) = delete;

	void build(filament::RenderableManager::Builder &builder, size_t index);

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

private:
	struct ElementVertex {
		filament::math::float2 position;
		filament::math::float2 uv;
		filament::math::float4 modulatingColor;
	};

	void buildGeometry();

	template<typename T>
	static Vector<T> generateIndexBufferForVertexCount(size_t vertexCount);

	FilamentEngineObject<filament::IndexBuffer> m_indexBuffer;
	FilamentEngineObject<filament::VertexBuffer> m_vertexBuffer;
	Vector<ElementVertex> m_vertices;
	bool m_dirty;
};

#endif

