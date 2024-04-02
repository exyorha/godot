#include "filament/filament_canvas_element_triangle_array.h"
#include "filament/filament_rendering_server_backend.h"
#include "filament/filament_upload_helpers.h"

#include <filament/IndexBuffer.h>
#include <filament/MaterialEnums.h>
#include <filament/VertexBuffer.h>

FilamentCanvasElementTriangleArray::FilamentCanvasElementTriangleArray(
	FilamentControlledObjectReferenceOwner *owner, RID texture,
	const Vector<int> & p_indices,
	const Vector<Point2> & p_points,
	const Vector<Color> & p_colors,
	const Vector<Point2> & p_uvs,
	const Vector<int> & p_bones,
	const Vector<float> & p_weights
) : FilamentCanvasElement(owner, texture) {

	Vector<uint16_t> truncatedBones;
	if(!p_bones.size()) {
		truncatedBones.resize(p_bones.size());
		auto outData = truncatedBones.ptrw();
		for(size_t index = 0, size = p_bones.size(); index < size; index++) {
			outData[index] = p_bones[index];
		}
	}

	auto engine = FilamentRenderingServerBackend::filamentEngine();

	if(!p_indices.is_empty()) {
		m_indexBuffer =
			FilamentEngineObject<filament::IndexBuffer>(
				filament::IndexBuffer::Builder().bufferType(filament::IndexBuffer::IndexType::UINT).indexCount(p_indices.size()).build(*engine)
			);

		if(!m_indexBuffer) {
			throw std::bad_alloc();
		}

		m_indexBuffer->setBuffer(*engine, makeFilamentDescriptor(p_indices));
	}

	if(!p_points.is_empty()) {
		size_t numberOfVertexBuffers = 1;

		if(!p_colors.is_empty()) {
			numberOfVertexBuffers++;
		}

		if(!p_uvs.is_empty()) {
			numberOfVertexBuffers++;
		}

		if(!truncatedBones.is_empty()) {
			numberOfVertexBuffers++;
		}

		if(!p_weights.is_empty()) {
			numberOfVertexBuffers++;
		}

		filament::VertexBuffer::Builder builder;
		builder.vertexCount(p_points.size());
		builder.bufferCount(numberOfVertexBuffers);

		// 64-bit float build will need a blit of the points into a 32-bit float buffer
		static_assert(std::is_same_v<decltype(Point2::x), float>, "only 32-bit float bits are supported for now");
		builder.attribute(filament::VertexAttribute::POSITION, 0, filament::VertexBuffer::AttributeType::FLOAT2);
		size_t nextBuffer = 1;

		if(!p_colors.is_empty()) {
			static_assert(std::is_same_v<decltype(Color::r), float>, "Color is supposed to always use 32-bit float");
			builder.attribute(filament::VertexAttribute::COLOR, nextBuffer, filament::VertexBuffer::AttributeType::FLOAT4);
			nextBuffer++;
		}

		if(!p_uvs.is_empty()) {
			// Same as for the positions: 64-bit float build will require a blit into a 32-bit buffer
			builder.attribute(filament::VertexAttribute::UV0, nextBuffer, filament::VertexBuffer::AttributeType::FLOAT2);
			nextBuffer++;
		}

		if(!truncatedBones.is_empty()) {
			builder.attribute(filament::VertexAttribute::BONE_INDICES, nextBuffer, filament::VertexBuffer::AttributeType::USHORT4);
			builder.normalized(filament::VertexAttribute::BONE_INDICES, false);
			nextBuffer++;
		}

		if(!p_weights.is_empty()) {
			builder.attribute(filament::VertexAttribute::BONE_WEIGHTS, nextBuffer, filament::VertexBuffer::AttributeType::FLOAT4);
			nextBuffer++;
		}

		m_vertexBuffer =
			FilamentEngineObject<filament::VertexBuffer>(
				builder.build(*engine)
			);

		if(!m_vertexBuffer) {
			throw std::bad_alloc();
		}

		m_vertexBuffer->setBufferAt(*engine, 0, makeFilamentDescriptor(p_points));

		nextBuffer = 1;
		if(!p_colors.is_empty()) {
			m_vertexBuffer->setBufferAt(*engine, nextBuffer, makeFilamentDescriptor(p_colors));
			nextBuffer++;
		}

		if(!p_uvs.is_empty()) {
			m_vertexBuffer->setBufferAt(*engine, nextBuffer, makeFilamentDescriptor(p_uvs));
			nextBuffer++;
		}

		if(!truncatedBones.is_empty()) {
			m_vertexBuffer->setBufferAt(*engine, nextBuffer, makeFilamentDescriptor(truncatedBones));
			nextBuffer++;
		}

		if(!p_weights.is_empty()) {
			m_vertexBuffer->setBufferAt(*engine, nextBuffer, makeFilamentDescriptor(p_weights));
			nextBuffer++;
		}
	}
}

FilamentCanvasElementTriangleArray::~FilamentCanvasElementTriangleArray() = default;

auto FilamentCanvasElementTriangleArray::type() const -> Type {
	return Type::TriangleArray;
}

void FilamentCanvasElementTriangleArray::build(filament::RenderableManager::Builder &builder, size_t index) {
	builder.geometry(index, filament::RenderableManager::PrimitiveType::TRIANGLES, m_vertexBuffer.get(), m_indexBuffer.get());
	builder.material(index, material());
}
