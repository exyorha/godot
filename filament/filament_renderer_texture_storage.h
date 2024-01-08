#ifndef FILAMENT_RENDERER_TEXTURE_STORAGE_H
#define FILAMENT_RENDERER_TEXTURE_STORAGE_H

#include "servers/rendering/storage/texture_storage.h"

class FilamentRendererTextureStorage final : public RendererTextureStorage {
public:
	FilamentRendererTextureStorage();
	~FilamentRendererTextureStorage() override;
	/* Canvas Texture API */

	RID canvas_texture_allocate() override;
	void canvas_texture_initialize(RID p_rid) override;
	void canvas_texture_free(RID p_rid) override;

	void canvas_texture_set_channel(RID p_canvas_texture, RS::CanvasTextureChannel p_channel, RID p_texture) override;
	void canvas_texture_set_shading_parameters(RID p_canvas_texture, const Color &p_base_color, float p_shininess) override;

	void canvas_texture_set_texture_filter(RID p_item, RS::CanvasItemTextureFilter p_filter) override;
	void canvas_texture_set_texture_repeat(RID p_item, RS::CanvasItemTextureRepeat p_repeat) override;

	/* Texture API */
	bool can_create_resources_async() const override;

	RID texture_allocate() override;
	void texture_free(RID p_rid) override;

	void texture_2d_initialize(RID p_texture, const Ref<Image> &p_image) override;
	void texture_2d_layered_initialize(RID p_texture, const Vector<Ref<Image>> &p_layers, RS::TextureLayeredType p_layered_type) override;
	void texture_3d_initialize(RID p_texture, Image::Format, int p_width, int p_height, int p_depth, bool p_mipmaps, const Vector<Ref<Image>> &p_data) override;
	void texture_proxy_initialize(RID p_texture, RID p_base) override; //all slices, then all the mipmaps, must be coherent

	void texture_2d_update(RID p_texture, const Ref<Image> &p_image, int p_layer) override;
	void texture_3d_update(RID p_texture, const Vector<Ref<Image>> &p_data) override;
	void texture_proxy_update(RID p_proxy, RID p_base) override;

	//these two APIs can be used together or in combination with the others.
	void texture_2d_placeholder_initialize(RID p_texture) override;
	void texture_2d_layered_placeholder_initialize(RID p_texture, RenderingServer::TextureLayeredType p_layered_type) override;
	void texture_3d_placeholder_initialize(RID p_texture) override;

	Ref<Image> texture_2d_get(RID p_texture) const override;
	Ref<Image> texture_2d_layer_get(RID p_texture, int p_layer) const override;
	Vector<Ref<Image>> texture_3d_get(RID p_texture) const override;

	void texture_replace(RID p_texture, RID p_by_texture) override;
	void texture_set_size_override(RID p_texture, int p_width, int p_height) override;

	void texture_set_path(RID p_texture, const String &p_path) override;
	String texture_get_path(RID p_texture) const override;

	Image::Format texture_get_format(RID p_texture) const override;

	void texture_set_detect_3d_callback(RID p_texture, RS::TextureDetectCallback p_callback, void *p_userdata) override;
	void texture_set_detect_normal_callback(RID p_texture, RS::TextureDetectCallback p_callback, void *p_userdata) override;
	void texture_set_detect_roughness_callback(RID p_texture, RS::TextureDetectRoughnessCallback p_callback, void *p_userdata) override;

	void texture_debug_usage(List<RS::TextureInfo> *r_info) override;

	void texture_set_force_redraw_if_visible(RID p_texture, bool p_enable) override;

	Size2 texture_size_with_proxy(RID p_proxy) override;

	void texture_rd_initialize(RID p_texture, const RID &p_rd_texture, const RenderingServer::TextureLayeredType p_layer_type) override;
	RID texture_get_rd_texture(RID p_texture, bool p_srgb) const override;
	uint64_t texture_get_native_handle(RID p_texture, bool p_srgb) const override;

	/* Decal API */
	RID decal_allocate() override;
	void decal_initialize(RID p_rid) override;
	void decal_free(RID p_rid) override;

	void decal_set_size(RID p_decal, const Vector3 &p_size) override;
	void decal_set_texture(RID p_decal, RS::DecalTexture p_type, RID p_texture) override;
	void decal_set_emission_energy(RID p_decal, float p_energy) override;
	void decal_set_albedo_mix(RID p_decal, float p_mix) override;
	void decal_set_modulate(RID p_decal, const Color &p_modulate) override;
	void decal_set_cull_mask(RID p_decal, uint32_t p_layers) override;
	void decal_set_distance_fade(RID p_decal, bool p_enabled, float p_begin, float p_length) override;
	void decal_set_fade(RID p_decal, float p_above, float p_below) override;
	void decal_set_normal_fade(RID p_decal, float p_fade) override;

	AABB decal_get_aabb(RID p_decal) const override;
	uint32_t decal_get_cull_mask(RID p_decal) const override;

	void texture_add_to_decal_atlas(RID p_texture, bool p_panorama_to_dp) override;
	void texture_remove_from_decal_atlas(RID p_texture, bool p_panorama_to_dp) override;

	/* DECAL INSTANCE */

	RID decal_instance_create(RID p_decal) override;
	void decal_instance_free(RID p_decal_instance) override;
	void decal_instance_set_transform(RID p_decal_instance, const Transform3D &p_transform) override;
	void decal_instance_set_sorting_offset(RID p_decal_instance, float p_sorting_offset) override;

	/* RENDER TARGET */

	RID render_target_create() override;
	void render_target_free(RID p_rid) override;

	void render_target_set_position(RID p_render_target, int p_x, int p_y) override; // Q change input to const Point2i &p_position ?
	Point2i render_target_get_position(RID p_render_target) const override;
	void render_target_set_size(RID p_render_target, int p_width, int p_height, uint32_t p_view_count) override; // Q change input to const Size2i &p_size ?
	Size2i render_target_get_size(RID p_render_target) const override;
	void render_target_set_transparent(RID p_render_target, bool p_is_transparent) override;
	bool render_target_get_transparent(RID p_render_target) const override;
	void render_target_set_direct_to_screen(RID p_render_target, bool p_direct_to_screen) override;
	bool render_target_get_direct_to_screen(RID p_render_target) const override;
	bool render_target_was_used(RID p_render_target) const override;
	void render_target_set_as_unused(RID p_render_target) override;
	void render_target_set_msaa(RID p_render_target, RS::ViewportMSAA p_msaa) override;
	RS::ViewportMSAA render_target_get_msaa(RID p_render_target) const override;
	void render_target_set_msaa_needs_resolve(RID p_render_target, bool p_needs_resolve) override;
	bool render_target_get_msaa_needs_resolve(RID p_render_target) const override;
	void render_target_do_msaa_resolve(RID p_render_target) override;
	void render_target_set_use_hdr(RID p_render_target, bool p_use_hdr) override;
	bool render_target_is_using_hdr(RID p_render_target) const override;

	void render_target_request_clear(RID p_render_target, const Color &p_clear_color) override;
	bool render_target_is_clear_requested(RID p_render_target) override;
	Color render_target_get_clear_request_color(RID p_render_target) override;
	void render_target_disable_clear_request(RID p_render_target) override;
	void render_target_do_clear_request(RID p_render_target) override;

	void render_target_set_sdf_size_and_scale(RID p_render_target, RS::ViewportSDFOversize p_size, RS::ViewportSDFScale p_scale) override;
	Rect2i render_target_get_sdf_rect(RID p_render_target) const override;
	void render_target_mark_sdf_enabled(RID p_render_target, bool p_enabled) override;

	void render_target_set_vrs_mode(RID p_render_target, RS::ViewportVRSMode p_mode) override;
	RS::ViewportVRSMode render_target_get_vrs_mode(RID p_render_target) const override;
	void render_target_set_vrs_texture(RID p_render_target, RID p_texture) override;
	RID render_target_get_vrs_texture(RID p_render_target) const override;

	// override color, depth and velocity buffers (depth and velocity only for 3D)
	void render_target_set_override(RID p_render_target, RID p_color_texture, RID p_depth_texture, RID p_velocity_texture) override;
	RID render_target_get_override_color(RID p_render_target) const override;
	RID render_target_get_override_depth(RID p_render_target) const override;
	RID render_target_get_override_velocity(RID p_render_target) const override;

	// get textures
	RID render_target_get_texture(RID p_render_target) override;
};

#endif

