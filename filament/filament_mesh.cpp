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

	m_surfaces.emplace_back(static_cast<FilamentControlledObjectReferenceOwner *>(this), data);
}

void FilamentMesh::constructInstance(const utils::Entity &entity, filament::SkinningBuffer *skinWith) {
	auto builder = buildSingleInstanceOfMesh(skinWith);

	if(builder) {

		auto result = builder->build(*FilamentRenderingServerBackend::filamentEngine(), entity);
		if(result != decltype(result)::Success) {
			throw std::runtime_error("FilamentMesh::constructInstance: failed to construct the instance");
		}
	}
}


std::unique_ptr<filament::RenderableManager::Builder> FilamentMesh::buildSingleInstanceOfMesh(filament::SkinningBuffer *skinWith) const {
	size_t nonEmptySurfaces = 0;
	for(const auto &surface: m_surfaces) {
		if(!surface.isEmpty()) {
			nonEmptySurfaces++;
		}
	}

	if(nonEmptySurfaces == 0) {
		return nullptr;
	}

	auto builder = std::make_unique<filament::RenderableManager::Builder>(m_surfaces.size());

	filament::Box compositeBoundingBox;

	size_t surfaceIndex = 0;
	for(const auto &surface: m_surfaces) {
		if(!surface.isEmpty()) {

			surface.build(*builder, surfaceIndex);

			surfaceIndex++;

			compositeBoundingBox.unionSelf(surface.boundingBox());
		}
	}

	builder->boundingBox(compositeBoundingBox);

	if(skinWith) {
		builder->enableSkinningBuffers();
		builder->skinning(skinWith, skinWith->getBoneCount(), 0);
	}

	return builder;
}


void FilamentMesh::setSurfaceMaterial(size_t index, const std::shared_ptr<FilamentMaterialObject> &material) {
	if(index >= m_surfaces.size()) {
		fprintf(stderr, "FilamentMesh::setSurfaceMaterial: surface index is out of range\n");
		return;
	}

	objectAboutToInvalidate();

	m_surfaces[index].setMaterial(material);
}

void FilamentMesh::controlledObjectAboutToInvalidate(FilamentControlledObjectReferenceBase *linkedViaReference) {
	FilamentRenderableBase::controlledObjectAboutToInvalidate(linkedViaReference);

	objectAboutToInvalidate();
}
