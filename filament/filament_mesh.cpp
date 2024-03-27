#include "filament/filament_mesh.h"
#include "filament/filament_rendering_server_backend.h"

#include <filament/RenderableManager.h>

#include <utils/Entity.h>

#include <cstdio>

FilamentMesh::FilamentMesh() : m_blendShapeCount(0) {

}

FilamentMesh::~FilamentMesh() = default;

void FilamentMesh::clear() {
	m_surfaces.clear();
}

void FilamentMesh::addSurface(const RenderingServer::SurfaceData &data) {
	m_surfaces.emplace_back(data);
}

void FilamentMesh::constructInstance(const utils::Entity &entity) {
	auto builder = buildSingleInstanceOfMesh();

	auto result = builder.build(*FilamentRenderingServerBackend::filamentEngine(), entity);
	if(result != decltype(result)::Success) {
		throw std::runtime_error("FilamentMesh::constructInstance: failed to construct the instance");
	}
}


filament::RenderableManager::Builder FilamentMesh::buildSingleInstanceOfMesh() const {
	filament::RenderableManager::Builder builder(m_surfaces.size());

	filament::Box compositeBoundingBox;

	for(size_t index = 0, count = m_surfaces.size(); index < count; index++) {
		const auto &surface = m_surfaces[index];

		surface.build(builder, index);

		compositeBoundingBox.unionSelf(surface.boundingBox());
	}

	builder.boundingBox(compositeBoundingBox);

	builder.culling(false);

	return builder;
}


void FilamentMesh::setSurfaceMaterial(size_t index, const std::shared_ptr<FilamentMaterialObject> &material) {
	if(index >= m_surfaces.size()) {
		fprintf(stderr, "FilamentMesh::setSurfaceMaterial: surface index is out of range\n");
		return;
	}

	m_surfaces[index].setMaterial(material);
}
