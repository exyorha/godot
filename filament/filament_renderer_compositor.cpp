#include "filament_renderer_compositor.h"
#include "servers/rendering/renderer_scene_render.h"

FilamentRendererCompositor::FilamentRendererCompositor() {
	printf("FilamentRendererCompositor(%p)::constructor\n", this);
}

FilamentRendererCompositor::~FilamentRendererCompositor() {
	printf("FilamentRendererCompositor(%p)::destructor\n", this);
}

void FilamentRendererCompositor::set_default() {
	_create_func = create_func;
}

void FilamentRendererCompositor::clear_default() {
	if(_create_func == create_func) {
		_create_func = nullptr;
	}
}

RendererUtilities *FilamentRendererCompositor::get_utilities() {
	return &m_utilities;
}

RendererLightStorage *FilamentRendererCompositor::get_light_storage() {
	return &m_light;
}

RendererMaterialStorage *FilamentRendererCompositor::get_material_storage() {
	return &m_material;
}

RendererMeshStorage *FilamentRendererCompositor::get_mesh_storage() {
	return &m_mesh;
}

RendererParticlesStorage *FilamentRendererCompositor::get_particles_storage() {
	return &m_particles;
}

RendererTextureStorage *FilamentRendererCompositor::get_texture_storage() {
	return &m_texture;
}

RendererGI *FilamentRendererCompositor::get_gi() {
	return &m_gi;
}

RendererFog *FilamentRendererCompositor::get_fog() {
	return &m_fog;
}

RendererCanvasRender *FilamentRendererCompositor::get_canvas() {
	return &m_canvas;
}

RendererSceneRender *FilamentRendererCompositor::get_scene() {
	return &m_scene;
}

RendererCompositor *FilamentRendererCompositor::create_func() {
	return memnew(FilamentRendererCompositor);
}
