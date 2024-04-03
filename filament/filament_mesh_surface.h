#ifndef FILAMENT_FILAMENT_MESH_SURFACE_H
#define FILAMENT_FILAMENT_MESH_SURFACE_H

#include "servers/rendering_server.h"

#include "filament/filament_engine_object.h"
#include "filament/filament_controlled_object_reference.h"

#include <filament/RenderableManager.h>

class FilamentMaterialObject;

class FilamentMeshSurface {
public:
	FilamentMeshSurface(FilamentControlledObjectReferenceOwner *owner, const RenderingServer::SurfaceData &data);
	~FilamentMeshSurface();

	FilamentMeshSurface(const FilamentMeshSurface &other) = delete;
	FilamentMeshSurface &operator =(const FilamentMeshSurface &other) = delete;

	FilamentMeshSurface(FilamentMeshSurface &&other) noexcept;
	FilamentMeshSurface &operator =(FilamentMeshSurface &&other) noexcept;

	inline bool isEmpty() const {
		return !m_vertexBuffer || !m_indexBuffer;
	}

	void build(filament::RenderableManager::Builder &builder, size_t primitiveIndexInBuilder) const;

	void setMaterial(const std::shared_ptr<FilamentMaterialObject> &material);

	inline const filament::Box &boundingBox() const {
		return m_boundingBox;
	}

private:
	static filament::RenderableManager::PrimitiveType mapPrimitiveType(RenderingServer::PrimitiveType in);

	FilamentEngineObject<filament::VertexBuffer> m_vertexBuffer;
	FilamentEngineObject<filament::IndexBuffer> m_indexBuffer;
	filament::RenderableManager::PrimitiveType m_primitiveType;
	filament::Box m_boundingBox;
	std::shared_ptr<FilamentMaterialObject> m_material;
	mutable FilamentControlledObjectReference<FilamentMaterialObject> m_constructedFromMaterial;
};

#endif
