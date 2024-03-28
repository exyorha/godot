#ifndef FILAMENT_FILAMENT_MESH_H
#define FILAMENT_FILAMENT_MESH_H

#include "filament/filament_material_object.h"
#include "filament/filament_renderable_base.h"
#include "filament/filament_mesh_surface.h"
#include "servers/rendering_server.h"

#include <filament/RenderableManager.h>

#include <vector>

class FilamentMesh final : public FilamentRenderableBase {
public:
	FilamentMesh();
	~FilamentMesh();

	void constructInstance(const utils::Entity &entity, filament::SkinningBuffer *skinWith) override;
	filament::RenderableManager::Builder buildSingleInstanceOfMesh(filament::SkinningBuffer *skinWith) const;

	void clear();

	inline void reserveSurfaces(size_t surfaceCount) {
		m_surfaces.reserve(surfaceCount);
	}

	inline size_t surfaceCount() const {
		return m_surfaces.size();
	}

	inline size_t blendShapeCount() const {
		return m_blendShapeCount;
	}

	inline void setBlendShapeCount(size_t blendShapeCount) {
		m_blendShapeCount = blendShapeCount;
	}

	void addSurface(const RenderingServer::SurfaceData &data);

	void setSurfaceMaterial(size_t index, const std::shared_ptr<FilamentMaterialObject> &material);

private:
	std::vector<FilamentMeshSurface> m_surfaces;
	size_t m_blendShapeCount;
};

#endif
