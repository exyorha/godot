#ifndef FILAMENT_RENDERER_COMPOSITOR_H
#define FILAMENT_RENDERER_COMPOSITOR_H

#include "servers/rendering/renderer_compositor.h"
#include "filament_renderer_utilities.h"
#include "filament_renderer_light_storage.h"
#include "filament_renderer_material_storage.h"
#include "filament_renderer_mesh_storage.h"
#include "filament_renderer_particles_storage.h"
#include "filament_renderer_texture_storage.h"
#include "filament_renderer_gi.h"
#include "filament_renderer_fog.h"
#include "filament_renderer_canvas_render.h"
#include "filament_renderer_scene_render.h"

class FilamentRendererCompositor final : public RendererCompositor {
private:
	FilamentRendererCompositor();
	~FilamentRendererCompositor() override;

public:
	static void set_default();
	static void clear_default();

	RendererUtilities *get_utilities() override;
	RendererLightStorage *get_light_storage() override;
	RendererMaterialStorage *get_material_storage() override;
	RendererMeshStorage *get_mesh_storage() override;
	RendererParticlesStorage *get_particles_storage() override;
	RendererTextureStorage *get_texture_storage() override;
	RendererGI *get_gi() override;
	RendererFog *get_fog() override;
	RendererCanvasRender *get_canvas() override;
	RendererSceneRender *get_scene() override;

	void set_boot_image(const Ref<Image> &p_image, const Color &p_color, bool p_scale, bool p_use_filter) override;

	void initialize() override;
	void begin_frame(double frame_step) override;

	void prepare_for_blitting_render_targets() override;
	void blit_render_targets_to_screen(DisplayServer::WindowID p_screen, const BlitToScreen *p_render_targets, int p_amount) override;

	void end_frame(bool p_swap_buffers) override;
	void finalize() override;
	uint64_t get_frame_number() const override;
	double get_frame_delta_time() const override;
	double get_total_time() const override;

private:
	static RendererCompositor *create_func();
	FilamentRendererUtilities m_utilities;
	FilamentRendererLightStorage m_light;
	FilamentRendererMaterialStorage m_material;
	FilamentRendererMeshStorage m_mesh;
	FilamentRendererParticlesStorage m_particles;
	FilamentRendererTextureStorage m_texture;
	FilamentRendererGI m_gi;
	FilamentRendererFog m_fog;
	FilamentRendererCanvasRender m_canvas;
	FilamentRendererSceneRender m_scene;
};

#endif
