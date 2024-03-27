
#include "filament/filament_mesh_surface.h"
#include "filament/filament_rendering_server.h"
#include "filament/filament_rendering_server_backend.h"
#include "filament/filament_vertex_format.h"
#include "filament/filament_material_object.h"

#include "servers/rendering_server.h"

#include <filament/IndexBuffer.h>
#include <filament/VertexBuffer.h>

FilamentMeshSurface::FilamentMeshSurface(const RenderingServer::SurfaceData &data) :
	m_primitiveType(mapPrimitiveType(data.primitive)) {

	/*
	 * TODO: maybe implement LODs? They currently are not in Filament at all
	 * and we don't use them in nierdot at all, so it's not a priority.
	 */

	auto &engine = *FilamentRenderingServerBackend::filamentEngine();

	if(data.format & RenderingServer::ARRAY_FORMAT_INDEX) {

		auto indexType = filament::IndexBuffer::IndexType::UINT;

		if(data.vertex_count > 0 && data.vertex_count <= 65536) {
			indexType = filament::IndexBuffer::IndexType::USHORT;
		}

		m_indexBuffer = FilamentEngineObject<filament::IndexBuffer>(
			filament::IndexBuffer::Builder().bufferType(indexType).indexCount(data.index_count).build(engine)
		);
		if(!m_indexBuffer) {
			throw std::bad_alloc();
		}

		m_indexBuffer->setBuffer(engine, makeBufferDescriptor(data.index_data));
	}

	FilamentVertexFormat vertexFormat(data.format, data.vertex_count);

	if(!vertexFormat.empty()) {
		filament::VertexBuffer::Builder builder;
		builder.bufferCount(vertexFormat.buffers().size());
		builder.vertexCount(data.vertex_count);

		for(const auto &attribute: vertexFormat.attributes()) {
			builder.attribute(attribute.attribute, attribute.bufferIndex, attribute.attributeType, attribute.offset, attribute.stride);
			builder.normalized(attribute.attribute, attribute.normalized);
		}

		m_vertexBuffer = FilamentEngineObject<filament::VertexBuffer>(builder.build(engine));
		if(!m_vertexBuffer) {
			throw std::bad_alloc();
		}

		if(vertexFormat.hasPositionDataBuffer() || vertexFormat.hasTangentDataBuffer()) {
			std::optional<Vector<uint8_t>> positionData, tangentData;

			vertexFormat.transcodeVertexDataForFilament(data.vertex_data, positionData, tangentData, data.aabb);

			if(positionData.has_value()) {
				m_vertexBuffer->setBufferAt(engine, vertexFormat.positionDataBuffer(), makeBufferDescriptor(*positionData));
			}

			if(tangentData.has_value()) {
				m_vertexBuffer->setBufferAt(engine, vertexFormat.tangentDataBuffer(), makeBufferDescriptor(*tangentData));
			}
		}


		if(vertexFormat.hasAttributeDataBuffer()) {
			m_vertexBuffer->setBufferAt(engine, vertexFormat.attributeDataBuffer(), makeBufferDescriptor(data.attribute_data));
		}

		if(vertexFormat.hasSkinDataBuffer()) {
			m_vertexBuffer->setBufferAt(engine, vertexFormat.skinDataBuffer(), makeBufferDescriptor(data.skin_data));
		}

	}

	m_boundingBox.center = filament::math::float3(data.aabb.position.x, data.aabb.position.y, data.aabb.position.z);
	m_boundingBox.halfExtent = filament::math::float3(data.aabb.size.x * 0.5f, data.aabb.size.y * 0.5f, data.aabb.size.z * 0.5f);

	m_material = FilamentRenderingServerBackend::get()->objects().resolve<FilamentMaterialObject>(data.material);
}

FilamentMeshSurface::~FilamentMeshSurface() = default;

FilamentMeshSurface::FilamentMeshSurface(FilamentMeshSurface &&other) noexcept = default;

FilamentMeshSurface &FilamentMeshSurface::operator =(FilamentMeshSurface &&other) noexcept = default;

filament::backend::BufferDescriptor FilamentMeshSurface::makeBufferDescriptor(const Vector<uint8_t> &data) {
	auto vectorCopy = std::make_unique<Vector<uint8_t>>(data);

	/*
	 * BufferDescriptor constructor is noexcept.
	 */
	auto rawVectorCopy = vectorCopy.release();
	return filament::backend::BufferDescriptor(rawVectorCopy->ptr(), rawVectorCopy->size(), nullptr, bufferDescriptorCallback, rawVectorCopy);
}

void FilamentMeshSurface::bufferDescriptorCallback(void* buffer, size_t size, void* user) {
	delete static_cast<Vector<uint8_t> *>(user);
}


filament::RenderableManager::PrimitiveType FilamentMeshSurface::mapPrimitiveType(RenderingServer::PrimitiveType in) {
	switch(in) {
		case RenderingServer::PRIMITIVE_POINTS:
			return filament::RenderableManager::PrimitiveType::POINTS;

		case RenderingServer::PRIMITIVE_LINES:
			return filament::RenderableManager::PrimitiveType::LINES;

		case RenderingServer::PRIMITIVE_LINE_STRIP:
			return filament::RenderableManager::PrimitiveType::LINE_STRIP;

		case RenderingServer::PRIMITIVE_TRIANGLES:
			return filament::RenderableManager::PrimitiveType::TRIANGLES;

		case RenderingServer::PRIMITIVE_TRIANGLE_STRIP:
			return filament::RenderableManager::PrimitiveType::TRIANGLE_STRIP;

		default:
			throw std::runtime_error("FilamentMeshSurface::mapPrimitiveType: unsupported primitive type");
	}
}

void FilamentMeshSurface::build(filament::RenderableManager::Builder &builder, size_t primitiveIndexInBuilder) const {
	builder.geometry(
		primitiveIndexInBuilder,
		m_primitiveType,
		m_vertexBuffer.get(),
		m_indexBuffer.get()
	);

	if(m_material) {
		auto instance = m_material->materialInstance();
		printf("!! FilamentMeshSurface::bild: setting surface %zu material to %p\n", primitiveIndexInBuilder, instance);
		builder.material(primitiveIndexInBuilder, instance);
	}
}

void FilamentMeshSurface::setMaterial(const std::shared_ptr<FilamentMaterialObject> &material) {
	m_material = material;
}

