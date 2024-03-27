#ifndef FILAMENT_FILAMENT_MESH_SURFACE_H
#define FILAMENT_FILAMENT_MESH_SURFACE_H

#include "servers/rendering_server.h"

#include "filament/filament_engine_object.h"

#include <filament/RenderableManager.h>

namespace filament::backend {
	class BufferDescriptor;
}

class FilamentMaterialObject;

class FilamentMeshSurface {
public:
	explicit FilamentMeshSurface(const RenderingServer::SurfaceData &data);
	~FilamentMeshSurface();

	FilamentMeshSurface(const FilamentMeshSurface &other) = delete;
	FilamentMeshSurface &operator =(const FilamentMeshSurface &other) = delete;

	FilamentMeshSurface(FilamentMeshSurface &&other) noexcept;
	FilamentMeshSurface &operator =(FilamentMeshSurface &&other) noexcept;

	void build(filament::RenderableManager::Builder &builder, size_t primitiveIndexInBuilder) const;

	void setMaterial(const std::shared_ptr<FilamentMaterialObject> &material);

	inline const filament::Box &boundingBox() const {
		return m_boundingBox;
	}

private:
	static filament::RenderableManager::PrimitiveType mapPrimitiveType(RenderingServer::PrimitiveType in);

	static filament::backend::BufferDescriptor makeBufferDescriptor(const Vector<uint8_t> &data);
	static void bufferDescriptorCallback(void* buffer, size_t size, void* user);

	FilamentEngineObject<filament::VertexBuffer> m_vertexBuffer;
	FilamentEngineObject<filament::IndexBuffer> m_indexBuffer;
	filament::RenderableManager::PrimitiveType m_primitiveType;
	filament::Box m_boundingBox;
	std::shared_ptr<FilamentMaterialObject> m_material;
};

#endif
