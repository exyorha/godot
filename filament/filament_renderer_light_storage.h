#ifndef FILAMENT_RENDERER_LIGHT_STORAGE_H
#define FILAMENT_RENDERER_LIGHT_STORAGE_H

#include "servers/rendering/storage/light_storage.h"

class FilamentRendererLightStorage final : public RendererLightStorage {
public:
	FilamentRendererLightStorage();
	~FilamentRendererLightStorage() override;

	RID directional_light_allocate() override;
	void directional_light_initialize(RID p_rid) override;

	RID omni_light_allocate() override;
	void omni_light_initialize(RID p_rid) override;

	RID spot_light_allocate() override;
	void spot_light_initialize(RID p_rid) override;

	void light_free(RID p_rid) override;

	void light_set_color(RID p_light, const Color &p_color) override;
	void light_set_param(RID p_light, RS::LightParam p_param, float p_value) override;
	void light_set_shadow(RID p_light, bool p_enabled) override;
	void light_set_projector(RID p_light, RID p_texture) override;
	void light_set_negative(RID p_light, bool p_enable) override;
	void light_set_cull_mask(RID p_light, uint32_t p_mask) override;
	void light_set_distance_fade(RID p_light, bool p_enabled, float p_begin, float p_shadow, float p_length) override;
	void light_set_reverse_cull_face_mode(RID p_light, bool p_enabled) override;
	void light_set_bake_mode(RID p_light, RS::LightBakeMode p_bake_mode) override;
	void light_set_max_sdfgi_cascade(RID p_light, uint32_t p_cascade) override;

	void light_omni_set_shadow_mode(RID p_light, RS::LightOmniShadowMode p_mode) override;

	void light_directional_set_shadow_mode(RID p_light, RS::LightDirectionalShadowMode p_mode) override;
	void light_directional_set_blend_splits(RID p_light, bool p_enable) override;
	bool light_directional_get_blend_splits(RID p_light) const override;
	void light_directional_set_sky_mode(RID p_light, RS::LightDirectionalSkyMode p_mode) override;
	RS::LightDirectionalSkyMode light_directional_get_sky_mode(RID p_light) const override;

	RS::LightDirectionalShadowMode light_directional_get_shadow_mode(RID p_light) override;
	RS::LightOmniShadowMode light_omni_get_shadow_mode(RID p_light) override;

	bool light_has_shadow(RID p_light) const override;

	bool light_has_projector(RID p_light) const override;

	RS::LightType light_get_type(RID p_light) const override;
	AABB light_get_aabb(RID p_light) const override;
	float light_get_param(RID p_light, RS::LightParam p_param) override;
	Color light_get_color(RID p_light) override;
	bool light_get_reverse_cull_face_mode(RID p_light) const override;
	RS::LightBakeMode light_get_bake_mode(RID p_light) override;
	uint32_t light_get_max_sdfgi_cascade(RID p_light) override;
	uint64_t light_get_version(RID p_light) const override;
	uint32_t light_get_cull_mask(RID p_light) const override;

	/* LIGHT INSTANCE API */

	RID light_instance_create(RID p_light) override;
	void light_instance_free(RID p_light_instance) override;
	void light_instance_set_transform(RID p_light_instance, const Transform3D &p_transform) override;
	void light_instance_set_aabb(RID p_light_instance, const AABB &p_aabb) override;
	void light_instance_set_shadow_transform(RID p_light_instance, const Projection &p_projection, const Transform3D &p_transform, float p_far, float p_split, int p_pass, float p_shadow_texel_size, float p_bias_scale, float p_range_begin, const Vector2 &p_uv_scale) override;
	void light_instance_mark_visible(RID p_light_instance) override;

	/* PROBE API */

	RID reflection_probe_allocate() override;
	void reflection_probe_initialize(RID p_rid) override;
	void reflection_probe_free(RID p_rid) override;

	void reflection_probe_set_update_mode(RID p_probe, RS::ReflectionProbeUpdateMode p_mode) override;
	void reflection_probe_set_resolution(RID p_probe, int p_resolution) override;
	void reflection_probe_set_intensity(RID p_probe, float p_intensity) override;
	void reflection_probe_set_ambient_mode(RID p_probe, RS::ReflectionProbeAmbientMode p_mode) override;
	void reflection_probe_set_ambient_color(RID p_probe, const Color &p_color) override;
	void reflection_probe_set_ambient_energy(RID p_probe, float p_energy) override;
	void reflection_probe_set_max_distance(RID p_probe, float p_distance) override;
	void reflection_probe_set_size(RID p_probe, const Vector3 &p_size) override;
	void reflection_probe_set_origin_offset(RID p_probe, const Vector3 &p_offset) override;
	void reflection_probe_set_as_interior(RID p_probe, bool p_enable) override;
	void reflection_probe_set_enable_box_projection(RID p_probe, bool p_enable) override;
	void reflection_probe_set_enable_shadows(RID p_probe, bool p_enable) override;
	void reflection_probe_set_cull_mask(RID p_probe, uint32_t p_layers) override;
	void reflection_probe_set_mesh_lod_threshold(RID p_probe, float p_ratio) override;

	AABB reflection_probe_get_aabb(RID p_probe) const override;
	RS::ReflectionProbeUpdateMode reflection_probe_get_update_mode(RID p_probe) const override;
	uint32_t reflection_probe_get_cull_mask(RID p_probe) const override;
	Vector3 reflection_probe_get_size(RID p_probe) const override;
	Vector3 reflection_probe_get_origin_offset(RID p_probe) const override;
	float reflection_probe_get_origin_max_distance(RID p_probe) const override;
	bool reflection_probe_renders_shadows(RID p_probe) const override;
	float reflection_probe_get_mesh_lod_threshold(RID p_probe) const override;

	/* REFLECTION ATLAS */

	RID reflection_atlas_create() override;
	void reflection_atlas_free(RID p_ref_atlas) override;
	void reflection_atlas_set_size(RID p_ref_atlas, int p_reflection_size, int p_reflection_count) override;
	int reflection_atlas_get_size(RID p_ref_atlas) const override;

	/* REFLECTION PROBE INSTANCE */

	RID reflection_probe_instance_create(RID p_probe) override;
	void reflection_probe_instance_free(RID p_instance) override;
	void reflection_probe_instance_set_transform(RID p_instance, const Transform3D &p_transform) override;
	void reflection_probe_release_atlas_index(RID p_instance) override;
	bool reflection_probe_instance_needs_redraw(RID p_instance) override;
	bool reflection_probe_instance_has_reflection(RID p_instance) override;
	bool reflection_probe_instance_begin_render(RID p_instance, RID p_reflection_atlas) override;
	Ref<RenderSceneBuffers> reflection_probe_atlas_get_render_buffers(RID p_reflection_atlas) override;
	bool reflection_probe_instance_postprocess_step(RID p_instance) override;

	/* LIGHTMAP  */

	RID lightmap_allocate() override;
	void lightmap_initialize(RID p_rid) override;
	void lightmap_free(RID p_rid) override;

	void lightmap_set_textures(RID p_lightmap, RID p_light, bool p_uses_spherical_haromics) override;
	void lightmap_set_probe_bounds(RID p_lightmap, const AABB &p_bounds) override;
	void lightmap_set_probe_interior(RID p_lightmap, bool p_interior) override;
	void lightmap_set_probe_capture_data(RID p_lightmap, const PackedVector3Array &p_points, const PackedColorArray &p_point_sh, const PackedInt32Array &p_tetrahedra, const PackedInt32Array &p_bsp_tree) override;
	void lightmap_set_baked_exposure_normalization(RID p_lightmap, float p_exposure) override;
	PackedVector3Array lightmap_get_probe_capture_points(RID p_lightmap) const override;
	PackedColorArray lightmap_get_probe_capture_sh(RID p_lightmap) const override;
	PackedInt32Array lightmap_get_probe_capture_tetrahedra(RID p_lightmap) const override;
	PackedInt32Array lightmap_get_probe_capture_bsp_tree(RID p_lightmap) const override;
	AABB lightmap_get_aabb(RID p_lightmap) const override;
	void lightmap_tap_sh_light(RID p_lightmap, const Vector3 &p_point, Color *r_sh) override;
	bool lightmap_is_interior(RID p_lightmap) const override;
	void lightmap_set_probe_capture_update_speed(float p_speed) override;
	float lightmap_get_probe_capture_update_speed() const override;

	/* LIGHTMAP INSTANCE */

	RID lightmap_instance_create(RID p_lightmap) override;
	void lightmap_instance_free(RID p_lightmap) override;
	void lightmap_instance_set_transform(RID p_lightmap, const Transform3D &p_transform) override;

	/* SHADOW ATLAS */

	RID shadow_atlas_create() override;
	void shadow_atlas_free(RID p_atlas) override;

	void shadow_atlas_set_size(RID p_atlas, int p_size, bool p_use_16_bits) override;
	void shadow_atlas_set_quadrant_subdivision(RID p_atlas, int p_quadrant, int p_subdivision) override;
	bool shadow_atlas_update_light(RID p_atlas, RID p_light_intance, float p_coverage, uint64_t p_light_version) override;

	void shadow_atlas_update(RID p_atlas) override;

	void directional_shadow_atlas_set_size(int p_size, bool p_16_bits) override;
	int get_directional_light_shadow_size(RID p_light_intance) override;
	void set_directional_shadow_count(int p_count) override;
};

#endif
