#include "filament/filament_canvas_element_texture_rect.h"
#include "filament/filament_engine_object.h"
#include "filament/filament_rendering_server_backend.h"
#include "filament/filament_upload_helpers.h"

#include <filament/IndexBuffer.h>
#include <filament/VertexBuffer.h>
#include <filament/MaterialEnums.h>

FilamentCanvasElementTextureRect::FilamentCanvasElementTextureRect() : m_dirty(false) {

}

FilamentCanvasElementTextureRect::~FilamentCanvasElementTextureRect() = default;

void FilamentCanvasElementTextureRect::build(filament::RenderableManager::Builder &builder, size_t index) {
	if(m_dirty) {
		m_dirty = false;
		buildGeometry();
	}

	printf("FilamentCanvasElementTextureRect: build: %d vertices, vertex buffer %p, index buffer %p\n",
		   m_vertices.size(), m_vertexBuffer.get(), m_indexBuffer.get());

	if(m_vertices.is_empty()) {
		return;
	}

	builder.geometry(index, filament::RenderableManager::PrimitiveType::TRIANGLES, m_vertexBuffer.get(), m_indexBuffer.get());
}

void FilamentCanvasElementTextureRect::buildGeometry() {
	m_vertexBuffer = FilamentEngineObject<filament::VertexBuffer>();
	m_indexBuffer = FilamentEngineObject<filament::IndexBuffer>();

	if(m_vertices.is_empty()) {
		return;
	}

	auto engine = FilamentRenderingServerBackend::filamentEngine();

	filament::VertexBuffer::Builder builder;
	builder.bufferCount(1);
	builder.vertexCount(m_vertices.size());
	builder.attribute(filament::VertexAttribute::POSITION, 0, filament::VertexBuffer::AttributeType::FLOAT2,
					  offsetof(ElementVertex, position), sizeof(ElementVertex));
	builder.attribute(filament::VertexAttribute::UV0, 0, filament::VertexBuffer::AttributeType::FLOAT2,
					  offsetof(ElementVertex, uv), sizeof(ElementVertex));
	builder.attribute(filament::VertexAttribute::COLOR, 0, filament::VertexBuffer::AttributeType::FLOAT4,
					  offsetof(ElementVertex, modulatingColor), sizeof(ElementVertex));

	m_vertexBuffer = FilamentEngineObject<filament::VertexBuffer>(builder.build(*engine));
	if(!m_vertexBuffer) {
		throw std::runtime_error("failed to build FilamentCanvasElementTextureRect's vertex buffer");
	}
	m_vertexBuffer->setBufferAt(*engine, 0, makeFilamentDescriptor(m_vertices));

	if(m_vertices.size() < 65535) {
		/*
		 * We don't generate a 16 bit index buffer for exactly 65535 vertices
		 * to avoid possible primitive restart issues.
		 */

		auto indexBuffer = generateIndexBufferForVertexCount<uint16_t>(m_vertices.size());

		m_indexBuffer = FilamentEngineObject<filament::IndexBuffer>(
			filament::IndexBuffer::Builder().bufferType(filament::IndexBuffer::IndexType::USHORT)
				.indexCount(indexBuffer.size())
				.build(*engine));

		if(!m_indexBuffer) {
			throw std::runtime_error("failed to build FilamentCanvasElementTextureRect's index buffer");
		}

		m_indexBuffer->setBuffer(*engine, makeFilamentDescriptor(indexBuffer));

	} else {
		auto indexBuffer = generateIndexBufferForVertexCount<uint32_t>(m_vertices.size());

		m_indexBuffer = FilamentEngineObject<filament::IndexBuffer>(
			filament::IndexBuffer::Builder().bufferType(filament::IndexBuffer::IndexType::UINT)
				.indexCount(indexBuffer.size())
				.build(*engine));

		if(!m_indexBuffer) {
			throw std::runtime_error("failed to build FilamentCanvasElementTextureRect's index buffer");
		}

		m_indexBuffer->setBuffer(*engine, makeFilamentDescriptor(indexBuffer));

	}
}

template<typename T>
Vector<T> FilamentCanvasElementTextureRect::generateIndexBufferForVertexCount(size_t vertexCount) {

	size_t quads = vertexCount / 4;

	Vector<T> output;
	output.resize(quads * 6);

	auto ptr = output.ptrw();

	size_t base = 0;

	for(; quads != 0; quads--) {
		*ptr++ = base + 3;
		*ptr++ = base + 2;
		*ptr++ = base + 0;
		*ptr++ = base + 2;
		*ptr++ = base + 1;
		*ptr++ = base + 0;
		base += 4;
	}

	return output;
}

void FilamentCanvasElementTextureRect::addTextureRect(
	const Rect2 & p_rect,
	bool p_tile,
	const Color & p_modulate,
	bool p_transpose) {

	ElementVertex topLeft;
	ElementVertex topRight;
	ElementVertex bottomLeft;
	ElementVertex bottomRight;

	topLeft.position.x = p_rect.position.x;
	topLeft.position.y = p_rect.position.y;

	topRight.position.x = p_rect.position.x + p_rect.size.width;
	topRight.position.y = p_rect.position.y;

	bottomLeft.position.x = p_rect.position.x;
	bottomLeft.position.y = p_rect.position.y + p_rect.size.height;

	bottomRight.position.x = p_rect.position.x + p_rect.size.width;
	bottomRight.position.y = p_rect.position.y + p_rect.size.height;

	m_dirty = true;
	m_vertices.append(topLeft);
	m_vertices.append(topRight);
	m_vertices.append(bottomRight);
	m_vertices.append(bottomLeft);
}

void FilamentCanvasElementTextureRect::addTextureRectRegion(
	const Rect2 & p_rect,
	const Rect2 & p_src_rect,
	const Color & p_modulate,
	bool p_transpose,
	bool p_clip_uv) {

	ElementVertex topLeft;
	ElementVertex topRight;
	ElementVertex bottomLeft;
	ElementVertex bottomRight;

	topLeft.position.x = p_rect.position.x;
	topLeft.position.y = p_rect.position.y;

	topRight.position.x = p_rect.position.x + p_rect.size.width;
	topRight.position.y = p_rect.position.y;

	bottomLeft.position.x = p_rect.position.x;
	bottomLeft.position.y = p_rect.position.y + p_rect.size.height;

	bottomRight.position.x = p_rect.position.x + p_rect.size.width;
	bottomRight.position.y = p_rect.position.y + p_rect.size.height;

	m_dirty = true;
	m_vertices.append(topLeft);
	m_vertices.append(topRight);
	m_vertices.append(bottomRight);
	m_vertices.append(bottomLeft);

}
