#include "filament/filament_mesh.h"
#include "filament/filament_rendering_server_backend.h"

#include <filament/RenderableManager.h>
#include <filament/SkinningBuffer.h>

#include <utils/Entity.h>

#include <cstdio>

FilamentMesh::FilamentMesh() : m_blendShapeCount(0) {

}

FilamentMesh::~FilamentMesh() = default;

void FilamentMesh::clear() {
	objectAboutToInvalidate();

	m_surfaces.clear();
}

void FilamentMesh::addSurface(const RenderingServer::SurfaceData &data) {
	objectAboutToInvalidate();

	m_surfaces.emplace_back(data);
}

void FilamentMesh::constructInstance(const utils::Entity &entity, filament::SkinningBuffer *skinWith) {
	auto builder = buildSingleInstanceOfMesh(skinWith);

	auto result = builder.build(*FilamentRenderingServerBackend::filamentEngine(), entity);
	if(result != decltype(result)::Success) {
		throw std::runtime_error("FilamentMesh::constructInstance: failed to construct the instance");
	}
}


filament::RenderableManager::Builder FilamentMesh::buildSingleInstanceOfMesh(filament::SkinningBuffer *skinWith) const {
	filament::RenderableManager::Builder builder(m_surfaces.size());

	filament::Box compositeBoundingBox;

	for(size_t index = 0, count = m_surfaces.size(); index < count; index++) {
		const auto &surface = m_surfaces[index];

		surface.build(builder, index);

		compositeBoundingBox.unionSelf(surface.boundingBox());
	}

	builder.boundingBox(compositeBoundingBox);

	if(skinWith) {
		builder.enableSkinningBuffers();
		builder.skinning(skinWith, skinWith->getBoneCount(), 0);
	}

	return builder;
}


void FilamentMesh::setSurfaceMaterial(size_t index, const std::shared_ptr<FilamentMaterialObject> &material) {
	if(index >= m_surfaces.size()) {
		fprintf(stderr, "FilamentMesh::setSurfaceMaterial: surface index is out of range\n");
		return;
	}

	m_surfaces[index].setMaterial(material);
}
