#include "filament_rendering_server_backend.h"
#include "filament/filament_window.h"

#include <cstdio>
#include <stdexcept>

#include <filament/Engine.h>

filament::Engine *FilamentRenderingServerBackend::m_filamentEngine = nullptr;

FilamentRenderingServerBackend::FilamentRenderingServerBackend() : m_engine(filament::Engine::create(), &m_filamentEngine) {
	if(!m_engine) {
		throw std::runtime_error("failed to create the filament Engine");
	}
}

FilamentRenderingServerBackend::~FilamentRenderingServerBackend() = default;

FilamentRenderingServerBackend::EnginePointer::EnginePointer(filament::Engine *engine, filament::Engine **pointerToPointer) noexcept : m_ptr(engine),
	m_pointerToPointer(pointerToPointer) {

	if(m_pointerToPointer) {
		*m_pointerToPointer = engine;
	}
}

FilamentRenderingServerBackend::EnginePointer::~EnginePointer() {
	filament::Engine::destroy(&m_ptr);

	if(m_pointerToPointer) {
		*m_pointerToPointer = nullptr;
	}
}

void FilamentRenderingServerBackend::runStepOnThread() {
	bool hadAnyWindows = false;

	for(const auto &windowPtr: m_windows) {
		auto window = windowPtr.get();
		if(window) {
			window->renderWindow();

			hadAnyWindows = true;
		}
	}

	if(!hadAnyWindows) {
		/*
		* If we don't have any windows active, sleep a bit to not burn the CPU
		* in a tight loop. Otherwise, the window renderers will pace the loop.
		*/

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

void FilamentRenderingServerBackend::shutdown() {
	m_windows.clear();
	m_objectManager.clear();
}

void FilamentRenderingServerBackend::texture_2d_create(RID output, const Ref<Image> & p_image)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "texture_2d_create");
};

void FilamentRenderingServerBackend::texture_2d_layered_create(RID output, const Vector<Ref<Image>> & p_layers, RenderingServer::TextureLayeredType p_layered_type)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "texture_2d_layered_create");
};

void FilamentRenderingServerBackend::texture_3d_create(RID output, Image::Format anonarg, int p_width, int p_height, int p_depth, bool p_mipmaps, const Vector<Ref<Image>> & p_data)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "texture_3d_create");
};

void FilamentRenderingServerBackend::texture_proxy_create(RID output, RID p_base)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "texture_proxy_create");
};

void FilamentRenderingServerBackend::texture_2d_update(RID p_texture, const Ref<Image> & p_image, int p_layer)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "texture_2d_update");
};

void FilamentRenderingServerBackend::texture_3d_update(RID p_texture, const Vector<Ref<Image>> & p_data)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "texture_3d_update");
};

void FilamentRenderingServerBackend::texture_proxy_update(RID p_texture, RID p_proxy_to)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "texture_proxy_update");
};

void FilamentRenderingServerBackend::texture_2d_placeholder_create(RID output)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "texture_2d_placeholder_create");
};

void FilamentRenderingServerBackend::texture_2d_layered_placeholder_create(RID output, RenderingServer::TextureLayeredType p_layered_type)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "texture_2d_layered_placeholder_create");
};

void FilamentRenderingServerBackend::texture_3d_placeholder_create(RID output)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "texture_3d_placeholder_create");
};

Ref<Image> FilamentRenderingServerBackend::texture_2d_get(RID p_texture) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "texture_2d_get");
	return Ref<Image>();
};

Ref<Image> FilamentRenderingServerBackend::texture_2d_layer_get(RID p_texture, int p_layer) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "texture_2d_layer_get");
	return Ref<Image>();
};

Vector<Ref<Image>> FilamentRenderingServerBackend::texture_3d_get(RID p_texture) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "texture_3d_get");
	return Vector<Ref<Image>>();
};

void FilamentRenderingServerBackend::texture_replace(RID p_texture, RID p_by_texture)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "texture_replace");
};

void FilamentRenderingServerBackend::texture_set_size_override(RID p_texture, int p_width, int p_height)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "texture_set_size_override");
};

void FilamentRenderingServerBackend::texture_set_path(RID p_texture, const String & p_path)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "texture_set_path");
};

String FilamentRenderingServerBackend::texture_get_path(RID p_texture) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "texture_get_path");
	return String();
};

Image::Format FilamentRenderingServerBackend::texture_get_format(RID p_texture) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "texture_get_format");
	return Image::Format();
};

void FilamentRenderingServerBackend::texture_set_detect_3d_callback(RID p_texture, RenderingServer::TextureDetectCallback p_callback, void * p_userdata)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "texture_set_detect_3d_callback");
};

void FilamentRenderingServerBackend::texture_set_detect_normal_callback(RID p_texture, RenderingServer::TextureDetectCallback p_callback, void * p_userdata)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "texture_set_detect_normal_callback");
};

void FilamentRenderingServerBackend::texture_set_detect_roughness_callback(RID p_texture, RenderingServer::TextureDetectRoughnessCallback p_callback, void * p_userdata)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "texture_set_detect_roughness_callback");
};

void FilamentRenderingServerBackend::texture_debug_usage(List<RenderingServer::TextureInfo> * r_info)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "texture_debug_usage");
};

void FilamentRenderingServerBackend::texture_set_force_redraw_if_visible(RID p_texture, bool p_enable)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "texture_set_force_redraw_if_visible");
};

void FilamentRenderingServerBackend::texture_rd_create(RID output, const RID & p_rd_texture, const RenderingServer::TextureLayeredType p_layer_type)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "texture_rd_create");
};

RID FilamentRenderingServerBackend::texture_get_rd_texture(RID p_texture, bool p_srgb) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "texture_get_rd_texture");
	return RID();
};

uint64_t FilamentRenderingServerBackend::texture_get_native_handle(RID p_texture, bool p_srgb) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "texture_get_native_handle");
	return uint64_t();
};

void FilamentRenderingServerBackend::shader_create(RID output)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "shader_create");
};

void FilamentRenderingServerBackend::shader_set_code(RID p_shader, const String & p_code)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "shader_set_code");
};

void FilamentRenderingServerBackend::shader_set_path_hint(RID p_shader, const String & p_path)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "shader_set_path_hint");
};

String FilamentRenderingServerBackend::shader_get_code(RID p_shader) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "shader_get_code");
	return String();
};

void FilamentRenderingServerBackend::get_shader_parameter_list(RID p_shader, List<PropertyInfo> * p_param_list) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "get_shader_parameter_list");
};

Variant FilamentRenderingServerBackend::shader_get_parameter_default(RID p_shader, const StringName & p_param) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "shader_get_parameter_default");
	return Variant();
};

void FilamentRenderingServerBackend::shader_set_default_texture_parameter(RID p_shader, const StringName & p_name, RID p_texture, int p_index)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "shader_set_default_texture_parameter");
};

RID FilamentRenderingServerBackend::shader_get_default_texture_parameter(RID p_shader, const StringName & p_name, int p_index) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "shader_get_default_texture_parameter");
	return RID();
};

RenderingServer::ShaderNativeSourceCode FilamentRenderingServerBackend::shader_get_native_source_code(RID p_shader) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "shader_get_native_source_code");
	return RenderingServer::ShaderNativeSourceCode();
};

void FilamentRenderingServerBackend::material_create(RID output)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "material_create");
};

void FilamentRenderingServerBackend::material_set_shader(RID p_shader_material, RID p_shader)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "material_set_shader");
};

void FilamentRenderingServerBackend::material_set_param(RID p_material, const StringName & p_param, const Variant & p_value)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "material_set_param");
};

Variant FilamentRenderingServerBackend::material_get_param(RID p_material, const StringName & p_param) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "material_get_param");
	return Variant();
};

void FilamentRenderingServerBackend::material_set_render_priority(RID p_material, int priority)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "material_set_render_priority");
};

void FilamentRenderingServerBackend::material_set_next_pass(RID p_material, RID p_next_material)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "material_set_next_pass");
};

void FilamentRenderingServerBackend::mesh_create_from_surfaces(RID output, const Vector<RenderingServer::SurfaceData> & p_surfaces, int p_blend_shape_count)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "mesh_create_from_surfaces");
};

void FilamentRenderingServerBackend::mesh_create(RID output)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "mesh_create");
};

void FilamentRenderingServerBackend::mesh_set_blend_shape_count(RID p_mesh, int p_blend_shape_count)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "mesh_set_blend_shape_count");
};

void FilamentRenderingServerBackend::mesh_add_surface(RID p_mesh, const RenderingServer::SurfaceData & p_surface)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "mesh_add_surface");
};

int FilamentRenderingServerBackend::mesh_get_blend_shape_count(RID p_mesh) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "mesh_get_blend_shape_count");
	return int();
};

void FilamentRenderingServerBackend::mesh_set_blend_shape_mode(RID p_mesh, RenderingServer::BlendShapeMode p_mode)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "mesh_set_blend_shape_mode");
};

RenderingServer::BlendShapeMode FilamentRenderingServerBackend::mesh_get_blend_shape_mode(RID p_mesh) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "mesh_get_blend_shape_mode");
	return RenderingServer::BlendShapeMode();
};

void FilamentRenderingServerBackend::mesh_surface_update_vertex_region(RID p_mesh, int p_surface, int p_offset, const Vector<uint8_t> & p_data)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "mesh_surface_update_vertex_region");
};

void FilamentRenderingServerBackend::mesh_surface_update_attribute_region(RID p_mesh, int p_surface, int p_offset, const Vector<uint8_t> & p_data)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "mesh_surface_update_attribute_region");
};

void FilamentRenderingServerBackend::mesh_surface_update_skin_region(RID p_mesh, int p_surface, int p_offset, const Vector<uint8_t> & p_data)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "mesh_surface_update_skin_region");
};

void FilamentRenderingServerBackend::mesh_surface_set_material(RID p_mesh, int p_surface, RID p_material)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "mesh_surface_set_material");
};

RID FilamentRenderingServerBackend::mesh_surface_get_material(RID p_mesh, int p_surface) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "mesh_surface_get_material");
	return RID();
};

RenderingServer::SurfaceData FilamentRenderingServerBackend::mesh_get_surface(RID p_mesh, int p_surface) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "mesh_get_surface");
	return RenderingServer::SurfaceData();
};

int FilamentRenderingServerBackend::mesh_get_surface_count(RID p_mesh) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "mesh_get_surface_count");
	return int();
};

void FilamentRenderingServerBackend::mesh_set_custom_aabb(RID p_mesh, const AABB & p_aabb)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "mesh_set_custom_aabb");
};

AABB FilamentRenderingServerBackend::mesh_get_custom_aabb(RID p_mesh) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "mesh_get_custom_aabb");
	return AABB();
};

void FilamentRenderingServerBackend::mesh_set_shadow_mesh(RID p_mesh, RID p_shadow_mesh)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "mesh_set_shadow_mesh");
};

void FilamentRenderingServerBackend::mesh_clear(RID p_mesh)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "mesh_clear");
};

void FilamentRenderingServerBackend::multimesh_create(RID output)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "multimesh_create");
};

void FilamentRenderingServerBackend::multimesh_allocate_data(RID p_multimesh, int p_instances, RenderingServer::MultimeshTransformFormat p_transform_format, bool p_use_colors, bool p_use_custom_data)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "multimesh_allocate_data");
};

int FilamentRenderingServerBackend::multimesh_get_instance_count(RID p_multimesh) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "multimesh_get_instance_count");
	return int();
};

void FilamentRenderingServerBackend::multimesh_set_mesh(RID p_multimesh, RID p_mesh)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "multimesh_set_mesh");
};

void FilamentRenderingServerBackend::multimesh_instance_set_transform(RID p_multimesh, int p_index, const Transform3D & p_transform)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "multimesh_instance_set_transform");
};

void FilamentRenderingServerBackend::multimesh_instance_set_transform_2d(RID p_multimesh, int p_index, const Transform2D & p_transform)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "multimesh_instance_set_transform_2d");
};

void FilamentRenderingServerBackend::multimesh_instance_set_color(RID p_multimesh, int p_index, const Color & p_color)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "multimesh_instance_set_color");
};

void FilamentRenderingServerBackend::multimesh_instance_set_custom_data(RID p_multimesh, int p_index, const Color & p_color)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "multimesh_instance_set_custom_data");
};

RID FilamentRenderingServerBackend::multimesh_get_mesh(RID p_multimesh) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "multimesh_get_mesh");
	return RID();
};

AABB FilamentRenderingServerBackend::multimesh_get_aabb(RID p_multimesh) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "multimesh_get_aabb");
	return AABB();
};

Transform3D FilamentRenderingServerBackend::multimesh_instance_get_transform(RID p_multimesh, int p_index) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "multimesh_instance_get_transform");
	return Transform3D();
};

Transform2D FilamentRenderingServerBackend::multimesh_instance_get_transform_2d(RID p_multimesh, int p_index) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "multimesh_instance_get_transform_2d");
	return Transform2D();
};

Color FilamentRenderingServerBackend::multimesh_instance_get_color(RID p_multimesh, int p_index) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "multimesh_instance_get_color");
	return Color();
};

Color FilamentRenderingServerBackend::multimesh_instance_get_custom_data(RID p_multimesh, int p_index) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "multimesh_instance_get_custom_data");
	return Color();
};

void FilamentRenderingServerBackend::multimesh_set_buffer(RID p_multimesh, const Vector<float> & p_buffer)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "multimesh_set_buffer");
};

Vector<float> FilamentRenderingServerBackend::multimesh_get_buffer(RID p_multimesh) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "multimesh_get_buffer");
	return Vector<float>();
};

void FilamentRenderingServerBackend::multimesh_set_visible_instances(RID p_multimesh, int p_visible)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "multimesh_set_visible_instances");
};

int FilamentRenderingServerBackend::multimesh_get_visible_instances(RID p_multimesh) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "multimesh_get_visible_instances");
	return int();
};

void FilamentRenderingServerBackend::skeleton_create(RID output)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "skeleton_create");
};

void FilamentRenderingServerBackend::skeleton_allocate_data(RID p_skeleton, int p_bones, bool p_2d_skeleton)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "skeleton_allocate_data");
};

int FilamentRenderingServerBackend::skeleton_get_bone_count(RID p_skeleton) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "skeleton_get_bone_count");
	return int();
};

void FilamentRenderingServerBackend::skeleton_bone_set_transform(RID p_skeleton, int p_bone, const Transform3D & p_transform)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "skeleton_bone_set_transform");
};

Transform3D FilamentRenderingServerBackend::skeleton_bone_get_transform(RID p_skeleton, int p_bone) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "skeleton_bone_get_transform");
	return Transform3D();
};

void FilamentRenderingServerBackend::skeleton_bone_set_transform_2d(RID p_skeleton, int p_bone, const Transform2D & p_transform)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "skeleton_bone_set_transform_2d");
};

Transform2D FilamentRenderingServerBackend::skeleton_bone_get_transform_2d(RID p_skeleton, int p_bone) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "skeleton_bone_get_transform_2d");
	return Transform2D();
};

void FilamentRenderingServerBackend::skeleton_set_base_transform_2d(RID p_skeleton, const Transform2D & p_base_transform)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "skeleton_set_base_transform_2d");
};

void FilamentRenderingServerBackend::directional_light_create(RID output)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "directional_light_create");
};

void FilamentRenderingServerBackend::omni_light_create(RID output)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "omni_light_create");
};

void FilamentRenderingServerBackend::spot_light_create(RID output)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "spot_light_create");
};

void FilamentRenderingServerBackend::light_set_color(RID p_light, const Color & p_color)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "light_set_color");
};

void FilamentRenderingServerBackend::light_set_param(RID p_light, RenderingServer::LightParam p_param, float p_value)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "light_set_param");
};

void FilamentRenderingServerBackend::light_set_shadow(RID p_light, bool p_enabled)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "light_set_shadow");
};

void FilamentRenderingServerBackend::light_set_projector(RID p_light, RID p_texture)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "light_set_projector");
};

void FilamentRenderingServerBackend::light_set_negative(RID p_light, bool p_enable)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "light_set_negative");
};

void FilamentRenderingServerBackend::light_set_cull_mask(RID p_light, uint32_t p_mask)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "light_set_cull_mask");
};

void FilamentRenderingServerBackend::light_set_distance_fade(RID p_light, bool p_enabled, float p_begin, float p_shadow, float p_length)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "light_set_distance_fade");
};

void FilamentRenderingServerBackend::light_set_reverse_cull_face_mode(RID p_light, bool p_enabled)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "light_set_reverse_cull_face_mode");
};

void FilamentRenderingServerBackend::light_set_bake_mode(RID p_light, RenderingServer::LightBakeMode p_bake_mode)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "light_set_bake_mode");
};

void FilamentRenderingServerBackend::light_set_max_sdfgi_cascade(RID p_light, uint32_t p_cascade)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "light_set_max_sdfgi_cascade");
};

void FilamentRenderingServerBackend::light_omni_set_shadow_mode(RID p_light, RenderingServer::LightOmniShadowMode p_mode)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "light_omni_set_shadow_mode");
};

void FilamentRenderingServerBackend::light_directional_set_shadow_mode(RID p_light, RenderingServer::LightDirectionalShadowMode p_mode)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "light_directional_set_shadow_mode");
};

void FilamentRenderingServerBackend::light_directional_set_blend_splits(RID p_light, bool p_enable)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "light_directional_set_blend_splits");
};

void FilamentRenderingServerBackend::light_directional_set_sky_mode(RID p_light, RenderingServer::LightDirectionalSkyMode p_mode)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "light_directional_set_sky_mode");
};

void FilamentRenderingServerBackend::shadow_atlas_create(RID output)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "shadow_atlas_create");
};

void FilamentRenderingServerBackend::shadow_atlas_set_size(RID p_atlas, int p_size, bool p_use_16_bits)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "shadow_atlas_set_size");
};

void FilamentRenderingServerBackend::shadow_atlas_set_quadrant_subdivision(RID p_atlas, int p_quadrant, int p_subdivision)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "shadow_atlas_set_quadrant_subdivision");
};

void FilamentRenderingServerBackend::directional_shadow_atlas_set_size(int p_size, bool p_16_bits)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "directional_shadow_atlas_set_size");
};

void FilamentRenderingServerBackend::positional_soft_shadow_filter_set_quality(RenderingServer::ShadowQuality p_quality)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "positional_soft_shadow_filter_set_quality");
};

void FilamentRenderingServerBackend::directional_soft_shadow_filter_set_quality(RenderingServer::ShadowQuality p_quality)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "directional_soft_shadow_filter_set_quality");
};

void FilamentRenderingServerBackend::light_projectors_set_filter(RenderingServer::LightProjectorFilter p_filter)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "light_projectors_set_filter");
};

void FilamentRenderingServerBackend::reflection_probe_create(RID output)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "reflection_probe_create");
};

void FilamentRenderingServerBackend::reflection_probe_set_update_mode(RID p_probe, RenderingServer::ReflectionProbeUpdateMode p_mode)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "reflection_probe_set_update_mode");
};

void FilamentRenderingServerBackend::reflection_probe_set_intensity(RID p_probe, float p_intensity)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "reflection_probe_set_intensity");
};

void FilamentRenderingServerBackend::reflection_probe_set_ambient_mode(RID p_probe, RenderingServer::ReflectionProbeAmbientMode p_mode)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "reflection_probe_set_ambient_mode");
};

void FilamentRenderingServerBackend::reflection_probe_set_ambient_color(RID p_probe, const Color & p_color)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "reflection_probe_set_ambient_color");
};

void FilamentRenderingServerBackend::reflection_probe_set_ambient_energy(RID p_probe, float p_energy)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "reflection_probe_set_ambient_energy");
};

void FilamentRenderingServerBackend::reflection_probe_set_max_distance(RID p_probe, float p_distance)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "reflection_probe_set_max_distance");
};

void FilamentRenderingServerBackend::reflection_probe_set_size(RID p_probe, const Vector3 & p_size)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "reflection_probe_set_size");
};

void FilamentRenderingServerBackend::reflection_probe_set_origin_offset(RID p_probe, const Vector3 & p_offset)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "reflection_probe_set_origin_offset");
};

void FilamentRenderingServerBackend::reflection_probe_set_as_interior(RID p_probe, bool p_enable)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "reflection_probe_set_as_interior");
};

void FilamentRenderingServerBackend::reflection_probe_set_enable_box_projection(RID p_probe, bool p_enable)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "reflection_probe_set_enable_box_projection");
};

void FilamentRenderingServerBackend::reflection_probe_set_enable_shadows(RID p_probe, bool p_enable)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "reflection_probe_set_enable_shadows");
};

void FilamentRenderingServerBackend::reflection_probe_set_cull_mask(RID p_probe, uint32_t p_layers)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "reflection_probe_set_cull_mask");
};

void FilamentRenderingServerBackend::reflection_probe_set_resolution(RID p_probe, int p_resolution)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "reflection_probe_set_resolution");
};

void FilamentRenderingServerBackend::reflection_probe_set_mesh_lod_threshold(RID p_probe, float p_pixels)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "reflection_probe_set_mesh_lod_threshold");
};

void FilamentRenderingServerBackend::decal_create(RID output)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "decal_create");
};

void FilamentRenderingServerBackend::decal_set_size(RID p_decal, const Vector3 & p_size)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "decal_set_size");
};

void FilamentRenderingServerBackend::decal_set_texture(RID p_decal, RenderingServer::DecalTexture p_type, RID p_texture)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "decal_set_texture");
};

void FilamentRenderingServerBackend::decal_set_emission_energy(RID p_decal, float p_energy)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "decal_set_emission_energy");
};

void FilamentRenderingServerBackend::decal_set_albedo_mix(RID p_decal, float p_mix)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "decal_set_albedo_mix");
};

void FilamentRenderingServerBackend::decal_set_modulate(RID p_decal, const Color & p_modulate)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "decal_set_modulate");
};

void FilamentRenderingServerBackend::decal_set_cull_mask(RID p_decal, uint32_t p_layers)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "decal_set_cull_mask");
};

void FilamentRenderingServerBackend::decal_set_distance_fade(RID p_decal, bool p_enabled, float p_begin, float p_length)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "decal_set_distance_fade");
};

void FilamentRenderingServerBackend::decal_set_fade(RID p_decal, float p_above, float p_below)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "decal_set_fade");
};

void FilamentRenderingServerBackend::decal_set_normal_fade(RID p_decal, float p_fade)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "decal_set_normal_fade");
};

void FilamentRenderingServerBackend::decals_set_filter(RenderingServer::DecalFilter p_quality)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "decals_set_filter");
};

void FilamentRenderingServerBackend::voxel_gi_create(RID output)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "voxel_gi_create");
};

void FilamentRenderingServerBackend::voxel_gi_allocate_data(RID p_voxel_gi, const Transform3D & p_to_cell_xform, const AABB & p_aabb, const Vector3i & p_octree_size, const Vector<uint8_t> & p_octree_cells, const Vector<uint8_t> & p_data_cells, const Vector<uint8_t> & p_distance_field, const Vector<int> & p_level_counts)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "voxel_gi_allocate_data");
};

AABB FilamentRenderingServerBackend::voxel_gi_get_bounds(RID p_voxel_gi) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "voxel_gi_get_bounds");
	return AABB();
};

Vector3i FilamentRenderingServerBackend::voxel_gi_get_octree_size(RID p_voxel_gi) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "voxel_gi_get_octree_size");
	return Vector3i();
};

Vector<uint8_t> FilamentRenderingServerBackend::voxel_gi_get_octree_cells(RID p_voxel_gi) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "voxel_gi_get_octree_cells");
	return Vector<uint8_t>();
};

Vector<uint8_t> FilamentRenderingServerBackend::voxel_gi_get_data_cells(RID p_voxel_gi) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "voxel_gi_get_data_cells");
	return Vector<uint8_t>();
};

Vector<uint8_t> FilamentRenderingServerBackend::voxel_gi_get_distance_field(RID p_voxel_gi) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "voxel_gi_get_distance_field");
	return Vector<uint8_t>();
};

Vector<int> FilamentRenderingServerBackend::voxel_gi_get_level_counts(RID p_voxel_gi) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "voxel_gi_get_level_counts");
	return Vector<int>();
};

Transform3D FilamentRenderingServerBackend::voxel_gi_get_to_cell_xform(RID p_voxel_gi) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "voxel_gi_get_to_cell_xform");
	return Transform3D();
};

void FilamentRenderingServerBackend::voxel_gi_set_dynamic_range(RID p_voxel_gi, float p_range)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "voxel_gi_set_dynamic_range");
};

void FilamentRenderingServerBackend::voxel_gi_set_propagation(RID p_voxel_gi, float p_range)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "voxel_gi_set_propagation");
};

void FilamentRenderingServerBackend::voxel_gi_set_energy(RID p_voxel_gi, float p_energy)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "voxel_gi_set_energy");
};

void FilamentRenderingServerBackend::voxel_gi_set_baked_exposure_normalization(RID p_voxel_gi, float p_baked_exposure)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "voxel_gi_set_baked_exposure_normalization");
};

void FilamentRenderingServerBackend::voxel_gi_set_bias(RID p_voxel_gi, float p_bias)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "voxel_gi_set_bias");
};

void FilamentRenderingServerBackend::voxel_gi_set_normal_bias(RID p_voxel_gi, float p_range)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "voxel_gi_set_normal_bias");
};

void FilamentRenderingServerBackend::voxel_gi_set_interior(RID p_voxel_gi, bool p_enable)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "voxel_gi_set_interior");
};

void FilamentRenderingServerBackend::voxel_gi_set_use_two_bounces(RID p_voxel_gi, bool p_enable)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "voxel_gi_set_use_two_bounces");
};

void FilamentRenderingServerBackend::voxel_gi_set_quality(RenderingServer::VoxelGIQuality anonarg)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "voxel_gi_set_quality");
};

void FilamentRenderingServerBackend::sdfgi_reset()  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "sdfgi_reset");
};

void FilamentRenderingServerBackend::lightmap_create(RID output)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "lightmap_create");
};

void FilamentRenderingServerBackend::lightmap_set_textures(RID p_lightmap, RID p_light, bool p_uses_spherical_haromics)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "lightmap_set_textures");
};

void FilamentRenderingServerBackend::lightmap_set_probe_bounds(RID p_lightmap, const AABB & p_bounds)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "lightmap_set_probe_bounds");
};

void FilamentRenderingServerBackend::lightmap_set_probe_interior(RID p_lightmap, bool p_interior)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "lightmap_set_probe_interior");
};

void FilamentRenderingServerBackend::lightmap_set_probe_capture_data(RID p_lightmap, const PackedVector3Array & p_points, const PackedColorArray & p_point_sh, const PackedInt32Array & p_tetrahedra, const PackedInt32Array & p_bsp_tree)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "lightmap_set_probe_capture_data");
};

void FilamentRenderingServerBackend::lightmap_set_baked_exposure_normalization(RID p_lightmap, float p_exposure)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "lightmap_set_baked_exposure_normalization");
};

PackedVector3Array FilamentRenderingServerBackend::lightmap_get_probe_capture_points(RID p_lightmap) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "lightmap_get_probe_capture_points");
	return PackedVector3Array();
};

PackedColorArray FilamentRenderingServerBackend::lightmap_get_probe_capture_sh(RID p_lightmap) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "lightmap_get_probe_capture_sh");
	return PackedColorArray();
};

PackedInt32Array FilamentRenderingServerBackend::lightmap_get_probe_capture_tetrahedra(RID p_lightmap) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "lightmap_get_probe_capture_tetrahedra");
	return PackedInt32Array();
};

PackedInt32Array FilamentRenderingServerBackend::lightmap_get_probe_capture_bsp_tree(RID p_lightmap) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "lightmap_get_probe_capture_bsp_tree");
	return PackedInt32Array();
};

void FilamentRenderingServerBackend::lightmap_set_probe_capture_update_speed(float p_speed)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "lightmap_set_probe_capture_update_speed");
};

void FilamentRenderingServerBackend::particles_create(RID output)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "particles_create");
};

void FilamentRenderingServerBackend::particles_set_mode(RID p_particles, RenderingServer::ParticlesMode p_mode)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "particles_set_mode");
};

void FilamentRenderingServerBackend::particles_set_emitting(RID p_particles, bool p_enable)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "particles_set_emitting");
};

bool FilamentRenderingServerBackend::particles_get_emitting(RID p_particles)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "particles_get_emitting");
	return bool();
};

void FilamentRenderingServerBackend::particles_set_amount(RID p_particles, int p_amount)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "particles_set_amount");
};

void FilamentRenderingServerBackend::particles_set_amount_ratio(RID p_particles, float p_amount_ratio)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "particles_set_amount_ratio");
};

void FilamentRenderingServerBackend::particles_set_lifetime(RID p_particles, double p_lifetime)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "particles_set_lifetime");
};

void FilamentRenderingServerBackend::particles_set_one_shot(RID p_particles, bool p_one_shot)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "particles_set_one_shot");
};

void FilamentRenderingServerBackend::particles_set_pre_process_time(RID p_particles, double p_time)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "particles_set_pre_process_time");
};

void FilamentRenderingServerBackend::particles_set_explosiveness_ratio(RID p_particles, float p_ratio)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "particles_set_explosiveness_ratio");
};

void FilamentRenderingServerBackend::particles_set_randomness_ratio(RID p_particles, float p_ratio)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "particles_set_randomness_ratio");
};

void FilamentRenderingServerBackend::particles_set_custom_aabb(RID p_particles, const AABB & p_aabb)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "particles_set_custom_aabb");
};

void FilamentRenderingServerBackend::particles_set_speed_scale(RID p_particles, double p_scale)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "particles_set_speed_scale");
};

void FilamentRenderingServerBackend::particles_set_use_local_coordinates(RID p_particles, bool p_enable)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "particles_set_use_local_coordinates");
};

void FilamentRenderingServerBackend::particles_set_process_material(RID p_particles, RID p_material)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "particles_set_process_material");
};

void FilamentRenderingServerBackend::particles_set_fixed_fps(RID p_particles, int p_fps)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "particles_set_fixed_fps");
};

void FilamentRenderingServerBackend::particles_set_interpolate(RID p_particles, bool p_enable)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "particles_set_interpolate");
};

void FilamentRenderingServerBackend::particles_set_fractional_delta(RID p_particles, bool p_enable)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "particles_set_fractional_delta");
};

void FilamentRenderingServerBackend::particles_set_collision_base_size(RID p_particles, float p_size)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "particles_set_collision_base_size");
};

void FilamentRenderingServerBackend::particles_set_transform_align(RID p_particles, RenderingServer::ParticlesTransformAlign p_transform_align)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "particles_set_transform_align");
};

void FilamentRenderingServerBackend::particles_set_trails(RID p_particles, bool p_enable, float p_length_sec)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "particles_set_trails");
};

void FilamentRenderingServerBackend::particles_set_trail_bind_poses(RID p_particles, const Vector<Transform3D> & p_bind_poses)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "particles_set_trail_bind_poses");
};

bool FilamentRenderingServerBackend::particles_is_inactive(RID p_particles)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "particles_is_inactive");
	return bool();
};

void FilamentRenderingServerBackend::particles_request_process(RID p_particles)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "particles_request_process");
};

void FilamentRenderingServerBackend::particles_restart(RID p_particles)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "particles_restart");
};

void FilamentRenderingServerBackend::particles_set_subemitter(RID p_particles, RID p_subemitter_particles)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "particles_set_subemitter");
};

void FilamentRenderingServerBackend::particles_emit(RID p_particles, const Transform3D & p_transform, const Vector3 & p_velocity, const Color & p_color, const Color & p_custom, uint32_t p_emit_flags)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "particles_emit");
};

void FilamentRenderingServerBackend::particles_set_draw_order(RID p_particles, RenderingServer::ParticlesDrawOrder p_order)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "particles_set_draw_order");
};

void FilamentRenderingServerBackend::particles_set_draw_passes(RID p_particles, int p_count)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "particles_set_draw_passes");
};

void FilamentRenderingServerBackend::particles_set_draw_pass_mesh(RID p_particles, int p_pass, RID p_mesh)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "particles_set_draw_pass_mesh");
};

AABB FilamentRenderingServerBackend::particles_get_current_aabb(RID p_particles)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "particles_get_current_aabb");
	return AABB();
};

void FilamentRenderingServerBackend::particles_set_emission_transform(RID p_particles, const Transform3D & p_transform)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "particles_set_emission_transform");
};

void FilamentRenderingServerBackend::particles_set_emitter_velocity(RID p_particles, const Vector3 & p_velocity)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "particles_set_emitter_velocity");
};

void FilamentRenderingServerBackend::particles_set_interp_to_end(RID p_particles, float p_interp)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "particles_set_interp_to_end");
};

void FilamentRenderingServerBackend::particles_collision_create(RID output)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "particles_collision_create");
};

void FilamentRenderingServerBackend::particles_collision_set_collision_type(RID p_particles_collision, RenderingServer::ParticlesCollisionType p_type)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "particles_collision_set_collision_type");
};

void FilamentRenderingServerBackend::particles_collision_set_cull_mask(RID p_particles_collision, uint32_t p_cull_mask)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "particles_collision_set_cull_mask");
};

void FilamentRenderingServerBackend::particles_collision_set_sphere_radius(RID p_particles_collision, real_t p_radius)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "particles_collision_set_sphere_radius");
};

void FilamentRenderingServerBackend::particles_collision_set_box_extents(RID p_particles_collision, const Vector3 & p_extents)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "particles_collision_set_box_extents");
};

void FilamentRenderingServerBackend::particles_collision_set_attractor_strength(RID p_particles_collision, real_t p_strength)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "particles_collision_set_attractor_strength");
};

void FilamentRenderingServerBackend::particles_collision_set_attractor_directionality(RID p_particles_collision, real_t p_directionality)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "particles_collision_set_attractor_directionality");
};

void FilamentRenderingServerBackend::particles_collision_set_attractor_attenuation(RID p_particles_collision, real_t p_curve)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "particles_collision_set_attractor_attenuation");
};

void FilamentRenderingServerBackend::particles_collision_set_field_texture(RID p_particles_collision, RID p_texture)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "particles_collision_set_field_texture");
};

void FilamentRenderingServerBackend::particles_collision_height_field_update(RID p_particles_collision)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "particles_collision_height_field_update");
};

void FilamentRenderingServerBackend::particles_collision_set_height_field_resolution(RID p_particles_collision, RenderingServer::ParticlesCollisionHeightfieldResolution p_resolution)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "particles_collision_set_height_field_resolution");
};

void FilamentRenderingServerBackend::fog_volume_create(RID output)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "fog_volume_create");
};

void FilamentRenderingServerBackend::fog_volume_set_shape(RID p_fog_volume, RenderingServer::FogVolumeShape p_shape)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "fog_volume_set_shape");
};

void FilamentRenderingServerBackend::fog_volume_set_size(RID p_fog_volume, const Vector3 & p_size)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "fog_volume_set_size");
};

void FilamentRenderingServerBackend::fog_volume_set_material(RID p_fog_volume, RID p_material)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "fog_volume_set_material");
};

void FilamentRenderingServerBackend::visibility_notifier_create(RID output)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "visibility_notifier_create");
};

void FilamentRenderingServerBackend::visibility_notifier_set_aabb(RID p_notifier, const AABB & p_aabb)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "visibility_notifier_set_aabb");
};

void FilamentRenderingServerBackend::visibility_notifier_set_callbacks(RID p_notifier, const Callable & p_enter_callbable, const Callable & p_exit_callable)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "visibility_notifier_set_callbacks");
};

void FilamentRenderingServerBackend::occluder_create(RID output)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "occluder_create");
};

void FilamentRenderingServerBackend::occluder_set_mesh(RID p_occluder, const PackedVector3Array & p_vertices, const PackedInt32Array & p_indices)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "occluder_set_mesh");
};

void FilamentRenderingServerBackend::camera_create(RID output)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "camera_create");
};

void FilamentRenderingServerBackend::camera_set_perspective(RID p_camera, float p_fovy_degrees, float p_z_near, float p_z_far)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "camera_set_perspective");
};

void FilamentRenderingServerBackend::camera_set_orthogonal(RID p_camera, float p_size, float p_z_near, float p_z_far)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "camera_set_orthogonal");
};

void FilamentRenderingServerBackend::camera_set_frustum(RID p_camera, float p_size, Vector2 p_offset, float p_z_near, float p_z_far)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "camera_set_frustum");
};

void FilamentRenderingServerBackend::camera_set_transform(RID p_camera, const Transform3D & p_transform)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "camera_set_transform");
};

void FilamentRenderingServerBackend::camera_set_cull_mask(RID p_camera, uint32_t p_layers)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "camera_set_cull_mask");
};

void FilamentRenderingServerBackend::camera_set_environment(RID p_camera, RID p_env)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "camera_set_environment");
};

void FilamentRenderingServerBackend::camera_set_camera_attributes(RID p_camera, RID p_camera_attributes)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "camera_set_camera_attributes");
};

void FilamentRenderingServerBackend::camera_set_use_vertical_aspect(RID p_camera, bool p_enable)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "camera_set_use_vertical_aspect");
};

void FilamentRenderingServerBackend::viewport_create(RID output)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "viewport_create");
};

void FilamentRenderingServerBackend::viewport_set_use_xr(RID p_viewport, bool p_use_xr)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "viewport_set_use_xr");
};

void FilamentRenderingServerBackend::viewport_set_size(RID p_viewport, int p_width, int p_height)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "viewport_set_size");
};

void FilamentRenderingServerBackend::viewport_set_active(RID p_viewport, bool p_active)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "viewport_set_active");
};

void FilamentRenderingServerBackend::viewport_set_parent_viewport(RID p_viewport, RID p_parent_viewport)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "viewport_set_parent_viewport");
};

void FilamentRenderingServerBackend::viewport_set_canvas_cull_mask(RID p_viewport, uint32_t p_canvas_cull_mask)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "viewport_set_canvas_cull_mask");
};

void FilamentRenderingServerBackend::viewport_attach_to_screen(RID p_viewport, const Rect2 & p_rect, DisplayServer::WindowID p_screen)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "viewport_attach_to_screen");
};

void FilamentRenderingServerBackend::viewport_set_render_direct_to_screen(RID p_viewport, bool p_enable)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "viewport_set_render_direct_to_screen");
};

void FilamentRenderingServerBackend::viewport_set_scaling_3d_mode(RID p_viewport, RenderingServer::ViewportScaling3DMode p_scaling_3d_mode)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "viewport_set_scaling_3d_mode");
};

void FilamentRenderingServerBackend::viewport_set_scaling_3d_scale(RID p_viewport, float p_scaling_3d_scale)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "viewport_set_scaling_3d_scale");
};

void FilamentRenderingServerBackend::viewport_set_fsr_sharpness(RID p_viewport, float p_fsr_sharpness)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "viewport_set_fsr_sharpness");
};

void FilamentRenderingServerBackend::viewport_set_texture_mipmap_bias(RID p_viewport, float p_texture_mipmap_bias)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "viewport_set_texture_mipmap_bias");
};

void FilamentRenderingServerBackend::viewport_set_update_mode(RID p_viewport, RenderingServer::ViewportUpdateMode p_mode)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "viewport_set_update_mode");
};

void FilamentRenderingServerBackend::viewport_set_clear_mode(RID p_viewport, RenderingServer::ViewportClearMode p_clear_mode)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "viewport_set_clear_mode");
};

RID FilamentRenderingServerBackend::viewport_get_render_target(RID p_viewport) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "viewport_get_render_target");
	return RID();
};

RID FilamentRenderingServerBackend::viewport_get_texture(RID p_viewport) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "viewport_get_texture");
	return RID();
};

void FilamentRenderingServerBackend::viewport_set_environment_mode(RID p_viewport, RenderingServer::ViewportEnvironmentMode p_mode)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "viewport_set_environment_mode");
};

void FilamentRenderingServerBackend::viewport_set_disable_3d(RID p_viewport, bool p_disable)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "viewport_set_disable_3d");
};

void FilamentRenderingServerBackend::viewport_set_disable_2d(RID p_viewport, bool p_disable)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "viewport_set_disable_2d");
};

void FilamentRenderingServerBackend::viewport_attach_camera(RID p_viewport, RID p_camera)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "viewport_attach_camera");
};

void FilamentRenderingServerBackend::viewport_set_scenario(RID p_viewport, RID p_scenario)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "viewport_set_scenario");
};

void FilamentRenderingServerBackend::viewport_attach_canvas(RID p_viewport, RID p_canvas)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "viewport_attach_canvas");
};

void FilamentRenderingServerBackend::viewport_remove_canvas(RID p_viewport, RID p_canvas)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "viewport_remove_canvas");
};

void FilamentRenderingServerBackend::viewport_set_canvas_transform(RID p_viewport, RID p_canvas, const Transform2D & p_offset)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "viewport_set_canvas_transform");
};

void FilamentRenderingServerBackend::viewport_set_transparent_background(RID p_viewport, bool p_enabled)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "viewport_set_transparent_background");
};

void FilamentRenderingServerBackend::viewport_set_use_hdr_2d(RID p_viewport, bool p_use_hdr)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "viewport_set_use_hdr_2d");
};

void FilamentRenderingServerBackend::viewport_set_snap_2d_transforms_to_pixel(RID p_viewport, bool p_enabled)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "viewport_set_snap_2d_transforms_to_pixel");
};

void FilamentRenderingServerBackend::viewport_set_snap_2d_vertices_to_pixel(RID p_viewport, bool p_enabled)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "viewport_set_snap_2d_vertices_to_pixel");
};

void FilamentRenderingServerBackend::viewport_set_default_canvas_item_texture_filter(RID p_viewport, RenderingServer::CanvasItemTextureFilter p_filter)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "viewport_set_default_canvas_item_texture_filter");
};

void FilamentRenderingServerBackend::viewport_set_default_canvas_item_texture_repeat(RID p_viewport, RenderingServer::CanvasItemTextureRepeat p_repeat)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "viewport_set_default_canvas_item_texture_repeat");
};

void FilamentRenderingServerBackend::viewport_set_global_canvas_transform(RID p_viewport, const Transform2D & p_transform)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "viewport_set_global_canvas_transform");
};

void FilamentRenderingServerBackend::viewport_set_canvas_stacking(RID p_viewport, RID p_canvas, int p_layer, int p_sublayer)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "viewport_set_canvas_stacking");
};

void FilamentRenderingServerBackend::viewport_set_sdf_oversize_and_scale(RID p_viewport, RenderingServer::ViewportSDFOversize p_oversize, RenderingServer::ViewportSDFScale p_scale)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "viewport_set_sdf_oversize_and_scale");
};

void FilamentRenderingServerBackend::viewport_set_positional_shadow_atlas_size(RID p_viewport, int p_size, bool p_16_bits)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "viewport_set_positional_shadow_atlas_size");
};

void FilamentRenderingServerBackend::viewport_set_positional_shadow_atlas_quadrant_subdivision(RID p_viewport, int p_quadrant, int p_subdiv)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "viewport_set_positional_shadow_atlas_quadrant_subdivision");
};

void FilamentRenderingServerBackend::viewport_set_msaa_3d(RID p_viewport, RenderingServer::ViewportMSAA p_msaa)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "viewport_set_msaa_3d");
};

void FilamentRenderingServerBackend::viewport_set_msaa_2d(RID p_viewport, RenderingServer::ViewportMSAA p_msaa)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "viewport_set_msaa_2d");
};

void FilamentRenderingServerBackend::viewport_set_screen_space_aa(RID p_viewport, RenderingServer::ViewportScreenSpaceAA p_mode)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "viewport_set_screen_space_aa");
};

void FilamentRenderingServerBackend::viewport_set_use_taa(RID p_viewport, bool p_use_taa)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "viewport_set_use_taa");
};

void FilamentRenderingServerBackend::viewport_set_use_debanding(RID p_viewport, bool p_use_debanding)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "viewport_set_use_debanding");
};

void FilamentRenderingServerBackend::viewport_set_mesh_lod_threshold(RID p_viewport, float p_pixels)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "viewport_set_mesh_lod_threshold");
};

void FilamentRenderingServerBackend::viewport_set_use_occlusion_culling(RID p_viewport, bool p_use_occlusion_culling)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "viewport_set_use_occlusion_culling");
};

void FilamentRenderingServerBackend::viewport_set_occlusion_rays_per_thread(int p_rays_per_thread)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "viewport_set_occlusion_rays_per_thread");
};

void FilamentRenderingServerBackend::viewport_set_occlusion_culling_build_quality(RenderingServer::ViewportOcclusionCullingBuildQuality p_quality)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "viewport_set_occlusion_culling_build_quality");
};

int FilamentRenderingServerBackend::viewport_get_render_info(RID p_viewport, RenderingServer::ViewportRenderInfoType p_type, RenderingServer::ViewportRenderInfo p_info)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "viewport_get_render_info");
	return int();
};

void FilamentRenderingServerBackend::viewport_set_debug_draw(RID p_viewport, RenderingServer::ViewportDebugDraw p_draw)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "viewport_set_debug_draw");
};

void FilamentRenderingServerBackend::viewport_set_measure_render_time(RID p_viewport, bool p_enable)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "viewport_set_measure_render_time");
};

double FilamentRenderingServerBackend::viewport_get_measured_render_time_cpu(RID p_viewport) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "viewport_get_measured_render_time_cpu");
	return double();
};

double FilamentRenderingServerBackend::viewport_get_measured_render_time_gpu(RID p_viewport) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "viewport_get_measured_render_time_gpu");
	return double();
};

RID FilamentRenderingServerBackend::viewport_find_from_screen_attachment(DisplayServer::WindowID p_id) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "viewport_find_from_screen_attachment");
	return RID();
};

void FilamentRenderingServerBackend::viewport_set_vrs_mode(RID p_viewport, RenderingServer::ViewportVRSMode p_mode)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "viewport_set_vrs_mode");
};

void FilamentRenderingServerBackend::viewport_set_vrs_texture(RID p_viewport, RID p_texture)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "viewport_set_vrs_texture");
};

void FilamentRenderingServerBackend::sky_create(RID output)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "sky_create");
};

void FilamentRenderingServerBackend::sky_set_radiance_size(RID p_sky, int p_radiance_size)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "sky_set_radiance_size");
};

void FilamentRenderingServerBackend::sky_set_mode(RID p_sky, RenderingServer::SkyMode p_mode)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "sky_set_mode");
};

void FilamentRenderingServerBackend::sky_set_material(RID p_sky, RID p_material)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "sky_set_material");
};

Ref<Image> FilamentRenderingServerBackend::sky_bake_panorama(RID p_sky, float p_energy, bool p_bake_irradiance, const Size2i & p_size)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "sky_bake_panorama");
	return Ref<Image>();
};

void FilamentRenderingServerBackend::environment_create(RID output)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "environment_create");
};

void FilamentRenderingServerBackend::environment_set_background(RID p_env, RenderingServer::EnvironmentBG p_bg)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "environment_set_background");
};

void FilamentRenderingServerBackend::environment_set_sky(RID p_env, RID p_sky)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "environment_set_sky");
};

void FilamentRenderingServerBackend::environment_set_sky_custom_fov(RID p_env, float p_scale)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "environment_set_sky_custom_fov");
};

void FilamentRenderingServerBackend::environment_set_sky_orientation(RID p_env, const Basis & p_orientation)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "environment_set_sky_orientation");
};

void FilamentRenderingServerBackend::environment_set_bg_color(RID p_env, const Color & p_color)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "environment_set_bg_color");
};

void FilamentRenderingServerBackend::environment_set_bg_energy(RID p_env, float p_multiplier, float p_exposure_value)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "environment_set_bg_energy");
};

void FilamentRenderingServerBackend::environment_set_canvas_max_layer(RID p_env, int p_max_layer)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "environment_set_canvas_max_layer");
};

void FilamentRenderingServerBackend::environment_set_ambient_light(RID p_env, const Color & p_color, RenderingServer::EnvironmentAmbientSource p_ambient, float p_energy, float p_sky_contribution, RenderingServer::EnvironmentReflectionSource p_reflection_source)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "environment_set_ambient_light");
};

void FilamentRenderingServerBackend::environment_set_glow(RID p_env, bool p_enable, Vector<float> p_levels, float p_intensity, float p_strength, float p_mix, float p_bloom_threshold, RenderingServer::EnvironmentGlowBlendMode p_blend_mode, float p_hdr_bleed_threshold, float p_hdr_bleed_scale, float p_hdr_luminance_cap, float p_glow_map_strength, RID p_glow_map)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "environment_set_glow");
};

void FilamentRenderingServerBackend::environment_glow_set_use_bicubic_upscale(bool p_enable)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "environment_glow_set_use_bicubic_upscale");
};

void FilamentRenderingServerBackend::environment_set_tonemap(RID p_env, RenderingServer::EnvironmentToneMapper p_tone_mapper, float p_exposure, float p_white)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "environment_set_tonemap");
};

void FilamentRenderingServerBackend::environment_set_adjustment(RID p_env, bool p_enable, float p_brightness, float p_contrast, float p_saturation, bool p_use_1d_color_correction, RID p_color_correction)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "environment_set_adjustment");
};

void FilamentRenderingServerBackend::environment_set_ssr(RID p_env, bool p_enable, int p_max_steps, float p_fade_in, float p_fade_out, float p_depth_tolerance)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "environment_set_ssr");
};

void FilamentRenderingServerBackend::environment_set_ssr_roughness_quality(RenderingServer::EnvironmentSSRRoughnessQuality p_quality)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "environment_set_ssr_roughness_quality");
};

void FilamentRenderingServerBackend::environment_set_ssao(RID p_env, bool p_enable, float p_radius, float p_intensity, float p_power, float p_detail, float p_horizon, float p_sharpness, float p_light_affect, float p_ao_channel_affect)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "environment_set_ssao");
};

void FilamentRenderingServerBackend::environment_set_ssao_quality(RenderingServer::EnvironmentSSAOQuality p_quality, bool p_half_size, float p_adaptive_target, int p_blur_passes, float p_fadeout_from, float p_fadeout_to)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "environment_set_ssao_quality");
};

void FilamentRenderingServerBackend::environment_set_ssil(RID p_env, bool p_enable, float p_radius, float p_intensity, float p_sharpness, float p_normal_rejection)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "environment_set_ssil");
};

void FilamentRenderingServerBackend::environment_set_ssil_quality(RenderingServer::EnvironmentSSILQuality p_quality, bool p_half_size, float p_adaptive_target, int p_blur_passes, float p_fadeout_from, float p_fadeout_to)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "environment_set_ssil_quality");
};

void FilamentRenderingServerBackend::environment_set_sdfgi(RID p_env, bool p_enable, int p_cascades, float p_min_cell_size, RenderingServer::EnvironmentSDFGIYScale p_y_scale, bool p_use_occlusion, float p_bounce_feedback, bool p_read_sky, float p_energy, float p_normal_bias, float p_probe_bias)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "environment_set_sdfgi");
};

void FilamentRenderingServerBackend::environment_set_sdfgi_ray_count(RenderingServer::EnvironmentSDFGIRayCount p_ray_count)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "environment_set_sdfgi_ray_count");
};

void FilamentRenderingServerBackend::environment_set_sdfgi_frames_to_converge(RenderingServer::EnvironmentSDFGIFramesToConverge p_frames)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "environment_set_sdfgi_frames_to_converge");
};

void FilamentRenderingServerBackend::environment_set_sdfgi_frames_to_update_light(RenderingServer::EnvironmentSDFGIFramesToUpdateLight p_update)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "environment_set_sdfgi_frames_to_update_light");
};

void FilamentRenderingServerBackend::environment_set_fog(RID p_env, bool p_enable, const Color & p_light_color, float p_light_energy, float p_sun_scatter, float p_density, float p_height, float p_height_density, float p_aerial_perspective, float p_sky_affect)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "environment_set_fog");
};

void FilamentRenderingServerBackend::environment_set_volumetric_fog(RID p_env, bool p_enable, float p_density, const Color & p_albedo, const Color & p_emission, float p_emission_energy, float p_anisotropy, float p_length, float p_detail_spread, float p_gi_inject, bool p_temporal_reprojection, float p_temporal_reprojection_amount, float p_ambient_inject, float p_sky_affect)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "environment_set_volumetric_fog");
};

void FilamentRenderingServerBackend::environment_set_volumetric_fog_volume_size(int p_size, int p_depth)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "environment_set_volumetric_fog_volume_size");
};

void FilamentRenderingServerBackend::environment_set_volumetric_fog_filter_active(bool p_enable)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "environment_set_volumetric_fog_filter_active");
};

Ref<Image> FilamentRenderingServerBackend::environment_bake_panorama(RID p_env, bool p_bake_irradiance, const Size2i & p_size)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "environment_bake_panorama");
	return Ref<Image>();
};

void FilamentRenderingServerBackend::screen_space_roughness_limiter_set_active(bool p_enable, float p_amount, float p_limit)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "screen_space_roughness_limiter_set_active");
};

void FilamentRenderingServerBackend::sub_surface_scattering_set_quality(RenderingServer::SubSurfaceScatteringQuality p_quality)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "sub_surface_scattering_set_quality");
};

void FilamentRenderingServerBackend::sub_surface_scattering_set_scale(float p_scale, float p_depth_scale)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "sub_surface_scattering_set_scale");
};

void FilamentRenderingServerBackend::camera_attributes_create(RID output)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "camera_attributes_create");
};

void FilamentRenderingServerBackend::camera_attributes_set_dof_blur_quality(RenderingServer::DOFBlurQuality p_quality, bool p_use_jitter)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "camera_attributes_set_dof_blur_quality");
};

void FilamentRenderingServerBackend::camera_attributes_set_dof_blur_bokeh_shape(RenderingServer::DOFBokehShape p_shape)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "camera_attributes_set_dof_blur_bokeh_shape");
};

void FilamentRenderingServerBackend::camera_attributes_set_dof_blur(RID p_camera_attributes, bool p_far_enable, float p_far_distance, float p_far_transition, bool p_near_enable, float p_near_distance, float p_near_transition, float p_amount)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "camera_attributes_set_dof_blur");
};

void FilamentRenderingServerBackend::camera_attributes_set_exposure(RID p_camera_attributes, float p_multiplier, float p_exposure_normalization)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "camera_attributes_set_exposure");
};

void FilamentRenderingServerBackend::camera_attributes_set_auto_exposure(RID p_camera_attributes, bool p_enable, float p_min_sensitivity, float p_max_sensitivity, float p_speed, float p_scale)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "camera_attributes_set_auto_exposure");
};

void FilamentRenderingServerBackend::scenario_create(RID output)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "scenario_create");
};

void FilamentRenderingServerBackend::scenario_set_environment(RID p_scenario, RID p_environment)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "scenario_set_environment");
};

void FilamentRenderingServerBackend::scenario_set_fallback_environment(RID p_scenario, RID p_environment)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "scenario_set_fallback_environment");
};

void FilamentRenderingServerBackend::scenario_set_camera_attributes(RID p_scenario, RID p_camera_attributes)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "scenario_set_camera_attributes");
};

void FilamentRenderingServerBackend::instance_create(RID output)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "instance_create");
};

void FilamentRenderingServerBackend::instance_set_base(RID p_instance, RID p_base)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "instance_set_base");
};

void FilamentRenderingServerBackend::instance_set_scenario(RID p_instance, RID p_scenario)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "instance_set_scenario");
};

void FilamentRenderingServerBackend::instance_set_layer_mask(RID p_instance, uint32_t p_mask)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "instance_set_layer_mask");
};

void FilamentRenderingServerBackend::instance_set_pivot_data(RID p_instance, float p_sorting_offset, bool p_use_aabb_center)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "instance_set_pivot_data");
};

void FilamentRenderingServerBackend::instance_set_transform(RID p_instance, const Transform3D & p_transform)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "instance_set_transform");
};

void FilamentRenderingServerBackend::instance_attach_object_instance_id(RID p_instance, ObjectID p_id)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "instance_attach_object_instance_id");
};

void FilamentRenderingServerBackend::instance_set_blend_shape_weight(RID p_instance, int p_shape, float p_weight)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "instance_set_blend_shape_weight");
};

void FilamentRenderingServerBackend::instance_set_surface_override_material(RID p_instance, int p_surface, RID p_material)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "instance_set_surface_override_material");
};

void FilamentRenderingServerBackend::instance_set_visible(RID p_instance, bool p_visible)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "instance_set_visible");
};

void FilamentRenderingServerBackend::instance_set_custom_aabb(RID p_instance, AABB aabb)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "instance_set_custom_aabb");
};

void FilamentRenderingServerBackend::instance_attach_skeleton(RID p_instance, RID p_skeleton)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "instance_attach_skeleton");
};

void FilamentRenderingServerBackend::instance_set_extra_visibility_margin(RID p_instance, real_t p_margin)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "instance_set_extra_visibility_margin");
};

void FilamentRenderingServerBackend::instance_set_visibility_parent(RID p_instance, RID p_parent_instance)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "instance_set_visibility_parent");
};

void FilamentRenderingServerBackend::instance_set_ignore_culling(RID p_instance, bool p_enabled)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "instance_set_ignore_culling");
};

Vector<ObjectID> FilamentRenderingServerBackend::instances_cull_aabb(const AABB & p_aabb, RID p_scenario) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "instances_cull_aabb");
	return Vector<ObjectID>();
};

Vector<ObjectID> FilamentRenderingServerBackend::instances_cull_ray(const Vector3 & p_from, const Vector3 & p_to, RID p_scenario) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "instances_cull_ray");
	return Vector<ObjectID>();
};

Vector<ObjectID> FilamentRenderingServerBackend::instances_cull_convex(const Vector<Plane> & p_convex, RID p_scenario) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "instances_cull_convex");
	return Vector<ObjectID>();
};

void FilamentRenderingServerBackend::instance_geometry_set_flag(RID p_instance, RenderingServer::InstanceFlags p_flags, bool p_enabled)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "instance_geometry_set_flag");
};

void FilamentRenderingServerBackend::instance_geometry_set_cast_shadows_setting(RID p_instance, RenderingServer::ShadowCastingSetting p_shadow_casting_setting)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "instance_geometry_set_cast_shadows_setting");
};

void FilamentRenderingServerBackend::instance_geometry_set_material_override(RID p_instance, RID p_material)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "instance_geometry_set_material_override");
};

void FilamentRenderingServerBackend::instance_geometry_set_material_overlay(RID p_instance, RID p_material)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "instance_geometry_set_material_overlay");
};

void FilamentRenderingServerBackend::instance_geometry_set_visibility_range(RID p_instance, float p_min, float p_max, float p_min_margin, float p_max_margin, RenderingServer::VisibilityRangeFadeMode p_fade_mode)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "instance_geometry_set_visibility_range");
};

void FilamentRenderingServerBackend::instance_geometry_set_lightmap(RID p_instance, RID p_lightmap, const Rect2 & p_lightmap_uv_scale, int p_lightmap_slice)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "instance_geometry_set_lightmap");
};

void FilamentRenderingServerBackend::instance_geometry_set_lod_bias(RID p_instance, float p_lod_bias)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "instance_geometry_set_lod_bias");
};

void FilamentRenderingServerBackend::instance_geometry_set_transparency(RID p_instance, float p_transparency)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "instance_geometry_set_transparency");
};

void FilamentRenderingServerBackend::instance_geometry_set_shader_parameter(RID p_instance, const StringName & anonarg, const Variant & p_value)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "instance_geometry_set_shader_parameter");
};

Variant FilamentRenderingServerBackend::instance_geometry_get_shader_parameter(RID p_instance, const StringName & anonarg) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "instance_geometry_get_shader_parameter");
	return Variant();
};

Variant FilamentRenderingServerBackend::instance_geometry_get_shader_parameter_default_value(RID p_instance, const StringName & anonarg) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "instance_geometry_get_shader_parameter_default_value");
	return Variant();
};

void FilamentRenderingServerBackend::instance_geometry_get_shader_parameter_list(RID p_instance, List<PropertyInfo> * p_parameters) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "instance_geometry_get_shader_parameter_list");
};

TypedArray<Image> FilamentRenderingServerBackend::bake_render_uv2(RID p_base, const TypedArray<RID> & p_material_overrides, const Size2i & p_image_size)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "bake_render_uv2");
	return TypedArray<Image>();
};

void FilamentRenderingServerBackend::canvas_create(RID output)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_create");
};

void FilamentRenderingServerBackend::canvas_set_item_mirroring(RID p_canvas, RID p_item, const Point2 & p_mirroring)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_set_item_mirroring");
};

void FilamentRenderingServerBackend::canvas_set_modulate(RID p_canvas, const Color & p_color)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_set_modulate");
};

void FilamentRenderingServerBackend::canvas_set_parent(RID p_canvas, RID p_parent, float p_scale)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_set_parent");
};

void FilamentRenderingServerBackend::canvas_set_disable_scale(bool p_disable)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_set_disable_scale");
};

void FilamentRenderingServerBackend::canvas_texture_create(RID output)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_texture_create");
};

void FilamentRenderingServerBackend::canvas_texture_set_channel(RID p_canvas_texture, RenderingServer::CanvasTextureChannel p_channel, RID p_texture)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_texture_set_channel");
};

void FilamentRenderingServerBackend::canvas_texture_set_shading_parameters(RID p_canvas_texture, const Color & p_base_color, float p_shininess)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_texture_set_shading_parameters");
};

void FilamentRenderingServerBackend::canvas_texture_set_texture_filter(RID p_canvas_texture, RenderingServer::CanvasItemTextureFilter p_filter)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_texture_set_texture_filter");
};

void FilamentRenderingServerBackend::canvas_texture_set_texture_repeat(RID p_canvas_texture, RenderingServer::CanvasItemTextureRepeat p_repeat)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_texture_set_texture_repeat");
};

void FilamentRenderingServerBackend::canvas_item_create(RID output)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_item_create");
}

void FilamentRenderingServerBackend::canvas_item_set_parent(RID p_item, RID p_parent)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_item_set_parent");
};

void FilamentRenderingServerBackend::canvas_item_set_default_texture_filter(RID p_item, RenderingServer::CanvasItemTextureFilter p_filter)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_item_set_default_texture_filter");
};

void FilamentRenderingServerBackend::canvas_item_set_default_texture_repeat(RID p_item, RenderingServer::CanvasItemTextureRepeat p_repeat)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_item_set_default_texture_repeat");
};

void FilamentRenderingServerBackend::canvas_item_set_visible(RID p_item, bool p_visible)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_item_set_visible");
};

void FilamentRenderingServerBackend::canvas_item_set_light_mask(RID p_item, int p_mask)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_item_set_light_mask");
};

void FilamentRenderingServerBackend::canvas_item_set_update_when_visible(RID p_item, bool p_update)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_item_set_update_when_visible");
};

void FilamentRenderingServerBackend::canvas_item_set_transform(RID p_item, const Transform2D & p_transform)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_item_set_transform");
};

void FilamentRenderingServerBackend::canvas_item_set_clip(RID p_item, bool p_clip)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_item_set_clip");
};

void FilamentRenderingServerBackend::canvas_item_set_distance_field_mode(RID p_item, bool p_enable)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_item_set_distance_field_mode");
};

void FilamentRenderingServerBackend::canvas_item_set_custom_rect(RID p_item, bool p_custom_rect, const Rect2 & p_rect)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_item_set_custom_rect");
};

void FilamentRenderingServerBackend::canvas_item_set_modulate(RID p_item, const Color & p_color)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_item_set_modulate");
};

void FilamentRenderingServerBackend::canvas_item_set_self_modulate(RID p_item, const Color & p_color)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_item_set_self_modulate");
};

void FilamentRenderingServerBackend::canvas_item_set_visibility_layer(RID p_item, uint32_t p_visibility_layer)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_item_set_visibility_layer");
};

void FilamentRenderingServerBackend::canvas_item_set_draw_behind_parent(RID p_item, bool p_enable)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_item_set_draw_behind_parent");
};

void FilamentRenderingServerBackend::canvas_item_add_line(RID p_item, const Point2 & p_from, const Point2 & p_to, const Color & p_color, float p_width, bool p_antialiased)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_item_add_line");
};

void FilamentRenderingServerBackend::canvas_item_add_polyline(RID p_item, const Vector<Point2> & p_points, const Vector<Color> & p_colors, float p_width, bool p_antialiased)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_item_add_polyline");
};

void FilamentRenderingServerBackend::canvas_item_add_multiline(RID p_item, const Vector<Point2> & p_points, const Vector<Color> & p_colors, float p_width)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_item_add_multiline");
};

void FilamentRenderingServerBackend::canvas_item_add_rect(RID p_item, const Rect2 & p_rect, const Color & p_color)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_item_add_rect");
};

void FilamentRenderingServerBackend::canvas_item_add_circle(RID p_item, const Point2 & p_pos, float p_radius, const Color & p_color)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_item_add_circle");
};

void FilamentRenderingServerBackend::canvas_item_add_texture_rect(RID p_item, const Rect2 & p_rect, RID p_texture, bool p_tile, const Color & p_modulate, bool p_transpose)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_item_add_texture_rect");
};

void FilamentRenderingServerBackend::canvas_item_add_texture_rect_region(RID p_item, const Rect2 & p_rect, RID p_texture, const Rect2 & p_src_rect, const Color & p_modulate, bool p_transpose, bool p_clip_uv)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_item_add_texture_rect_region");
};

void FilamentRenderingServerBackend::canvas_item_add_msdf_texture_rect_region(RID p_item, const Rect2 & p_rect, RID p_texture, const Rect2 & p_src_rect, const Color & p_modulate, int p_outline_size, float p_px_range, float p_scale)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_item_add_msdf_texture_rect_region");
};

void FilamentRenderingServerBackend::canvas_item_add_lcd_texture_rect_region(RID p_item, const Rect2 & p_rect, RID p_texture, const Rect2 & p_src_rect, const Color & p_modulate)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_item_add_lcd_texture_rect_region");
};

void FilamentRenderingServerBackend::canvas_item_add_nine_patch(RID p_item, const Rect2 & p_rect, const Rect2 & p_source, RID p_texture, const Vector2 & p_topleft, const Vector2 & p_bottomright, RenderingServer::NinePatchAxisMode p_x_axis_mode, RenderingServer::NinePatchAxisMode p_y_axis_mode, bool p_draw_center, const Color & p_modulate)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_item_add_nine_patch");
};

void FilamentRenderingServerBackend::canvas_item_add_primitive(RID p_item, const Vector<Point2> & p_points, const Vector<Color> & p_colors, const Vector<Point2> & p_uvs, RID p_texture)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_item_add_primitive");
};

void FilamentRenderingServerBackend::canvas_item_add_polygon(RID p_item, const Vector<Point2> & p_points, const Vector<Color> & p_colors, const Vector<Point2> & p_uvs, RID p_texture)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_item_add_polygon");
};

void FilamentRenderingServerBackend::canvas_item_add_triangle_array(RID p_item, const Vector<int> & p_indices, const Vector<Point2> & p_points, const Vector<Color> & p_colors, const Vector<Point2> & p_uvs, const Vector<int> & p_bones, const Vector<float> & p_weights, RID p_texture, int p_count)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_item_add_triangle_array");
};

void FilamentRenderingServerBackend::canvas_item_add_mesh(RID p_item, const RID & p_mesh, const Transform2D & p_transform, const Color & p_modulate, RID p_texture)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_item_add_mesh");
};

void FilamentRenderingServerBackend::canvas_item_add_multimesh(RID p_item, RID p_mesh, RID p_texture)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_item_add_multimesh");
};

void FilamentRenderingServerBackend::canvas_item_add_particles(RID p_item, RID p_particles, RID p_texture)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_item_add_particles");
};

void FilamentRenderingServerBackend::canvas_item_add_set_transform(RID p_item, const Transform2D & p_transform)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_item_add_set_transform");
};

void FilamentRenderingServerBackend::canvas_item_add_clip_ignore(RID p_item, bool p_ignore)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_item_add_clip_ignore");
};

void FilamentRenderingServerBackend::canvas_item_add_animation_slice(RID p_item, double p_animation_length, double p_slice_begin, double p_slice_end, double p_offset)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_item_add_animation_slice");
};

void FilamentRenderingServerBackend::canvas_item_set_sort_children_by_y(RID p_item, bool p_enable)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_item_set_sort_children_by_y");
};

void FilamentRenderingServerBackend::canvas_item_set_z_index(RID p_item, int p_z)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_item_set_z_index");
};

void FilamentRenderingServerBackend::canvas_item_set_z_as_relative_to_parent(RID p_item, bool p_enable)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_item_set_z_as_relative_to_parent");
};

void FilamentRenderingServerBackend::canvas_item_set_copy_to_backbuffer(RID p_item, bool p_enable, const Rect2 & p_rect)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_item_set_copy_to_backbuffer");
};

void FilamentRenderingServerBackend::canvas_item_attach_skeleton(RID p_item, RID p_skeleton)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_item_attach_skeleton");
};

void FilamentRenderingServerBackend::canvas_item_clear(RID p_item)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_item_clear");
};

void FilamentRenderingServerBackend::canvas_item_set_draw_index(RID p_item, int p_index)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_item_set_draw_index");
};

void FilamentRenderingServerBackend::canvas_item_set_material(RID p_item, RID p_material)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_item_set_material");
};

void FilamentRenderingServerBackend::canvas_item_set_use_parent_material(RID p_item, bool p_enable)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_item_set_use_parent_material");
};

void FilamentRenderingServerBackend::canvas_item_set_visibility_notifier(RID p_item, bool p_enable, const Rect2 & p_area, const Callable & p_enter_callbable, const Callable & p_exit_callable)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_item_set_visibility_notifier");
};

void FilamentRenderingServerBackend::canvas_item_set_canvas_group_mode(RID p_item, RenderingServer::CanvasGroupMode p_mode, float p_clear_margin, bool p_fit_empty, float p_fit_margin, bool p_blur_mipmaps)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_item_set_canvas_group_mode");
};

void FilamentRenderingServerBackend::canvas_item_set_debug_redraw(bool p_enabled)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_item_set_debug_redraw");
};

bool FilamentRenderingServerBackend::canvas_item_get_debug_redraw() const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_item_get_debug_redraw");
	return bool();
};

void FilamentRenderingServerBackend::canvas_light_create(RID output)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_light_create");
};

void FilamentRenderingServerBackend::canvas_light_set_mode(RID p_light, RenderingServer::CanvasLightMode p_mode)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_light_set_mode");
};

void FilamentRenderingServerBackend::canvas_light_attach_to_canvas(RID p_light, RID p_canvas)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_light_attach_to_canvas");
};

void FilamentRenderingServerBackend::canvas_light_set_enabled(RID p_light, bool p_enabled)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_light_set_enabled");
};

void FilamentRenderingServerBackend::canvas_light_set_transform(RID p_light, const Transform2D & p_transform)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_light_set_transform");
};

void FilamentRenderingServerBackend::canvas_light_set_color(RID p_light, const Color & p_color)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_light_set_color");
};

void FilamentRenderingServerBackend::canvas_light_set_height(RID p_light, float p_height)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_light_set_height");
};

void FilamentRenderingServerBackend::canvas_light_set_energy(RID p_light, float p_energy)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_light_set_energy");
};

void FilamentRenderingServerBackend::canvas_light_set_z_range(RID p_light, int p_min_z, int p_max_z)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_light_set_z_range");
};

void FilamentRenderingServerBackend::canvas_light_set_layer_range(RID p_light, int p_min_layer, int p_max_layer)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_light_set_layer_range");
};

void FilamentRenderingServerBackend::canvas_light_set_item_cull_mask(RID p_light, int p_mask)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_light_set_item_cull_mask");
};

void FilamentRenderingServerBackend::canvas_light_set_item_shadow_cull_mask(RID p_light, int p_mask)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_light_set_item_shadow_cull_mask");
};

void FilamentRenderingServerBackend::canvas_light_set_directional_distance(RID p_light, float p_distance)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_light_set_directional_distance");
};

void FilamentRenderingServerBackend::canvas_light_set_texture_scale(RID p_light, float p_scale)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_light_set_texture_scale");
};

void FilamentRenderingServerBackend::canvas_light_set_texture(RID p_light, RID p_texture)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_light_set_texture");
};

void FilamentRenderingServerBackend::canvas_light_set_texture_offset(RID p_light, const Vector2 & p_offset)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_light_set_texture_offset");
};

void FilamentRenderingServerBackend::canvas_light_set_blend_mode(RID p_light, RenderingServer::CanvasLightBlendMode p_mode)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_light_set_blend_mode");
};

void FilamentRenderingServerBackend::canvas_light_set_shadow_enabled(RID p_light, bool p_enabled)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_light_set_shadow_enabled");
};

void FilamentRenderingServerBackend::canvas_light_set_shadow_filter(RID p_light, RenderingServer::CanvasLightShadowFilter p_filter)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_light_set_shadow_filter");
};

void FilamentRenderingServerBackend::canvas_light_set_shadow_color(RID p_light, const Color & p_color)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_light_set_shadow_color");
};

void FilamentRenderingServerBackend::canvas_light_set_shadow_smooth(RID p_light, float p_smooth)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_light_set_shadow_smooth");
};

void FilamentRenderingServerBackend::canvas_light_occluder_create(RID output)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_light_occluder_create");
};

void FilamentRenderingServerBackend::canvas_light_occluder_attach_to_canvas(RID p_occluder, RID p_canvas)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_light_occluder_attach_to_canvas");
};

void FilamentRenderingServerBackend::canvas_light_occluder_set_enabled(RID p_occluder, bool p_enabled)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_light_occluder_set_enabled");
};

void FilamentRenderingServerBackend::canvas_light_occluder_set_polygon(RID p_occluder, RID p_polygon)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_light_occluder_set_polygon");
};

void FilamentRenderingServerBackend::canvas_light_occluder_set_as_sdf_collision(RID p_occluder, bool p_enable)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_light_occluder_set_as_sdf_collision");
};

void FilamentRenderingServerBackend::canvas_light_occluder_set_transform(RID p_occluder, const Transform2D & p_xform)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_light_occluder_set_transform");
};

void FilamentRenderingServerBackend::canvas_light_occluder_set_light_mask(RID p_occluder, int p_mask)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_light_occluder_set_light_mask");
};

void FilamentRenderingServerBackend::canvas_occluder_polygon_create(RID output)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_occluder_polygon_create");
};

void FilamentRenderingServerBackend::canvas_occluder_polygon_set_shape(RID p_occluder_polygon, const Vector<Vector2> & p_shape, bool p_closed)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_occluder_polygon_set_shape");
};

void FilamentRenderingServerBackend::canvas_occluder_polygon_set_cull_mode(RID p_occluder_polygon, RenderingServer::CanvasOccluderPolygonCullMode p_mode)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_occluder_polygon_set_cull_mode");
};

void FilamentRenderingServerBackend::canvas_set_shadow_texture_size(int p_size)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "canvas_set_shadow_texture_size");
};

void FilamentRenderingServerBackend::global_shader_parameter_add(const StringName & p_name, RenderingServer::GlobalShaderParameterType p_type, const Variant & p_value)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "global_shader_parameter_add");
};

void FilamentRenderingServerBackend::global_shader_parameter_remove(const StringName & p_name)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "global_shader_parameter_remove");
};

Vector<StringName> FilamentRenderingServerBackend::global_shader_parameter_get_list() const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "global_shader_parameter_get_list");
	return Vector<StringName>();
};

void FilamentRenderingServerBackend::global_shader_parameter_set(const StringName & p_name, const Variant & p_value)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "global_shader_parameter_set");
};

void FilamentRenderingServerBackend::global_shader_parameter_set_override(const StringName & p_name, const Variant & p_value)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "global_shader_parameter_set_override");
};

Variant FilamentRenderingServerBackend::global_shader_parameter_get(const StringName & p_name) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "global_shader_parameter_get");
	return Variant();
};

RenderingServer::GlobalShaderParameterType FilamentRenderingServerBackend::global_shader_parameter_get_type(const StringName & p_name) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "global_shader_parameter_get_type");
	return RenderingServer::GlobalShaderParameterType();
};

void FilamentRenderingServerBackend::global_shader_parameters_load_settings(bool p_load_textures)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "global_shader_parameters_load_settings");
};

void FilamentRenderingServerBackend::global_shader_parameters_clear()  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "global_shader_parameters_clear");
};

void FilamentRenderingServerBackend::free(RID p_rid)  {
	m_objectManager.free(p_rid);
};

void FilamentRenderingServerBackend::request_frame_drawn_callback(const Callable & p_callable)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "request_frame_drawn_callback");
};

void FilamentRenderingServerBackend::draw(bool p_swap_buffers, double frame_step)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "draw");
};

void FilamentRenderingServerBackend::sync()  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "sync");
};

bool FilamentRenderingServerBackend::has_changed() const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "has_changed");
	return bool();
};

uint64_t FilamentRenderingServerBackend::get_rendering_info(RenderingServer::RenderingInfo p_info)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "get_rendering_info");
	return uint64_t();
};

String FilamentRenderingServerBackend::get_video_adapter_name() const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "get_video_adapter_name");
	return String();
};

String FilamentRenderingServerBackend::get_video_adapter_vendor() const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "get_video_adapter_vendor");
	return String();
};

RenderingDevice::DeviceType FilamentRenderingServerBackend::get_video_adapter_type() const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "get_video_adapter_type");
	return RenderingDevice::DeviceType();
};

String FilamentRenderingServerBackend::get_video_adapter_api_version() const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "get_video_adapter_api_version");
	return String();
};

void FilamentRenderingServerBackend::set_frame_profiling_enabled(bool p_enable)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "set_frame_profiling_enabled");
};

Vector<RenderingServer::FrameProfileArea> FilamentRenderingServerBackend::get_frame_profile()  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "get_frame_profile");
	return Vector<RenderingServer::FrameProfileArea>();
};

uint64_t FilamentRenderingServerBackend::get_frame_profile_frame()  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "get_frame_profile_frame");
	return uint64_t();
};

double FilamentRenderingServerBackend::get_frame_setup_time_cpu() const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "get_frame_setup_time_cpu");
	return double();
};

void FilamentRenderingServerBackend::gi_set_use_half_resolution(bool p_enable)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "gi_set_use_half_resolution");
};

RID FilamentRenderingServerBackend::get_test_cube()  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "get_test_cube");
	return RID();
};

void FilamentRenderingServerBackend::sdfgi_set_debug_probe_select(const Vector3 & p_position, const Vector3 & p_dir)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "sdfgi_set_debug_probe_select");
};

void FilamentRenderingServerBackend::set_boot_image(const Ref<Image> & p_image, const Color & p_color, bool p_scale, bool p_use_filter)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "set_boot_image");
};

Color FilamentRenderingServerBackend::get_default_clear_color()  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "get_default_clear_color");
	return Color();
};

void FilamentRenderingServerBackend::set_default_clear_color(const Color & p_color)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "set_default_clear_color");
};

bool FilamentRenderingServerBackend::has_feature(RenderingServer::Features p_feature) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "has_feature");
	return bool();
};

bool FilamentRenderingServerBackend::has_os_feature(const String & p_feature) const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "has_os_feature");
	return bool();
};

void FilamentRenderingServerBackend::set_debug_generate_wireframes(bool p_generate)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "set_debug_generate_wireframes");
};

void FilamentRenderingServerBackend::call_set_vsync_mode(DisplayServer::VSyncMode p_mode, DisplayServer::WindowID p_window)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "call_set_vsync_mode");
};

bool FilamentRenderingServerBackend::is_low_end() const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "is_low_end");
	return bool();
};

void FilamentRenderingServerBackend::set_print_gpu_profile(bool p_enable)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "set_print_gpu_profile");
};

Size2i FilamentRenderingServerBackend::get_maximum_viewport_size() const {
	printf("FilamentRenderingServerBackend::%s stub!\n", "get_maximum_viewport_size");
	return Size2i();
};

void FilamentRenderingServerBackend::call_on_render_thread(const Callable & p_callable)  {
	printf("FilamentRenderingServerBackend::%s stub!\n", "call_on_render_thread");
};

Error FilamentRenderingServerBackend::display_server_initialize() {
	printf("FilamentRenderingServerBackend::display_server_initialize\n");

	return OK;
}

Error FilamentRenderingServerBackend::window_create(DisplayServer::WindowID p_window_id, void *p_native_window) {
	auto neededSize = static_cast<size_t>(p_window_id) + 1;
	if(m_windows.size() < neededSize) {
		m_windows.resize(neededSize);
	}

	m_windows[p_window_id] = std::make_unique<FilamentWindow>(p_native_window);

	return OK;
}

void FilamentRenderingServerBackend::window_destroy(DisplayServer::WindowID p_window_id) {
	if(m_windows.size() <= p_window_id) {
		return;
	}

	m_windows[p_window_id].reset();
}
