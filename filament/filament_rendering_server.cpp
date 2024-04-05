#include "filament_rendering_server.h"
#include "filament_object_manager.h"

FilamentRenderingServer *FilamentRenderingServer::Registerer::m_instance = nullptr;

FilamentRenderingServer::FilamentRenderingServer() : m_registerer(this) {

}

FilamentRenderingServer::~FilamentRenderingServer() = default;

void FilamentRenderingServer::_bind_methods() {
	ClassDB::bind_method(D_METHOD("shader_set_package", "shader", "package"), &FilamentRenderingServer::shader_set_package);
}

FilamentRenderingServer::Registerer::Registerer(FilamentRenderingServer *server) {
	m_instance = server;
}

FilamentRenderingServer::Registerer::~Registerer() {
	m_instance = nullptr;
}

RID FilamentRenderingServer::texture_2d_create(const Ref<Image> & p_image)  {
	auto output = FilamentObjectManager::allocate();

	execute<void, RID, const Ref<Image> &>(&FilamentRenderingServerBackend::texture_2d_create, output, p_image);

	return output;
};

RID FilamentRenderingServer::texture_2d_layered_create(const Vector<Ref<Image>> & p_layers, TextureLayeredType p_layered_type)  {
	auto output = FilamentObjectManager::allocate();

	execute<void, RID, const Vector<Ref<Image>> &, TextureLayeredType>(&FilamentRenderingServerBackend::texture_2d_layered_create, output, p_layers, p_layered_type);

	return output;
};

RID FilamentRenderingServer::texture_3d_create(Image::Format anonarg, int p_width, int p_height, int p_depth, bool p_mipmaps, const Vector<Ref<Image>> & p_data)  {
	auto output = FilamentObjectManager::allocate();

	execute<void, RID, Image::Format, int, int, int, bool, const Vector<Ref<Image>> &>(&FilamentRenderingServerBackend::texture_3d_create, output,
																							  anonarg, p_width, p_height, p_depth, p_mipmaps, p_data);

	return output;
};

RID FilamentRenderingServer::texture_proxy_create(RID p_base)  {
	auto output = FilamentObjectManager::allocate();

	execute<void, RID, RID>(&FilamentRenderingServerBackend::texture_proxy_create, output, p_base);

	return output;
};

void FilamentRenderingServer::texture_2d_update(RID p_texture, const Ref<Image> & p_image, int p_layer)  {
	return execute<void, RID, const Ref<Image> &, int>(&FilamentRenderingServerBackend::texture_2d_update, p_texture, p_image, p_layer);
};

void FilamentRenderingServer::texture_3d_update(RID p_texture, const Vector<Ref<Image>> & p_data)  {
	return execute<void, RID, const Vector<Ref<Image>> &>(&FilamentRenderingServerBackend::texture_3d_update, p_texture, p_data);
};

void FilamentRenderingServer::texture_proxy_update(RID p_texture, RID p_proxy_to)  {
	return execute<void, RID, RID>(&FilamentRenderingServerBackend::texture_proxy_update, p_texture, p_proxy_to);
};

RID FilamentRenderingServer::texture_2d_placeholder_create()  {
	auto output = FilamentObjectManager::allocate();

	execute<void, RID>(&FilamentRenderingServerBackend::texture_2d_placeholder_create, output);

	return output;
};

RID FilamentRenderingServer::texture_2d_layered_placeholder_create(TextureLayeredType p_layered_type)  {
	auto output = FilamentObjectManager::allocate();

	execute<void, RID, TextureLayeredType>(&FilamentRenderingServerBackend::texture_2d_layered_placeholder_create, output, p_layered_type);

	return output;
};

RID FilamentRenderingServer::texture_3d_placeholder_create()  {
	auto output = FilamentObjectManager::allocate();

	execute<void, RID>(&FilamentRenderingServerBackend::texture_3d_placeholder_create, output);

	return output;
};

Ref<Image> FilamentRenderingServer::texture_2d_get(RID p_texture) const {
	return execute<Ref<Image>, RID>(&FilamentRenderingServerBackend::texture_2d_get, p_texture);
};

Ref<Image> FilamentRenderingServer::texture_2d_layer_get(RID p_texture, int p_layer) const {
	return execute<Ref<Image>, RID, int>(&FilamentRenderingServerBackend::texture_2d_layer_get, p_texture, p_layer);
};

Vector<Ref<Image>> FilamentRenderingServer::texture_3d_get(RID p_texture) const {
	return execute<Vector<Ref<Image>>, RID>(&FilamentRenderingServerBackend::texture_3d_get, p_texture);
};

void FilamentRenderingServer::texture_replace(RID p_texture, RID p_by_texture)  {
	return execute<void, RID, RID>(&FilamentRenderingServerBackend::texture_replace, p_texture, p_by_texture);
};

void FilamentRenderingServer::texture_set_size_override(RID p_texture, int p_width, int p_height)  {
	return execute<void, RID, int, int>(&FilamentRenderingServerBackend::texture_set_size_override, p_texture, p_width, p_height);
};

void FilamentRenderingServer::texture_set_path(RID p_texture, const String & p_path)  {
	return execute<void, RID, const String &>(&FilamentRenderingServerBackend::texture_set_path, p_texture, p_path);
};

String FilamentRenderingServer::texture_get_path(RID p_texture) const {
	return execute<String, RID>(&FilamentRenderingServerBackend::texture_get_path, p_texture);
};

Image::Format FilamentRenderingServer::texture_get_format(RID p_texture) const {
	return execute<Image::Format, RID>(&FilamentRenderingServerBackend::texture_get_format, p_texture);
};

void FilamentRenderingServer::texture_set_detect_3d_callback(RID p_texture, TextureDetectCallback p_callback, void * p_userdata)  {
	return execute<void, RID, TextureDetectCallback, void *>(&FilamentRenderingServerBackend::texture_set_detect_3d_callback, p_texture, p_callback, p_userdata);
};

void FilamentRenderingServer::texture_set_detect_normal_callback(RID p_texture, TextureDetectCallback p_callback, void * p_userdata)  {
	return execute<void, RID, TextureDetectCallback, void *>(&FilamentRenderingServerBackend::texture_set_detect_normal_callback, p_texture, p_callback, p_userdata);
};

void FilamentRenderingServer::texture_set_detect_roughness_callback(RID p_texture, TextureDetectRoughnessCallback p_callback, void * p_userdata)  {
	return execute<void, RID, TextureDetectRoughnessCallback, void *>(&FilamentRenderingServerBackend::texture_set_detect_roughness_callback, p_texture, p_callback, p_userdata);
};

void FilamentRenderingServer::texture_debug_usage(List<TextureInfo> * r_info)  {
	return execute<void, List<TextureInfo> *>(&FilamentRenderingServerBackend::texture_debug_usage, r_info);
};

void FilamentRenderingServer::texture_set_force_redraw_if_visible(RID p_texture, bool p_enable)  {
	return execute<void, RID, bool>(&FilamentRenderingServerBackend::texture_set_force_redraw_if_visible, p_texture, p_enable);
};

RID FilamentRenderingServer::texture_rd_create(const RID & p_rd_texture, const RenderingServer::TextureLayeredType p_layer_type)  {
	auto output = FilamentObjectManager::allocate();

	execute<void, RID, const RID &, const RenderingServer::TextureLayeredType>(&FilamentRenderingServerBackend::texture_rd_create, p_rd_texture, output, p_layer_type);

	return output;
};

RID FilamentRenderingServer::texture_get_rd_texture(RID p_texture, bool p_srgb) const {
	return execute<RID, RID, bool>(&FilamentRenderingServerBackend::texture_get_rd_texture, p_texture, p_srgb);
};

uint64_t FilamentRenderingServer::texture_get_native_handle(RID p_texture, bool p_srgb) const {
	return execute<uint64_t, RID, bool>(&FilamentRenderingServerBackend::texture_get_native_handle, p_texture, p_srgb);
};

RID FilamentRenderingServer::shader_create()  {
	auto output = FilamentObjectManager::allocate();

	execute<void, RID>(&FilamentRenderingServerBackend::shader_create, output);

	return output;
};

void FilamentRenderingServer::shader_set_code(RID p_shader, const String & p_code)  {
	return execute<void, RID, const String &>(&FilamentRenderingServerBackend::shader_set_code, p_shader, p_code);
};

void FilamentRenderingServer::shader_set_package(RID p_shader, const Vector<uint8_t> &p_package) {
	return execute<void, RID, const Vector<uint8_t> &>(&FilamentRenderingServerBackend::shader_set_package, p_shader, p_package);
}

String FilamentRenderingServer::shader_get_name(RID p_shader) const {
	return execute<String, RID>(&FilamentRenderingServerBackend::shader_get_name, p_shader);
}

void FilamentRenderingServer::shader_set_path_hint(RID p_shader, const String & p_path)  {
	return execute<void, RID, const String &>(&FilamentRenderingServerBackend::shader_set_path_hint, p_shader, p_path);
};

String FilamentRenderingServer::shader_get_code(RID p_shader) const {
	return execute<String, RID>(&FilamentRenderingServerBackend::shader_get_code, p_shader);
};

void FilamentRenderingServer::get_shader_parameter_list(RID p_shader, List<PropertyInfo> * p_param_list) const {
	execute<bool, RID, List<PropertyInfo> *>(&FilamentRenderingServerBackend::get_shader_parameter_list, p_shader, p_param_list);
};

Variant FilamentRenderingServer::shader_get_parameter_default(RID p_shader, const StringName & p_param) const {
	return execute<Variant, RID, const StringName &>(&FilamentRenderingServerBackend::shader_get_parameter_default, p_shader, p_param);
};

void FilamentRenderingServer::shader_set_default_texture_parameter(RID p_shader, const StringName & p_name, RID p_texture, int p_index)  {
	return execute<void, RID, const StringName &, RID, int>(&FilamentRenderingServerBackend::shader_set_default_texture_parameter, p_shader, p_name, p_texture, p_index);
};

RID FilamentRenderingServer::shader_get_default_texture_parameter(RID p_shader, const StringName & p_name, int p_index) const {
	return execute<RID, RID, const StringName &, int>(&FilamentRenderingServerBackend::shader_get_default_texture_parameter, p_shader, p_name, p_index);
};

RenderingServer::ShaderNativeSourceCode FilamentRenderingServer::shader_get_native_source_code(RID p_shader) const {
	return execute<ShaderNativeSourceCode, RID>(&FilamentRenderingServerBackend::shader_get_native_source_code, p_shader);
};

RID FilamentRenderingServer::material_create()  {
	auto output = FilamentObjectManager::allocate();

	execute<void, RID>(&FilamentRenderingServerBackend::material_create, output);

	return output;
};

void FilamentRenderingServer::material_set_shader(RID p_shader_material, RID p_shader)  {
	return execute<void, RID, RID>(&FilamentRenderingServerBackend::material_set_shader, p_shader_material, p_shader);
};

void FilamentRenderingServer::material_set_param(RID p_material, const StringName & p_param, const Variant & p_value)  {
	return execute<void, RID, const StringName &, const Variant &>(&FilamentRenderingServerBackend::material_set_param, p_material, p_param, p_value);
};

Variant FilamentRenderingServer::material_get_param(RID p_material, const StringName & p_param) const {
	return execute<Variant, RID, const StringName &>(&FilamentRenderingServerBackend::material_get_param, p_material, p_param);
};

void FilamentRenderingServer::material_set_render_priority(RID p_material, int priority)  {
	return execute<void, RID, int>(&FilamentRenderingServerBackend::material_set_render_priority, p_material, priority);
};

void FilamentRenderingServer::material_set_next_pass(RID p_material, RID p_next_material)  {
	return execute<void, RID, RID>(&FilamentRenderingServerBackend::material_set_next_pass, p_material, p_next_material);
};

RID FilamentRenderingServer::mesh_create_from_surfaces(const Vector<SurfaceData> & p_surfaces, int p_blend_shape_count)  {
	auto output = FilamentObjectManager::allocate();

	execute<void, RID, const Vector<SurfaceData> &, int>(&FilamentRenderingServerBackend::mesh_create_from_surfaces, output, p_surfaces, p_blend_shape_count);

	return output;
};

RID FilamentRenderingServer::mesh_create()  {
	auto output = FilamentObjectManager::allocate();

	execute<void, RID>(&FilamentRenderingServerBackend::mesh_create, output);

	return output;
};

void FilamentRenderingServer::mesh_set_blend_shape_count(RID p_mesh, int p_blend_shape_count)  {
	return execute<void, RID, int>(&FilamentRenderingServerBackend::mesh_set_blend_shape_count, p_mesh, p_blend_shape_count);
};

void FilamentRenderingServer::mesh_add_surface(RID p_mesh, const SurfaceData & p_surface)  {
	return execute<void, RID, const SurfaceData &>(&FilamentRenderingServerBackend::mesh_add_surface, p_mesh, p_surface);
};

int FilamentRenderingServer::mesh_get_blend_shape_count(RID p_mesh) const {
	return execute<int, RID>(&FilamentRenderingServerBackend::mesh_get_blend_shape_count, p_mesh);
};

void FilamentRenderingServer::mesh_set_blend_shape_mode(RID p_mesh, BlendShapeMode p_mode)  {
	return execute<void, RID, BlendShapeMode>(&FilamentRenderingServerBackend::mesh_set_blend_shape_mode, p_mesh, p_mode);
};

RenderingServer::BlendShapeMode FilamentRenderingServer::mesh_get_blend_shape_mode(RID p_mesh) const {
	return execute<BlendShapeMode, RID>(&FilamentRenderingServerBackend::mesh_get_blend_shape_mode, p_mesh);
};

void FilamentRenderingServer::mesh_surface_update_vertex_region(RID p_mesh, int p_surface, int p_offset, const Vector<uint8_t> & p_data)  {
	return execute<void, RID, int, int, const Vector<uint8_t> &>(&FilamentRenderingServerBackend::mesh_surface_update_vertex_region, p_mesh, p_surface, p_offset, p_data);
};

void FilamentRenderingServer::mesh_surface_update_attribute_region(RID p_mesh, int p_surface, int p_offset, const Vector<uint8_t> & p_data)  {
	return execute<void, RID, int, int, const Vector<uint8_t> &>(&FilamentRenderingServerBackend::mesh_surface_update_attribute_region, p_mesh, p_surface, p_offset, p_data);
};

void FilamentRenderingServer::mesh_surface_update_skin_region(RID p_mesh, int p_surface, int p_offset, const Vector<uint8_t> & p_data)  {
	return execute<void, RID, int, int, const Vector<uint8_t> &>(&FilamentRenderingServerBackend::mesh_surface_update_skin_region, p_mesh, p_surface, p_offset, p_data);
};

void FilamentRenderingServer::mesh_surface_set_material(RID p_mesh, int p_surface, RID p_material)  {
	return execute<void, RID, int, RID>(&FilamentRenderingServerBackend::mesh_surface_set_material, p_mesh, p_surface, p_material);
};

RID FilamentRenderingServer::mesh_surface_get_material(RID p_mesh, int p_surface) const {
	return execute<RID, RID, int>(&FilamentRenderingServerBackend::mesh_surface_get_material, p_mesh, p_surface);
};

RenderingServer::SurfaceData FilamentRenderingServer::mesh_get_surface(RID p_mesh, int p_surface) const {
	return execute<SurfaceData, RID, int>(&FilamentRenderingServerBackend::mesh_get_surface, p_mesh, p_surface);
};

int FilamentRenderingServer::mesh_get_surface_count(RID p_mesh) const {
	return execute<int, RID>(&FilamentRenderingServerBackend::mesh_get_surface_count, p_mesh);
};

void FilamentRenderingServer::mesh_set_custom_aabb(RID p_mesh, const AABB & p_aabb)  {
	return execute<void, RID, const AABB &>(&FilamentRenderingServerBackend::mesh_set_custom_aabb, p_mesh, p_aabb);
};

AABB FilamentRenderingServer::mesh_get_custom_aabb(RID p_mesh) const {
	return execute<AABB, RID>(&FilamentRenderingServerBackend::mesh_get_custom_aabb, p_mesh);
};

void FilamentRenderingServer::mesh_set_shadow_mesh(RID p_mesh, RID p_shadow_mesh)  {
	return execute<void, RID, RID>(&FilamentRenderingServerBackend::mesh_set_shadow_mesh, p_mesh, p_shadow_mesh);
};

void FilamentRenderingServer::mesh_clear(RID p_mesh)  {
	return execute<void, RID>(&FilamentRenderingServerBackend::mesh_clear, p_mesh);
};

RID FilamentRenderingServer::multimesh_create()  {
	auto output = FilamentObjectManager::allocate();

	execute<void, RID>(&FilamentRenderingServerBackend::multimesh_create, output);

	return output;
};

void FilamentRenderingServer::multimesh_allocate_data(RID p_multimesh, int p_instances, MultimeshTransformFormat p_transform_format, bool p_use_colors, bool p_use_custom_data)  {
	return execute<void, RID, int, MultimeshTransformFormat, bool, bool>(&FilamentRenderingServerBackend::multimesh_allocate_data, p_multimesh, p_instances, p_transform_format, p_use_colors, p_use_custom_data);
};

int FilamentRenderingServer::multimesh_get_instance_count(RID p_multimesh) const {
	return execute<int, RID>(&FilamentRenderingServerBackend::multimesh_get_instance_count, p_multimesh);
};

void FilamentRenderingServer::multimesh_set_mesh(RID p_multimesh, RID p_mesh)  {
	return execute<void, RID, RID>(&FilamentRenderingServerBackend::multimesh_set_mesh, p_multimesh, p_mesh);
};

void FilamentRenderingServer::multimesh_instance_set_transform(RID p_multimesh, int p_index, const Transform3D & p_transform)  {
	return execute<void, RID, int, const Transform3D &>(&FilamentRenderingServerBackend::multimesh_instance_set_transform, p_multimesh, p_index, p_transform);
};

void FilamentRenderingServer::multimesh_instance_set_transform_2d(RID p_multimesh, int p_index, const Transform2D & p_transform)  {
	return execute<void, RID, int, const Transform2D &>(&FilamentRenderingServerBackend::multimesh_instance_set_transform_2d, p_multimesh, p_index, p_transform);
};

void FilamentRenderingServer::multimesh_instance_set_color(RID p_multimesh, int p_index, const Color & p_color)  {
	return execute<void, RID, int, const Color &>(&FilamentRenderingServerBackend::multimesh_instance_set_color, p_multimesh, p_index, p_color);
};

void FilamentRenderingServer::multimesh_instance_set_custom_data(RID p_multimesh, int p_index, const Color & p_color)  {
	return execute<void, RID, int, const Color &>(&FilamentRenderingServerBackend::multimesh_instance_set_custom_data, p_multimesh, p_index, p_color);
};

RID FilamentRenderingServer::multimesh_get_mesh(RID p_multimesh) const {
	return execute<RID, RID>(&FilamentRenderingServerBackend::multimesh_get_mesh, p_multimesh);
};

AABB FilamentRenderingServer::multimesh_get_aabb(RID p_multimesh) const {
	return execute<AABB, RID>(&FilamentRenderingServerBackend::multimesh_get_aabb, p_multimesh);
};

Transform3D FilamentRenderingServer::multimesh_instance_get_transform(RID p_multimesh, int p_index) const {
	return execute<Transform3D, RID, int>(&FilamentRenderingServerBackend::multimesh_instance_get_transform, p_multimesh, p_index);
};

Transform2D FilamentRenderingServer::multimesh_instance_get_transform_2d(RID p_multimesh, int p_index) const {
	return execute<Transform2D, RID, int>(&FilamentRenderingServerBackend::multimesh_instance_get_transform_2d, p_multimesh, p_index);
};

Color FilamentRenderingServer::multimesh_instance_get_color(RID p_multimesh, int p_index) const {
	return execute<Color, RID, int>(&FilamentRenderingServerBackend::multimesh_instance_get_color, p_multimesh, p_index);
};

Color FilamentRenderingServer::multimesh_instance_get_custom_data(RID p_multimesh, int p_index) const {
	return execute<Color, RID, int>(&FilamentRenderingServerBackend::multimesh_instance_get_custom_data, p_multimesh, p_index);
};

void FilamentRenderingServer::multimesh_set_buffer(RID p_multimesh, const Vector<float> & p_buffer)  {
	return execute<void, RID, const Vector<float> &>(&FilamentRenderingServerBackend::multimesh_set_buffer, p_multimesh, p_buffer);
};

Vector<float> FilamentRenderingServer::multimesh_get_buffer(RID p_multimesh) const {
	return execute<Vector<float>, RID>(&FilamentRenderingServerBackend::multimesh_get_buffer, p_multimesh);
};

void FilamentRenderingServer::multimesh_set_visible_instances(RID p_multimesh, int p_visible)  {
	return execute<void, RID, int>(&FilamentRenderingServerBackend::multimesh_set_visible_instances, p_multimesh, p_visible);
};

int FilamentRenderingServer::multimesh_get_visible_instances(RID p_multimesh) const {
	return execute<int, RID>(&FilamentRenderingServerBackend::multimesh_get_visible_instances, p_multimesh);
};

RID FilamentRenderingServer::skeleton_create()  {
	auto output = FilamentObjectManager::allocate();

	execute<void, RID>(&FilamentRenderingServerBackend::skeleton_create, output);

	return output;
};

void FilamentRenderingServer::skeleton_allocate_data(RID p_skeleton, int p_bones, bool p_2d_skeleton)  {
	return execute<void, RID, int, bool>(&FilamentRenderingServerBackend::skeleton_allocate_data, p_skeleton, p_bones, p_2d_skeleton);
};

int FilamentRenderingServer::skeleton_get_bone_count(RID p_skeleton) const {
	return execute<int, RID>(&FilamentRenderingServerBackend::skeleton_get_bone_count, p_skeleton);
};

void FilamentRenderingServer::skeleton_bone_set_transform(RID p_skeleton, int p_bone, const Transform3D & p_transform)  {
	return execute<void, RID, int, const Transform3D &>(&FilamentRenderingServerBackend::skeleton_bone_set_transform, p_skeleton, p_bone, p_transform);
};

Transform3D FilamentRenderingServer::skeleton_bone_get_transform(RID p_skeleton, int p_bone) const {
	return execute<Transform3D, RID, int>(&FilamentRenderingServerBackend::skeleton_bone_get_transform, p_skeleton, p_bone);
};

void FilamentRenderingServer::skeleton_bone_set_transform_2d(RID p_skeleton, int p_bone, const Transform2D & p_transform)  {
	return execute<void, RID, int, const Transform2D &>(&FilamentRenderingServerBackend::skeleton_bone_set_transform_2d, p_skeleton, p_bone, p_transform);
};

Transform2D FilamentRenderingServer::skeleton_bone_get_transform_2d(RID p_skeleton, int p_bone) const {
	return execute<Transform2D, RID, int>(&FilamentRenderingServerBackend::skeleton_bone_get_transform_2d, p_skeleton, p_bone);
};

void FilamentRenderingServer::skeleton_set_base_transform_2d(RID p_skeleton, const Transform2D & p_base_transform)  {
	return execute<void, RID, const Transform2D &>(&FilamentRenderingServerBackend::skeleton_set_base_transform_2d, p_skeleton, p_base_transform);
};

RID FilamentRenderingServer::directional_light_create()  {
	auto output = FilamentObjectManager::allocate();

	execute<void, RID>(&FilamentRenderingServerBackend::directional_light_create, output);

	return output;
};

RID FilamentRenderingServer::omni_light_create()  {
	auto output = FilamentObjectManager::allocate();

	execute<void, RID>(&FilamentRenderingServerBackend::omni_light_create, output);

	return output;
};

RID FilamentRenderingServer::spot_light_create()  {
	auto output = FilamentObjectManager::allocate();

	execute<void, RID>(&FilamentRenderingServerBackend::spot_light_create, output);

	return output;
};

void FilamentRenderingServer::light_set_color(RID p_light, const Color & p_color)  {
	return execute<void, RID, const Color &>(&FilamentRenderingServerBackend::light_set_color, p_light, p_color);
};

void FilamentRenderingServer::light_set_param(RID p_light, LightParam p_param, float p_value)  {
	return execute<void, RID, LightParam, float>(&FilamentRenderingServerBackend::light_set_param, p_light, p_param, p_value);
};

void FilamentRenderingServer::light_set_shadow(RID p_light, bool p_enabled)  {
	return execute<void, RID, bool>(&FilamentRenderingServerBackend::light_set_shadow, p_light, p_enabled);
};

void FilamentRenderingServer::light_set_projector(RID p_light, RID p_texture)  {
	return execute<void, RID, RID>(&FilamentRenderingServerBackend::light_set_projector, p_light, p_texture);
};

void FilamentRenderingServer::light_set_negative(RID p_light, bool p_enable)  {
	return execute<void, RID, bool>(&FilamentRenderingServerBackend::light_set_negative, p_light, p_enable);
};

void FilamentRenderingServer::light_set_cull_mask(RID p_light, uint32_t p_mask)  {
	return execute<void, RID, uint32_t>(&FilamentRenderingServerBackend::light_set_cull_mask, p_light, p_mask);
};

void FilamentRenderingServer::light_set_distance_fade(RID p_light, bool p_enabled, float p_begin, float p_shadow, float p_length)  {
	return execute<void, RID, bool, float, float, float>(&FilamentRenderingServerBackend::light_set_distance_fade, p_light, p_enabled, p_begin, p_shadow, p_length);
};

void FilamentRenderingServer::light_set_reverse_cull_face_mode(RID p_light, bool p_enabled)  {
	return execute<void, RID, bool>(&FilamentRenderingServerBackend::light_set_reverse_cull_face_mode, p_light, p_enabled);
};

void FilamentRenderingServer::light_set_bake_mode(RID p_light, LightBakeMode p_bake_mode)  {
	return execute<void, RID, LightBakeMode>(&FilamentRenderingServerBackend::light_set_bake_mode, p_light, p_bake_mode);
};

void FilamentRenderingServer::light_set_max_sdfgi_cascade(RID p_light, uint32_t p_cascade)  {
	return execute<void, RID, uint32_t>(&FilamentRenderingServerBackend::light_set_max_sdfgi_cascade, p_light, p_cascade);
};

void FilamentRenderingServer::light_omni_set_shadow_mode(RID p_light, LightOmniShadowMode p_mode)  {
	return execute<void, RID, LightOmniShadowMode>(&FilamentRenderingServerBackend::light_omni_set_shadow_mode, p_light, p_mode);
};

void FilamentRenderingServer::light_directional_set_shadow_mode(RID p_light, LightDirectionalShadowMode p_mode)  {
	return execute<void, RID, LightDirectionalShadowMode>(&FilamentRenderingServerBackend::light_directional_set_shadow_mode, p_light, p_mode);
};

void FilamentRenderingServer::light_directional_set_blend_splits(RID p_light, bool p_enable)  {
	return execute<void, RID, bool>(&FilamentRenderingServerBackend::light_directional_set_blend_splits, p_light, p_enable);
};

void FilamentRenderingServer::light_directional_set_sky_mode(RID p_light, LightDirectionalSkyMode p_mode)  {
	return execute<void, RID, LightDirectionalSkyMode>(&FilamentRenderingServerBackend::light_directional_set_sky_mode, p_light, p_mode);
};

RID FilamentRenderingServer::shadow_atlas_create()  {
	auto output = FilamentObjectManager::allocate();

	execute<RID>(&FilamentRenderingServerBackend::shadow_atlas_create, output);

	return output;
};

void FilamentRenderingServer::shadow_atlas_set_size(RID p_atlas, int p_size, bool p_use_16_bits)  {
	return execute<void, RID, int, bool>(&FilamentRenderingServerBackend::shadow_atlas_set_size, p_atlas, p_size, p_use_16_bits);
};

void FilamentRenderingServer::shadow_atlas_set_quadrant_subdivision(RID p_atlas, int p_quadrant, int p_subdivision)  {
	return execute<void, RID, int, int>(&FilamentRenderingServerBackend::shadow_atlas_set_quadrant_subdivision, p_atlas, p_quadrant, p_subdivision);
};

void FilamentRenderingServer::directional_shadow_atlas_set_size(int p_size, bool p_16_bits)  {
	return execute<void, int, bool>(&FilamentRenderingServerBackend::directional_shadow_atlas_set_size, p_size, p_16_bits);
};

void FilamentRenderingServer::positional_soft_shadow_filter_set_quality(ShadowQuality p_quality)  {
	return execute<void, ShadowQuality>(&FilamentRenderingServerBackend::positional_soft_shadow_filter_set_quality, p_quality);
};

void FilamentRenderingServer::directional_soft_shadow_filter_set_quality(ShadowQuality p_quality)  {
	return execute<void, ShadowQuality>(&FilamentRenderingServerBackend::directional_soft_shadow_filter_set_quality, p_quality);
};

void FilamentRenderingServer::light_projectors_set_filter(LightProjectorFilter p_filter)  {
	return execute<void, LightProjectorFilter>(&FilamentRenderingServerBackend::light_projectors_set_filter, p_filter);
};

RID FilamentRenderingServer::reflection_probe_create()  {
	auto output = FilamentObjectManager::allocate();

	execute<void, RID>(&FilamentRenderingServerBackend::reflection_probe_create, output);

	return output;
};

void FilamentRenderingServer::reflection_probe_set_update_mode(RID p_probe, ReflectionProbeUpdateMode p_mode)  {
	return execute<void, RID, ReflectionProbeUpdateMode>(&FilamentRenderingServerBackend::reflection_probe_set_update_mode, p_probe, p_mode);
};

void FilamentRenderingServer::reflection_probe_set_intensity(RID p_probe, float p_intensity)  {
	return execute<void, RID, float>(&FilamentRenderingServerBackend::reflection_probe_set_intensity, p_probe, p_intensity);
};

void FilamentRenderingServer::reflection_probe_set_ambient_mode(RID p_probe, ReflectionProbeAmbientMode p_mode)  {
	return execute<void, RID, ReflectionProbeAmbientMode>(&FilamentRenderingServerBackend::reflection_probe_set_ambient_mode, p_probe, p_mode);
};

void FilamentRenderingServer::reflection_probe_set_ambient_color(RID p_probe, const Color & p_color)  {
	return execute<void, RID, const Color &>(&FilamentRenderingServerBackend::reflection_probe_set_ambient_color, p_probe, p_color);
};

void FilamentRenderingServer::reflection_probe_set_ambient_energy(RID p_probe, float p_energy)  {
	return execute<void, RID, float>(&FilamentRenderingServerBackend::reflection_probe_set_ambient_energy, p_probe, p_energy);
};

void FilamentRenderingServer::reflection_probe_set_max_distance(RID p_probe, float p_distance)  {
	return execute<void, RID, float>(&FilamentRenderingServerBackend::reflection_probe_set_max_distance, p_probe, p_distance);
};

void FilamentRenderingServer::reflection_probe_set_size(RID p_probe, const Vector3 & p_size)  {
	return execute<void, RID, const Vector3 &>(&FilamentRenderingServerBackend::reflection_probe_set_size, p_probe, p_size);
};

void FilamentRenderingServer::reflection_probe_set_origin_offset(RID p_probe, const Vector3 & p_offset)  {
	return execute<void, RID, const Vector3 &>(&FilamentRenderingServerBackend::reflection_probe_set_origin_offset, p_probe, p_offset);
};

void FilamentRenderingServer::reflection_probe_set_as_interior(RID p_probe, bool p_enable)  {
	return execute<void, RID, bool>(&FilamentRenderingServerBackend::reflection_probe_set_as_interior, p_probe, p_enable);
};

void FilamentRenderingServer::reflection_probe_set_enable_box_projection(RID p_probe, bool p_enable)  {
	return execute<void, RID, bool>(&FilamentRenderingServerBackend::reflection_probe_set_enable_box_projection, p_probe, p_enable);
};

void FilamentRenderingServer::reflection_probe_set_enable_shadows(RID p_probe, bool p_enable)  {
	return execute<void, RID, bool>(&FilamentRenderingServerBackend::reflection_probe_set_enable_shadows, p_probe, p_enable);
};

void FilamentRenderingServer::reflection_probe_set_cull_mask(RID p_probe, uint32_t p_layers)  {
	return execute<void, RID, uint32_t>(&FilamentRenderingServerBackend::reflection_probe_set_cull_mask, p_probe, p_layers);
};

void FilamentRenderingServer::reflection_probe_set_resolution(RID p_probe, int p_resolution)  {
	return execute<void, RID, int>(&FilamentRenderingServerBackend::reflection_probe_set_resolution, p_probe, p_resolution);
};

void FilamentRenderingServer::reflection_probe_set_mesh_lod_threshold(RID p_probe, float p_pixels)  {
	return execute<void, RID, float>(&FilamentRenderingServerBackend::reflection_probe_set_mesh_lod_threshold, p_probe, p_pixels);
};

RID FilamentRenderingServer::decal_create()  {

	auto output = FilamentObjectManager::allocate();

	execute<void, RID>(&FilamentRenderingServerBackend::decal_create, output);

	return output;
};

void FilamentRenderingServer::decal_set_size(RID p_decal, const Vector3 & p_size)  {
	return execute<void, RID, const Vector3 &>(&FilamentRenderingServerBackend::decal_set_size, p_decal, p_size);
};

void FilamentRenderingServer::decal_set_texture(RID p_decal, DecalTexture p_type, RID p_texture)  {
	return execute<void, RID, DecalTexture, RID>(&FilamentRenderingServerBackend::decal_set_texture, p_decal, p_type, p_texture);
};

void FilamentRenderingServer::decal_set_emission_energy(RID p_decal, float p_energy)  {
	return execute<void, RID, float>(&FilamentRenderingServerBackend::decal_set_emission_energy, p_decal, p_energy);
};

void FilamentRenderingServer::decal_set_albedo_mix(RID p_decal, float p_mix)  {
	return execute<void, RID, float>(&FilamentRenderingServerBackend::decal_set_albedo_mix, p_decal, p_mix);
};

void FilamentRenderingServer::decal_set_modulate(RID p_decal, const Color & p_modulate)  {
	return execute<void, RID, const Color &>(&FilamentRenderingServerBackend::decal_set_modulate, p_decal, p_modulate);
};

void FilamentRenderingServer::decal_set_cull_mask(RID p_decal, uint32_t p_layers)  {
	return execute<void, RID, uint32_t>(&FilamentRenderingServerBackend::decal_set_cull_mask, p_decal, p_layers);
};

void FilamentRenderingServer::decal_set_distance_fade(RID p_decal, bool p_enabled, float p_begin, float p_length)  {
	return execute<void, RID, bool, float, float>(&FilamentRenderingServerBackend::decal_set_distance_fade, p_decal, p_enabled, p_begin, p_length);
};

void FilamentRenderingServer::decal_set_fade(RID p_decal, float p_above, float p_below)  {
	return execute<void, RID, float, float>(&FilamentRenderingServerBackend::decal_set_fade, p_decal, p_above, p_below);
};

void FilamentRenderingServer::decal_set_normal_fade(RID p_decal, float p_fade)  {
	return execute<void, RID, float>(&FilamentRenderingServerBackend::decal_set_normal_fade, p_decal, p_fade);
};

void FilamentRenderingServer::decals_set_filter(DecalFilter p_quality)  {
	return execute<void, DecalFilter>(&FilamentRenderingServerBackend::decals_set_filter, p_quality);
};

RID FilamentRenderingServer::voxel_gi_create()  {

	auto output = FilamentObjectManager::allocate();

	execute<void, RID>(&FilamentRenderingServerBackend::voxel_gi_create, output);

	return output;
};

void FilamentRenderingServer::voxel_gi_allocate_data(RID p_voxel_gi, const Transform3D & p_to_cell_xform, const AABB & p_aabb, const Vector3i & p_octree_size, const Vector<uint8_t> & p_octree_cells, const Vector<uint8_t> & p_data_cells, const Vector<uint8_t> & p_distance_field, const Vector<int> & p_level_counts)  {
	return execute<void, RID, const Transform3D &, const AABB &, const Vector3i &, const Vector<uint8_t> &, const Vector<uint8_t> &, const Vector<uint8_t> &, const Vector<int> &>(&FilamentRenderingServerBackend::voxel_gi_allocate_data, p_voxel_gi, p_to_cell_xform, p_aabb, p_octree_size, p_octree_cells, p_data_cells, p_distance_field, p_level_counts);
};

AABB FilamentRenderingServer::voxel_gi_get_bounds(RID p_voxel_gi) const {
	return execute<AABB, RID>(&FilamentRenderingServerBackend::voxel_gi_get_bounds, p_voxel_gi);
};

Vector3i FilamentRenderingServer::voxel_gi_get_octree_size(RID p_voxel_gi) const {
	return execute<Vector3i, RID>(&FilamentRenderingServerBackend::voxel_gi_get_octree_size, p_voxel_gi);
};

Vector<uint8_t> FilamentRenderingServer::voxel_gi_get_octree_cells(RID p_voxel_gi) const {
	return execute<Vector<uint8_t>, RID>(&FilamentRenderingServerBackend::voxel_gi_get_octree_cells, p_voxel_gi);
};

Vector<uint8_t> FilamentRenderingServer::voxel_gi_get_data_cells(RID p_voxel_gi) const {
	return execute<Vector<uint8_t>, RID>(&FilamentRenderingServerBackend::voxel_gi_get_data_cells, p_voxel_gi);
};

Vector<uint8_t> FilamentRenderingServer::voxel_gi_get_distance_field(RID p_voxel_gi) const {
	return execute<Vector<uint8_t>, RID>(&FilamentRenderingServerBackend::voxel_gi_get_distance_field, p_voxel_gi);
};

Vector<int> FilamentRenderingServer::voxel_gi_get_level_counts(RID p_voxel_gi) const {
	return execute<Vector<int>, RID>(&FilamentRenderingServerBackend::voxel_gi_get_level_counts, p_voxel_gi);
};

Transform3D FilamentRenderingServer::voxel_gi_get_to_cell_xform(RID p_voxel_gi) const {
	return execute<Transform3D, RID>(&FilamentRenderingServerBackend::voxel_gi_get_to_cell_xform, p_voxel_gi);
};

void FilamentRenderingServer::voxel_gi_set_dynamic_range(RID p_voxel_gi, float p_range)  {
	return execute<void, RID, float>(&FilamentRenderingServerBackend::voxel_gi_set_dynamic_range, p_voxel_gi, p_range);
};

void FilamentRenderingServer::voxel_gi_set_propagation(RID p_voxel_gi, float p_range)  {
	return execute<void, RID, float>(&FilamentRenderingServerBackend::voxel_gi_set_propagation, p_voxel_gi, p_range);
};

void FilamentRenderingServer::voxel_gi_set_energy(RID p_voxel_gi, float p_energy)  {
	return execute<void, RID, float>(&FilamentRenderingServerBackend::voxel_gi_set_energy, p_voxel_gi, p_energy);
};

void FilamentRenderingServer::voxel_gi_set_baked_exposure_normalization(RID p_voxel_gi, float p_baked_exposure)  {
	return execute<void, RID, float>(&FilamentRenderingServerBackend::voxel_gi_set_baked_exposure_normalization, p_voxel_gi, p_baked_exposure);
};

void FilamentRenderingServer::voxel_gi_set_bias(RID p_voxel_gi, float p_bias)  {
	return execute<void, RID, float>(&FilamentRenderingServerBackend::voxel_gi_set_bias, p_voxel_gi, p_bias);
};

void FilamentRenderingServer::voxel_gi_set_normal_bias(RID p_voxel_gi, float p_range)  {
	return execute<void, RID, float>(&FilamentRenderingServerBackend::voxel_gi_set_normal_bias, p_voxel_gi, p_range);
};

void FilamentRenderingServer::voxel_gi_set_interior(RID p_voxel_gi, bool p_enable)  {
	return execute<void, RID, bool>(&FilamentRenderingServerBackend::voxel_gi_set_interior, p_voxel_gi, p_enable);
};

void FilamentRenderingServer::voxel_gi_set_use_two_bounces(RID p_voxel_gi, bool p_enable)  {
	return execute<void, RID, bool>(&FilamentRenderingServerBackend::voxel_gi_set_use_two_bounces, p_voxel_gi, p_enable);
};

void FilamentRenderingServer::voxel_gi_set_quality(VoxelGIQuality anonarg)  {
	return execute<void, VoxelGIQuality>(&FilamentRenderingServerBackend::voxel_gi_set_quality, anonarg);
};

void FilamentRenderingServer::sdfgi_reset()  {
	return execute<void>(&FilamentRenderingServerBackend::sdfgi_reset);
};

RID FilamentRenderingServer::lightmap_create()  {

	auto output = FilamentObjectManager::allocate();

	execute<void, RID>(&FilamentRenderingServerBackend::lightmap_create, output);

	return output;
};

void FilamentRenderingServer::lightmap_set_textures(RID p_lightmap, RID p_light, bool p_uses_spherical_haromics)  {
	return execute<void, RID, RID, bool>(&FilamentRenderingServerBackend::lightmap_set_textures, p_lightmap, p_light, p_uses_spherical_haromics);
};

void FilamentRenderingServer::lightmap_set_probe_bounds(RID p_lightmap, const AABB & p_bounds)  {
	return execute<void, RID, const AABB &>(&FilamentRenderingServerBackend::lightmap_set_probe_bounds, p_lightmap, p_bounds);
};

void FilamentRenderingServer::lightmap_set_probe_interior(RID p_lightmap, bool p_interior)  {
	return execute<void, RID, bool>(&FilamentRenderingServerBackend::lightmap_set_probe_interior, p_lightmap, p_interior);
};

void FilamentRenderingServer::lightmap_set_probe_capture_data(RID p_lightmap, const PackedVector3Array & p_points, const PackedColorArray & p_point_sh, const PackedInt32Array & p_tetrahedra, const PackedInt32Array & p_bsp_tree)  {
	return execute<void, RID, const PackedVector3Array &, const PackedColorArray &, const PackedInt32Array &, const PackedInt32Array &>(&FilamentRenderingServerBackend::lightmap_set_probe_capture_data, p_lightmap, p_points, p_point_sh, p_tetrahedra, p_bsp_tree);
};

void FilamentRenderingServer::lightmap_set_baked_exposure_normalization(RID p_lightmap, float p_exposure)  {
	return execute<void, RID, float>(&FilamentRenderingServerBackend::lightmap_set_baked_exposure_normalization, p_lightmap, p_exposure);
};

PackedVector3Array FilamentRenderingServer::lightmap_get_probe_capture_points(RID p_lightmap) const {
	return execute<PackedVector3Array, RID>(&FilamentRenderingServerBackend::lightmap_get_probe_capture_points, p_lightmap);
};

PackedColorArray FilamentRenderingServer::lightmap_get_probe_capture_sh(RID p_lightmap) const {
	return execute<PackedColorArray, RID>(&FilamentRenderingServerBackend::lightmap_get_probe_capture_sh, p_lightmap);
};

PackedInt32Array FilamentRenderingServer::lightmap_get_probe_capture_tetrahedra(RID p_lightmap) const {
	return execute<PackedInt32Array, RID>(&FilamentRenderingServerBackend::lightmap_get_probe_capture_tetrahedra, p_lightmap);
};

PackedInt32Array FilamentRenderingServer::lightmap_get_probe_capture_bsp_tree(RID p_lightmap) const {
	return execute<PackedInt32Array, RID>(&FilamentRenderingServerBackend::lightmap_get_probe_capture_bsp_tree, p_lightmap);
};

void FilamentRenderingServer::lightmap_set_probe_capture_update_speed(float p_speed)  {
	return execute<void, float>(&FilamentRenderingServerBackend::lightmap_set_probe_capture_update_speed, p_speed);
};

RID FilamentRenderingServer::particles_create()  {

	auto output = FilamentObjectManager::allocate();

	execute<void, RID>(&FilamentRenderingServerBackend::particles_create, output);

	return output;
};

void FilamentRenderingServer::particles_set_mode(RID p_particles, ParticlesMode p_mode)  {
	return execute<void, RID, ParticlesMode>(&FilamentRenderingServerBackend::particles_set_mode, p_particles, p_mode);
};

void FilamentRenderingServer::particles_set_emitting(RID p_particles, bool p_enable)  {
	return execute<void, RID, bool>(&FilamentRenderingServerBackend::particles_set_emitting, p_particles, p_enable);
};

bool FilamentRenderingServer::particles_get_emitting(RID p_particles)  {
	return execute<bool, RID>(&FilamentRenderingServerBackend::particles_get_emitting, p_particles);
};

void FilamentRenderingServer::particles_set_amount(RID p_particles, int p_amount)  {
	return execute<void, RID, int>(&FilamentRenderingServerBackend::particles_set_amount, p_particles, p_amount);
};

void FilamentRenderingServer::particles_set_amount_ratio(RID p_particles, float p_amount_ratio)  {
	return execute<void, RID, float>(&FilamentRenderingServerBackend::particles_set_amount_ratio, p_particles, p_amount_ratio);
};

void FilamentRenderingServer::particles_set_lifetime(RID p_particles, double p_lifetime)  {
	return execute<void, RID, double>(&FilamentRenderingServerBackend::particles_set_lifetime, p_particles, p_lifetime);
};

void FilamentRenderingServer::particles_set_one_shot(RID p_particles, bool p_one_shot)  {
	return execute<void, RID, bool>(&FilamentRenderingServerBackend::particles_set_one_shot, p_particles, p_one_shot);
};

void FilamentRenderingServer::particles_set_pre_process_time(RID p_particles, double p_time)  {
	return execute<void, RID, double>(&FilamentRenderingServerBackend::particles_set_pre_process_time, p_particles, p_time);
};

void FilamentRenderingServer::particles_set_explosiveness_ratio(RID p_particles, float p_ratio)  {
	return execute<void, RID, float>(&FilamentRenderingServerBackend::particles_set_explosiveness_ratio, p_particles, p_ratio);
};

void FilamentRenderingServer::particles_set_randomness_ratio(RID p_particles, float p_ratio)  {
	return execute<void, RID, float>(&FilamentRenderingServerBackend::particles_set_randomness_ratio, p_particles, p_ratio);
};

void FilamentRenderingServer::particles_set_custom_aabb(RID p_particles, const AABB & p_aabb)  {
	return execute<void, RID, const AABB &>(&FilamentRenderingServerBackend::particles_set_custom_aabb, p_particles, p_aabb);
};

void FilamentRenderingServer::particles_set_speed_scale(RID p_particles, double p_scale)  {
	return execute<void, RID, double>(&FilamentRenderingServerBackend::particles_set_speed_scale, p_particles, p_scale);
};

void FilamentRenderingServer::particles_set_use_local_coordinates(RID p_particles, bool p_enable)  {
	return execute<void, RID, bool>(&FilamentRenderingServerBackend::particles_set_use_local_coordinates, p_particles, p_enable);
};

void FilamentRenderingServer::particles_set_process_material(RID p_particles, RID p_material)  {
	return execute<void, RID, RID>(&FilamentRenderingServerBackend::particles_set_process_material, p_particles, p_material);
};

void FilamentRenderingServer::particles_set_fixed_fps(RID p_particles, int p_fps)  {
	return execute<void, RID, int>(&FilamentRenderingServerBackend::particles_set_fixed_fps, p_particles, p_fps);
};

void FilamentRenderingServer::particles_set_interpolate(RID p_particles, bool p_enable)  {
	return execute<void, RID, bool>(&FilamentRenderingServerBackend::particles_set_interpolate, p_particles, p_enable);
};

void FilamentRenderingServer::particles_set_fractional_delta(RID p_particles, bool p_enable)  {
	return execute<void, RID, bool>(&FilamentRenderingServerBackend::particles_set_fractional_delta, p_particles, p_enable);
};

void FilamentRenderingServer::particles_set_collision_base_size(RID p_particles, float p_size)  {
	return execute<void, RID, float>(&FilamentRenderingServerBackend::particles_set_collision_base_size, p_particles, p_size);
};

void FilamentRenderingServer::particles_set_transform_align(RID p_particles, ParticlesTransformAlign p_transform_align)  {
	return execute<void, RID, ParticlesTransformAlign>(&FilamentRenderingServerBackend::particles_set_transform_align, p_particles, p_transform_align);
};

void FilamentRenderingServer::particles_set_trails(RID p_particles, bool p_enable, float p_length_sec)  {
	return execute<void, RID, bool, float>(&FilamentRenderingServerBackend::particles_set_trails, p_particles, p_enable, p_length_sec);
};

void FilamentRenderingServer::particles_set_trail_bind_poses(RID p_particles, const Vector<Transform3D> & p_bind_poses)  {
	return execute<void, RID, const Vector<Transform3D> &>(&FilamentRenderingServerBackend::particles_set_trail_bind_poses, p_particles, p_bind_poses);
};

bool FilamentRenderingServer::particles_is_inactive(RID p_particles)  {
	return execute<bool, RID>(&FilamentRenderingServerBackend::particles_is_inactive, p_particles);
};

void FilamentRenderingServer::particles_request_process(RID p_particles)  {
	return execute<void, RID>(&FilamentRenderingServerBackend::particles_request_process, p_particles);
};

void FilamentRenderingServer::particles_restart(RID p_particles)  {
	return execute<void, RID>(&FilamentRenderingServerBackend::particles_restart, p_particles);
};

void FilamentRenderingServer::particles_set_subemitter(RID p_particles, RID p_subemitter_particles)  {
	return execute<void, RID, RID>(&FilamentRenderingServerBackend::particles_set_subemitter, p_particles, p_subemitter_particles);
};

void FilamentRenderingServer::particles_emit(RID p_particles, const Transform3D & p_transform, const Vector3 & p_velocity, const Color & p_color, const Color & p_custom, uint32_t p_emit_flags)  {
	return execute<void, RID, const Transform3D &, const Vector3 &, const Color &, const Color &, uint32_t>(&FilamentRenderingServerBackend::particles_emit, p_particles, p_transform, p_velocity, p_color, p_custom, p_emit_flags);
};

void FilamentRenderingServer::particles_set_draw_order(RID p_particles, ParticlesDrawOrder p_order)  {
	return execute<void, RID, ParticlesDrawOrder>(&FilamentRenderingServerBackend::particles_set_draw_order, p_particles, p_order);
};

void FilamentRenderingServer::particles_set_draw_passes(RID p_particles, int p_count)  {
	return execute<void, RID, int>(&FilamentRenderingServerBackend::particles_set_draw_passes, p_particles, p_count);
};

void FilamentRenderingServer::particles_set_draw_pass_mesh(RID p_particles, int p_pass, RID p_mesh)  {
	return execute<void, RID, int, RID>(&FilamentRenderingServerBackend::particles_set_draw_pass_mesh, p_particles, p_pass, p_mesh);
};

AABB FilamentRenderingServer::particles_get_current_aabb(RID p_particles)  {
	return execute<AABB, RID>(&FilamentRenderingServerBackend::particles_get_current_aabb, p_particles);
};

void FilamentRenderingServer::particles_set_emission_transform(RID p_particles, const Transform3D & p_transform)  {
	return execute<void, RID, const Transform3D &>(&FilamentRenderingServerBackend::particles_set_emission_transform, p_particles, p_transform);
};

void FilamentRenderingServer::particles_set_emitter_velocity(RID p_particles, const Vector3 & p_velocity)  {
	return execute<void, RID, const Vector3 &>(&FilamentRenderingServerBackend::particles_set_emitter_velocity, p_particles, p_velocity);
};

void FilamentRenderingServer::particles_set_interp_to_end(RID p_particles, float p_interp)  {
	return execute<void, RID, float>(&FilamentRenderingServerBackend::particles_set_interp_to_end, p_particles, p_interp);
};

RID FilamentRenderingServer::particles_collision_create()  {

	auto output = FilamentObjectManager::allocate();

	execute<void, RID>(&FilamentRenderingServerBackend::particles_collision_create, output);

	return output;
};

void FilamentRenderingServer::particles_collision_set_collision_type(RID p_particles_collision, ParticlesCollisionType p_type)  {
	return execute<void, RID, ParticlesCollisionType>(&FilamentRenderingServerBackend::particles_collision_set_collision_type, p_particles_collision, p_type);
};

void FilamentRenderingServer::particles_collision_set_cull_mask(RID p_particles_collision, uint32_t p_cull_mask)  {
	return execute<void, RID, uint32_t>(&FilamentRenderingServerBackend::particles_collision_set_cull_mask, p_particles_collision, p_cull_mask);
};

void FilamentRenderingServer::particles_collision_set_sphere_radius(RID p_particles_collision, real_t p_radius)  {
	return execute<void, RID, real_t>(&FilamentRenderingServerBackend::particles_collision_set_sphere_radius, p_particles_collision, p_radius);
};

void FilamentRenderingServer::particles_collision_set_box_extents(RID p_particles_collision, const Vector3 & p_extents)  {
	return execute<void, RID, const Vector3 &>(&FilamentRenderingServerBackend::particles_collision_set_box_extents, p_particles_collision, p_extents);
};

void FilamentRenderingServer::particles_collision_set_attractor_strength(RID p_particles_collision, real_t p_strength)  {
	return execute<void, RID, real_t>(&FilamentRenderingServerBackend::particles_collision_set_attractor_strength, p_particles_collision, p_strength);
};

void FilamentRenderingServer::particles_collision_set_attractor_directionality(RID p_particles_collision, real_t p_directionality)  {
	return execute<void, RID, real_t>(&FilamentRenderingServerBackend::particles_collision_set_attractor_directionality, p_particles_collision, p_directionality);
};

void FilamentRenderingServer::particles_collision_set_attractor_attenuation(RID p_particles_collision, real_t p_curve)  {
	return execute<void, RID, real_t>(&FilamentRenderingServerBackend::particles_collision_set_attractor_attenuation, p_particles_collision, p_curve);
};

void FilamentRenderingServer::particles_collision_set_field_texture(RID p_particles_collision, RID p_texture)  {
	return execute<void, RID, RID>(&FilamentRenderingServerBackend::particles_collision_set_field_texture, p_particles_collision, p_texture);
};

void FilamentRenderingServer::particles_collision_height_field_update(RID p_particles_collision)  {
	return execute<void, RID>(&FilamentRenderingServerBackend::particles_collision_height_field_update, p_particles_collision);
};

void FilamentRenderingServer::particles_collision_set_height_field_resolution(RID p_particles_collision, ParticlesCollisionHeightfieldResolution p_resolution)  {
	return execute<void, RID, ParticlesCollisionHeightfieldResolution>(&FilamentRenderingServerBackend::particles_collision_set_height_field_resolution, p_particles_collision, p_resolution);
};

RID FilamentRenderingServer::fog_volume_create()  {

	auto output = FilamentObjectManager::allocate();

	execute<void, RID>(&FilamentRenderingServerBackend::fog_volume_create, output);

	return output;
};

void FilamentRenderingServer::fog_volume_set_shape(RID p_fog_volume, FogVolumeShape p_shape)  {
	return execute<void, RID, FogVolumeShape>(&FilamentRenderingServerBackend::fog_volume_set_shape, p_fog_volume, p_shape);
};

void FilamentRenderingServer::fog_volume_set_size(RID p_fog_volume, const Vector3 & p_size)  {
	return execute<void, RID, const Vector3 &>(&FilamentRenderingServerBackend::fog_volume_set_size, p_fog_volume, p_size);
};

void FilamentRenderingServer::fog_volume_set_material(RID p_fog_volume, RID p_material)  {
	return execute<void, RID, RID>(&FilamentRenderingServerBackend::fog_volume_set_material, p_fog_volume, p_material);
};

RID FilamentRenderingServer::visibility_notifier_create()  {
	auto output = FilamentObjectManager::allocate();

	execute<void, RID>(&FilamentRenderingServerBackend::visibility_notifier_create, output);

	return output;
};

void FilamentRenderingServer::visibility_notifier_set_aabb(RID p_notifier, const AABB & p_aabb)  {
	return execute<void, RID, const AABB &>(&FilamentRenderingServerBackend::visibility_notifier_set_aabb, p_notifier, p_aabb);
};

void FilamentRenderingServer::visibility_notifier_set_callbacks(RID p_notifier, const Callable & p_enter_callbable, const Callable & p_exit_callable)  {
	return execute<void, RID, const Callable &, const Callable &>(&FilamentRenderingServerBackend::visibility_notifier_set_callbacks, p_notifier, p_enter_callbable, p_exit_callable);
};

RID FilamentRenderingServer::occluder_create()  {

	auto output = FilamentObjectManager::allocate();

	execute<void, RID>(&FilamentRenderingServerBackend::occluder_create, output);

	return output;
};

void FilamentRenderingServer::occluder_set_mesh(RID p_occluder, const PackedVector3Array & p_vertices, const PackedInt32Array & p_indices)  {
	return execute<void, RID, const PackedVector3Array &, const PackedInt32Array &>(&FilamentRenderingServerBackend::occluder_set_mesh, p_occluder, p_vertices, p_indices);
};

RID FilamentRenderingServer::camera_create()  {
	auto output = FilamentObjectManager::allocate();

	execute<void, RID>(&FilamentRenderingServerBackend::camera_create, output);

	return output;
};

void FilamentRenderingServer::camera_set_perspective(RID p_camera, float p_fovy_degrees, float p_z_near, float p_z_far)  {
	return execute<void, RID, float, float, float>(&FilamentRenderingServerBackend::camera_set_perspective, p_camera, p_fovy_degrees, p_z_near, p_z_far);
};

void FilamentRenderingServer::camera_set_orthogonal(RID p_camera, float p_size, float p_z_near, float p_z_far)  {
	return execute<void, RID, float, float, float>(&FilamentRenderingServerBackend::camera_set_orthogonal, p_camera, p_size, p_z_near, p_z_far);
};

void FilamentRenderingServer::camera_set_frustum(RID p_camera, float p_size, Vector2 p_offset, float p_z_near, float p_z_far)  {
	return execute<void, RID, float, Vector2, float, float>(&FilamentRenderingServerBackend::camera_set_frustum, p_camera, p_size, p_offset, p_z_near, p_z_far);
};

void FilamentRenderingServer::camera_set_transform(RID p_camera, const Transform3D & p_transform)  {
	return execute<void, RID, const Transform3D &>(&FilamentRenderingServerBackend::camera_set_transform, p_camera, p_transform);
};

void FilamentRenderingServer::camera_set_cull_mask(RID p_camera, uint32_t p_layers)  {
	return execute<void, RID, uint32_t>(&FilamentRenderingServerBackend::camera_set_cull_mask, p_camera, p_layers);
};

void FilamentRenderingServer::camera_set_environment(RID p_camera, RID p_env)  {
	return execute<void, RID, RID>(&FilamentRenderingServerBackend::camera_set_environment, p_camera, p_env);
};

void FilamentRenderingServer::camera_set_camera_attributes(RID p_camera, RID p_camera_attributes)  {
	return execute<void, RID, RID>(&FilamentRenderingServerBackend::camera_set_camera_attributes, p_camera, p_camera_attributes);
};

void FilamentRenderingServer::camera_set_use_vertical_aspect(RID p_camera, bool p_enable)  {
	return execute<void, RID, bool>(&FilamentRenderingServerBackend::camera_set_use_vertical_aspect, p_camera, p_enable);
};

RID FilamentRenderingServer::viewport_create()  {
	auto output = FilamentObjectManager::allocate();

	execute<void, RID>(&FilamentRenderingServerBackend::viewport_create, output);

	return output;
};

void FilamentRenderingServer::viewport_set_use_xr(RID p_viewport, bool p_use_xr)  {
	return execute<void, RID, bool>(&FilamentRenderingServerBackend::viewport_set_use_xr, p_viewport, p_use_xr);
};

void FilamentRenderingServer::viewport_set_size(RID p_viewport, int p_width, int p_height)  {
	return execute<void, RID, int, int>(&FilamentRenderingServerBackend::viewport_set_size, p_viewport, p_width, p_height);
};

void FilamentRenderingServer::viewport_set_active(RID p_viewport, bool p_active)  {
	return execute<void, RID, bool>(&FilamentRenderingServerBackend::viewport_set_active, p_viewport, p_active);
};

void FilamentRenderingServer::viewport_set_parent_viewport(RID p_viewport, RID p_parent_viewport)  {
	return execute<void, RID, RID>(&FilamentRenderingServerBackend::viewport_set_parent_viewport, p_viewport, p_parent_viewport);
};

void FilamentRenderingServer::viewport_set_canvas_cull_mask(RID p_viewport, uint32_t p_canvas_cull_mask)  {
	return execute<void, RID, uint32_t>(&FilamentRenderingServerBackend::viewport_set_canvas_cull_mask, p_viewport, p_canvas_cull_mask);
};

void FilamentRenderingServer::viewport_attach_to_screen(RID p_viewport, const Rect2 & p_rect, DisplayServer::WindowID p_screen)  {
	return execute<void, RID, const Rect2 &, DisplayServer::WindowID>(&FilamentRenderingServerBackend::viewport_attach_to_screen, p_viewport, p_rect, p_screen);
};

void FilamentRenderingServer::viewport_set_render_direct_to_screen(RID p_viewport, bool p_enable)  {
	return execute<void, RID, bool>(&FilamentRenderingServerBackend::viewport_set_render_direct_to_screen, p_viewport, p_enable);
};

void FilamentRenderingServer::viewport_set_scaling_3d_mode(RID p_viewport, ViewportScaling3DMode p_scaling_3d_mode)  {
	return execute<void, RID, ViewportScaling3DMode>(&FilamentRenderingServerBackend::viewport_set_scaling_3d_mode, p_viewport, p_scaling_3d_mode);
};

void FilamentRenderingServer::viewport_set_scaling_3d_scale(RID p_viewport, float p_scaling_3d_scale)  {
	return execute<void, RID, float>(&FilamentRenderingServerBackend::viewport_set_scaling_3d_scale, p_viewport, p_scaling_3d_scale);
};

void FilamentRenderingServer::viewport_set_fsr_sharpness(RID p_viewport, float p_fsr_sharpness)  {
	return execute<void, RID, float>(&FilamentRenderingServerBackend::viewport_set_fsr_sharpness, p_viewport, p_fsr_sharpness);
};

void FilamentRenderingServer::viewport_set_texture_mipmap_bias(RID p_viewport, float p_texture_mipmap_bias)  {
	return execute<void, RID, float>(&FilamentRenderingServerBackend::viewport_set_texture_mipmap_bias, p_viewport, p_texture_mipmap_bias);
};

void FilamentRenderingServer::viewport_set_update_mode(RID p_viewport, ViewportUpdateMode p_mode)  {
	return execute<void, RID, ViewportUpdateMode>(&FilamentRenderingServerBackend::viewport_set_update_mode, p_viewport, p_mode);
};

void FilamentRenderingServer::viewport_set_clear_mode(RID p_viewport, ViewportClearMode p_clear_mode)  {
	return execute<void, RID, ViewportClearMode>(&FilamentRenderingServerBackend::viewport_set_clear_mode, p_viewport, p_clear_mode);
};

RID FilamentRenderingServer::viewport_get_render_target(RID p_viewport) const {
	return execute<RID, RID>(&FilamentRenderingServerBackend::viewport_get_render_target, p_viewport);
};

RID FilamentRenderingServer::viewport_get_texture(RID p_viewport) const {
	return execute<RID, RID>(&FilamentRenderingServerBackend::viewport_get_texture, p_viewport);
};

void FilamentRenderingServer::viewport_set_environment_mode(RID p_viewport, ViewportEnvironmentMode p_mode)  {
	return execute<void, RID, ViewportEnvironmentMode>(&FilamentRenderingServerBackend::viewport_set_environment_mode, p_viewport, p_mode);
};

void FilamentRenderingServer::viewport_set_disable_3d(RID p_viewport, bool p_disable)  {
	return execute<void, RID, bool>(&FilamentRenderingServerBackend::viewport_set_disable_3d, p_viewport, p_disable);
};

void FilamentRenderingServer::viewport_set_disable_2d(RID p_viewport, bool p_disable)  {
	return execute<void, RID, bool>(&FilamentRenderingServerBackend::viewport_set_disable_2d, p_viewport, p_disable);
};

void FilamentRenderingServer::viewport_attach_camera(RID p_viewport, RID p_camera)  {
	return execute<void, RID, RID>(&FilamentRenderingServerBackend::viewport_attach_camera, p_viewport, p_camera);
};

void FilamentRenderingServer::viewport_set_scenario(RID p_viewport, RID p_scenario)  {
	return execute<void, RID, RID>(&FilamentRenderingServerBackend::viewport_set_scenario, p_viewport, p_scenario);
};

void FilamentRenderingServer::viewport_attach_canvas(RID p_viewport, RID p_canvas)  {
	return execute<void, RID, RID>(&FilamentRenderingServerBackend::viewport_attach_canvas, p_viewport, p_canvas);
};

void FilamentRenderingServer::viewport_remove_canvas(RID p_viewport, RID p_canvas)  {
	return execute<void, RID, RID>(&FilamentRenderingServerBackend::viewport_remove_canvas, p_viewport, p_canvas);
};

void FilamentRenderingServer::viewport_set_canvas_transform(RID p_viewport, RID p_canvas, const Transform2D & p_offset)  {
	return execute<void, RID, RID, const Transform2D &>(&FilamentRenderingServerBackend::viewport_set_canvas_transform, p_viewport, p_canvas, p_offset);
};

void FilamentRenderingServer::viewport_set_transparent_background(RID p_viewport, bool p_enabled)  {
	return execute<void, RID, bool>(&FilamentRenderingServerBackend::viewport_set_transparent_background, p_viewport, p_enabled);
};

void FilamentRenderingServer::viewport_set_use_hdr_2d(RID p_viewport, bool p_use_hdr)  {
	return execute<void, RID, bool>(&FilamentRenderingServerBackend::viewport_set_use_hdr_2d, p_viewport, p_use_hdr);
};

void FilamentRenderingServer::viewport_set_snap_2d_transforms_to_pixel(RID p_viewport, bool p_enabled)  {
	return execute<void, RID, bool>(&FilamentRenderingServerBackend::viewport_set_snap_2d_transforms_to_pixel, p_viewport, p_enabled);
};

void FilamentRenderingServer::viewport_set_snap_2d_vertices_to_pixel(RID p_viewport, bool p_enabled)  {
	return execute<void, RID, bool>(&FilamentRenderingServerBackend::viewport_set_snap_2d_vertices_to_pixel, p_viewport, p_enabled);
};

void FilamentRenderingServer::viewport_set_default_canvas_item_texture_filter(RID p_viewport, CanvasItemTextureFilter p_filter)  {
	return execute<void, RID, CanvasItemTextureFilter>(&FilamentRenderingServerBackend::viewport_set_default_canvas_item_texture_filter, p_viewport, p_filter);
};

void FilamentRenderingServer::viewport_set_default_canvas_item_texture_repeat(RID p_viewport, CanvasItemTextureRepeat p_repeat)  {
	return execute<void, RID, CanvasItemTextureRepeat>(&FilamentRenderingServerBackend::viewport_set_default_canvas_item_texture_repeat, p_viewport, p_repeat);
};

void FilamentRenderingServer::viewport_set_global_canvas_transform(RID p_viewport, const Transform2D & p_transform)  {
	return execute<void, RID, const Transform2D &>(&FilamentRenderingServerBackend::viewport_set_global_canvas_transform, p_viewport, p_transform);
};

void FilamentRenderingServer::viewport_set_canvas_stacking(RID p_viewport, RID p_canvas, int p_layer, int p_sublayer)  {
	return execute<void, RID, RID, int, int>(&FilamentRenderingServerBackend::viewport_set_canvas_stacking, p_viewport, p_canvas, p_layer, p_sublayer);
};

void FilamentRenderingServer::viewport_set_sdf_oversize_and_scale(RID p_viewport, ViewportSDFOversize p_oversize, ViewportSDFScale p_scale)  {
	return execute<void, RID, ViewportSDFOversize, ViewportSDFScale>(&FilamentRenderingServerBackend::viewport_set_sdf_oversize_and_scale, p_viewport, p_oversize, p_scale);
};

void FilamentRenderingServer::viewport_set_positional_shadow_atlas_size(RID p_viewport, int p_size, bool p_16_bits)  {
	return execute<void, RID, int, bool>(&FilamentRenderingServerBackend::viewport_set_positional_shadow_atlas_size, p_viewport, p_size, p_16_bits);
};

void FilamentRenderingServer::viewport_set_positional_shadow_atlas_quadrant_subdivision(RID p_viewport, int p_quadrant, int p_subdiv)  {
	return execute<void, RID, int, int>(&FilamentRenderingServerBackend::viewport_set_positional_shadow_atlas_quadrant_subdivision, p_viewport, p_quadrant, p_subdiv);
};

void FilamentRenderingServer::viewport_set_msaa_3d(RID p_viewport, ViewportMSAA p_msaa)  {
	return execute<void, RID, ViewportMSAA>(&FilamentRenderingServerBackend::viewport_set_msaa_3d, p_viewport, p_msaa);
};

void FilamentRenderingServer::viewport_set_msaa_2d(RID p_viewport, ViewportMSAA p_msaa)  {
	return execute<void, RID, ViewportMSAA>(&FilamentRenderingServerBackend::viewport_set_msaa_2d, p_viewport, p_msaa);
};

void FilamentRenderingServer::viewport_set_screen_space_aa(RID p_viewport, ViewportScreenSpaceAA p_mode)  {
	return execute<void, RID, ViewportScreenSpaceAA>(&FilamentRenderingServerBackend::viewport_set_screen_space_aa, p_viewport, p_mode);
};

void FilamentRenderingServer::viewport_set_use_taa(RID p_viewport, bool p_use_taa)  {
	return execute<void, RID, bool>(&FilamentRenderingServerBackend::viewport_set_use_taa, p_viewport, p_use_taa);
};

void FilamentRenderingServer::viewport_set_use_debanding(RID p_viewport, bool p_use_debanding)  {
	return execute<void, RID, bool>(&FilamentRenderingServerBackend::viewport_set_use_debanding, p_viewport, p_use_debanding);
};

void FilamentRenderingServer::viewport_set_mesh_lod_threshold(RID p_viewport, float p_pixels)  {
	return execute<void, RID, float>(&FilamentRenderingServerBackend::viewport_set_mesh_lod_threshold, p_viewport, p_pixels);
};

void FilamentRenderingServer::viewport_set_use_occlusion_culling(RID p_viewport, bool p_use_occlusion_culling)  {
	return execute<void, RID, bool>(&FilamentRenderingServerBackend::viewport_set_use_occlusion_culling, p_viewport, p_use_occlusion_culling);
};

void FilamentRenderingServer::viewport_set_occlusion_rays_per_thread(int p_rays_per_thread)  {
	return execute<void, int>(&FilamentRenderingServerBackend::viewport_set_occlusion_rays_per_thread, p_rays_per_thread);
};

void FilamentRenderingServer::viewport_set_occlusion_culling_build_quality(ViewportOcclusionCullingBuildQuality p_quality)  {
	return execute<void, ViewportOcclusionCullingBuildQuality>(&FilamentRenderingServerBackend::viewport_set_occlusion_culling_build_quality, p_quality);
};

int FilamentRenderingServer::viewport_get_render_info(RID p_viewport, ViewportRenderInfoType p_type, ViewportRenderInfo p_info)  {
	return execute<int, RID, ViewportRenderInfoType, ViewportRenderInfo>(&FilamentRenderingServerBackend::viewport_get_render_info, p_viewport, p_type, p_info);
};

void FilamentRenderingServer::viewport_set_debug_draw(RID p_viewport, ViewportDebugDraw p_draw)  {
	return execute<void, RID, ViewportDebugDraw>(&FilamentRenderingServerBackend::viewport_set_debug_draw, p_viewport, p_draw);
};

void FilamentRenderingServer::viewport_set_measure_render_time(RID p_viewport, bool p_enable)  {
	return execute<void, RID, bool>(&FilamentRenderingServerBackend::viewport_set_measure_render_time, p_viewport, p_enable);
};

double FilamentRenderingServer::viewport_get_measured_render_time_cpu(RID p_viewport) const {
	return execute<double, RID>(&FilamentRenderingServerBackend::viewport_get_measured_render_time_cpu, p_viewport);
};

double FilamentRenderingServer::viewport_get_measured_render_time_gpu(RID p_viewport) const {
	return execute<double, RID>(&FilamentRenderingServerBackend::viewport_get_measured_render_time_gpu, p_viewport);
};

RID FilamentRenderingServer::viewport_find_from_screen_attachment(DisplayServer::WindowID p_id) const {
	return execute<RID, DisplayServer::WindowID>(&FilamentRenderingServerBackend::viewport_find_from_screen_attachment, p_id);
};

void FilamentRenderingServer::viewport_set_vrs_mode(RID p_viewport, ViewportVRSMode p_mode)  {
	return execute<void, RID, ViewportVRSMode>(&FilamentRenderingServerBackend::viewport_set_vrs_mode, p_viewport, p_mode);
};

void FilamentRenderingServer::viewport_set_vrs_texture(RID p_viewport, RID p_texture)  {
	return execute<void, RID, RID>(&FilamentRenderingServerBackend::viewport_set_vrs_texture, p_viewport, p_texture);
};

RID FilamentRenderingServer::sky_create()  {
	auto output = FilamentObjectManager::allocate();

	execute<void, RID>(&FilamentRenderingServerBackend::sky_create, output);

	return output;
};

void FilamentRenderingServer::sky_set_radiance_size(RID p_sky, int p_radiance_size)  {
	return execute<void, RID, int>(&FilamentRenderingServerBackend::sky_set_radiance_size, p_sky, p_radiance_size);
};

void FilamentRenderingServer::sky_set_mode(RID p_sky, SkyMode p_mode)  {
	return execute<void, RID, SkyMode>(&FilamentRenderingServerBackend::sky_set_mode, p_sky, p_mode);
};

void FilamentRenderingServer::sky_set_material(RID p_sky, RID p_material)  {
	return execute<void, RID, RID>(&FilamentRenderingServerBackend::sky_set_material, p_sky, p_material);
};

Ref<Image> FilamentRenderingServer::sky_bake_panorama(RID p_sky, float p_energy, bool p_bake_irradiance, const Size2i & p_size)  {
	return execute<Ref<Image>, RID, float, bool, const Size2i &>(&FilamentRenderingServerBackend::sky_bake_panorama, p_sky, p_energy, p_bake_irradiance, p_size);
};

RID FilamentRenderingServer::environment_create()  {

	auto output = FilamentObjectManager::allocate();

	execute<void, RID>(&FilamentRenderingServerBackend::environment_create, output);

	return output;
};

void FilamentRenderingServer::environment_set_background(RID p_env, EnvironmentBG p_bg)  {
	return execute<void, RID, EnvironmentBG>(&FilamentRenderingServerBackend::environment_set_background, p_env, p_bg);
};

void FilamentRenderingServer::environment_set_sky(RID p_env, RID p_sky)  {
	return execute<void, RID, RID>(&FilamentRenderingServerBackend::environment_set_sky, p_env, p_sky);
};

void FilamentRenderingServer::environment_set_sky_custom_fov(RID p_env, float p_scale)  {
	return execute<void, RID, float>(&FilamentRenderingServerBackend::environment_set_sky_custom_fov, p_env, p_scale);
};

void FilamentRenderingServer::environment_set_sky_orientation(RID p_env, const Basis & p_orientation)  {
	return execute<void, RID, const Basis &>(&FilamentRenderingServerBackend::environment_set_sky_orientation, p_env, p_orientation);
};

void FilamentRenderingServer::environment_set_bg_color(RID p_env, const Color & p_color)  {
	return execute<void, RID, const Color &>(&FilamentRenderingServerBackend::environment_set_bg_color, p_env, p_color);
};

void FilamentRenderingServer::environment_set_bg_energy(RID p_env, float p_multiplier, float p_exposure_value)  {
	return execute<void, RID, float, float>(&FilamentRenderingServerBackend::environment_set_bg_energy, p_env, p_multiplier, p_exposure_value);
};

void FilamentRenderingServer::environment_set_canvas_max_layer(RID p_env, int p_max_layer)  {
	return execute<void, RID, int>(&FilamentRenderingServerBackend::environment_set_canvas_max_layer, p_env, p_max_layer);
};

void FilamentRenderingServer::environment_set_ambient_light(RID p_env, const Color & p_color, EnvironmentAmbientSource p_ambient, float p_energy, float p_sky_contribution, EnvironmentReflectionSource p_reflection_source)  {
	return execute<void, RID, const Color &, EnvironmentAmbientSource, float, float, EnvironmentReflectionSource>(&FilamentRenderingServerBackend::environment_set_ambient_light, p_env, p_color, p_ambient, p_energy, p_sky_contribution, p_reflection_source);
};

void FilamentRenderingServer::environment_set_glow(RID p_env, bool p_enable, Vector<float> p_levels, float p_intensity, float p_strength, float p_mix, float p_bloom_threshold, EnvironmentGlowBlendMode p_blend_mode, float p_hdr_bleed_threshold, float p_hdr_bleed_scale, float p_hdr_luminance_cap, float p_glow_map_strength, RID p_glow_map)  {
	return execute<void, RID, bool, Vector<float>, float, float, float, float, EnvironmentGlowBlendMode, float, float, float, float, RID>(&FilamentRenderingServerBackend::environment_set_glow, p_env, p_enable, p_levels, p_intensity, p_strength, p_mix, p_bloom_threshold, p_blend_mode, p_hdr_bleed_threshold, p_hdr_bleed_scale, p_hdr_luminance_cap, p_glow_map_strength, p_glow_map);
};

void FilamentRenderingServer::environment_glow_set_use_bicubic_upscale(bool p_enable)  {
	return execute<void, bool>(&FilamentRenderingServerBackend::environment_glow_set_use_bicubic_upscale, p_enable);
};

void FilamentRenderingServer::environment_set_tonemap(RID p_env, EnvironmentToneMapper p_tone_mapper, float p_exposure, float p_white)  {
	return execute<void, RID, EnvironmentToneMapper, float, float>(&FilamentRenderingServerBackend::environment_set_tonemap, p_env, p_tone_mapper, p_exposure, p_white);
};

void FilamentRenderingServer::environment_set_adjustment(RID p_env, bool p_enable, float p_brightness, float p_contrast, float p_saturation, bool p_use_1d_color_correction, RID p_color_correction)  {
	return execute<void, RID, bool, float, float, float, bool, RID>(&FilamentRenderingServerBackend::environment_set_adjustment, p_env, p_enable, p_brightness, p_contrast, p_saturation, p_use_1d_color_correction, p_color_correction);
};

void FilamentRenderingServer::environment_set_ssr(RID p_env, bool p_enable, int p_max_steps, float p_fade_in, float p_fade_out, float p_depth_tolerance)  {
	return execute<void, RID, bool, int, float, float, float>(&FilamentRenderingServerBackend::environment_set_ssr, p_env, p_enable, p_max_steps, p_fade_in, p_fade_out, p_depth_tolerance);
};

void FilamentRenderingServer::environment_set_ssr_roughness_quality(EnvironmentSSRRoughnessQuality p_quality)  {
	return execute<void, EnvironmentSSRRoughnessQuality>(&FilamentRenderingServerBackend::environment_set_ssr_roughness_quality, p_quality);
};

void FilamentRenderingServer::environment_set_ssao(RID p_env, bool p_enable, float p_radius, float p_intensity, float p_power, float p_detail, float p_horizon, float p_sharpness, float p_light_affect, float p_ao_channel_affect)  {
	return execute<void, RID, bool, float, float, float, float, float, float, float, float>(&FilamentRenderingServerBackend::environment_set_ssao, p_env, p_enable, p_radius, p_intensity, p_power, p_detail, p_horizon, p_sharpness, p_light_affect, p_ao_channel_affect);
};

void FilamentRenderingServer::environment_set_ssao_quality(EnvironmentSSAOQuality p_quality, bool p_half_size, float p_adaptive_target, int p_blur_passes, float p_fadeout_from, float p_fadeout_to)  {
	return execute<void, EnvironmentSSAOQuality, bool, float, int, float, float>(&FilamentRenderingServerBackend::environment_set_ssao_quality, p_quality, p_half_size, p_adaptive_target, p_blur_passes, p_fadeout_from, p_fadeout_to);
};

void FilamentRenderingServer::environment_set_ssil(RID p_env, bool p_enable, float p_radius, float p_intensity, float p_sharpness, float p_normal_rejection)  {
	return execute<void, RID, bool, float, float, float, float>(&FilamentRenderingServerBackend::environment_set_ssil, p_env, p_enable, p_radius, p_intensity, p_sharpness, p_normal_rejection);
};

void FilamentRenderingServer::environment_set_ssil_quality(EnvironmentSSILQuality p_quality, bool p_half_size, float p_adaptive_target, int p_blur_passes, float p_fadeout_from, float p_fadeout_to)  {
	return execute<void, EnvironmentSSILQuality, bool, float, int, float, float>(&FilamentRenderingServerBackend::environment_set_ssil_quality, p_quality, p_half_size, p_adaptive_target, p_blur_passes, p_fadeout_from, p_fadeout_to);
};

void FilamentRenderingServer::environment_set_sdfgi(RID p_env, bool p_enable, int p_cascades, float p_min_cell_size, EnvironmentSDFGIYScale p_y_scale, bool p_use_occlusion, float p_bounce_feedback, bool p_read_sky, float p_energy, float p_normal_bias, float p_probe_bias)  {
	return execute<void, RID, bool, int, float, EnvironmentSDFGIYScale, bool, float, bool, float, float, float>(&FilamentRenderingServerBackend::environment_set_sdfgi, p_env, p_enable, p_cascades, p_min_cell_size, p_y_scale, p_use_occlusion, p_bounce_feedback, p_read_sky, p_energy, p_normal_bias, p_probe_bias);
};

void FilamentRenderingServer::environment_set_sdfgi_ray_count(EnvironmentSDFGIRayCount p_ray_count)  {
	return execute<void, EnvironmentSDFGIRayCount>(&FilamentRenderingServerBackend::environment_set_sdfgi_ray_count, p_ray_count);
};

void FilamentRenderingServer::environment_set_sdfgi_frames_to_converge(EnvironmentSDFGIFramesToConverge p_frames)  {
	return execute<void, EnvironmentSDFGIFramesToConverge>(&FilamentRenderingServerBackend::environment_set_sdfgi_frames_to_converge, p_frames);
};

void FilamentRenderingServer::environment_set_sdfgi_frames_to_update_light(EnvironmentSDFGIFramesToUpdateLight p_update)  {
	return execute<void, EnvironmentSDFGIFramesToUpdateLight>(&FilamentRenderingServerBackend::environment_set_sdfgi_frames_to_update_light, p_update);
};

void FilamentRenderingServer::environment_set_fog(RID p_env, bool p_enable, const Color & p_light_color, float p_light_energy, float p_sun_scatter, float p_density, float p_height, float p_height_density, float p_aerial_perspective, float p_sky_affect)  {
	return execute<void, RID, bool, const Color &, float, float, float, float, float, float, float>(&FilamentRenderingServerBackend::environment_set_fog, p_env, p_enable, p_light_color, p_light_energy, p_sun_scatter, p_density, p_height, p_height_density, p_aerial_perspective, p_sky_affect);
};

void FilamentRenderingServer::environment_set_volumetric_fog(RID p_env, bool p_enable, float p_density, const Color & p_albedo, const Color & p_emission, float p_emission_energy, float p_anisotropy, float p_length, float p_detail_spread, float p_gi_inject, bool p_temporal_reprojection, float p_temporal_reprojection_amount, float p_ambient_inject, float p_sky_affect)  {
	return execute<void, RID, bool, float, const Color &, const Color &, float, float, float, float, float, bool, float, float, float>(&FilamentRenderingServerBackend::environment_set_volumetric_fog, p_env, p_enable, p_density, p_albedo, p_emission, p_emission_energy, p_anisotropy, p_length, p_detail_spread, p_gi_inject, p_temporal_reprojection, p_temporal_reprojection_amount, p_ambient_inject, p_sky_affect);
};

void FilamentRenderingServer::environment_set_volumetric_fog_volume_size(int p_size, int p_depth)  {
	return execute<void, int, int>(&FilamentRenderingServerBackend::environment_set_volumetric_fog_volume_size, p_size, p_depth);
};

void FilamentRenderingServer::environment_set_volumetric_fog_filter_active(bool p_enable)  {
	return execute<void, bool>(&FilamentRenderingServerBackend::environment_set_volumetric_fog_filter_active, p_enable);
};

Ref<Image> FilamentRenderingServer::environment_bake_panorama(RID p_env, bool p_bake_irradiance, const Size2i & p_size)  {
	return execute<Ref<Image>, RID, bool, const Size2i &>(&FilamentRenderingServerBackend::environment_bake_panorama, p_env, p_bake_irradiance, p_size);
};

void FilamentRenderingServer::screen_space_roughness_limiter_set_active(bool p_enable, float p_amount, float p_limit)  {
	return execute<void, bool, float, float>(&FilamentRenderingServerBackend::screen_space_roughness_limiter_set_active, p_enable, p_amount, p_limit);
};

void FilamentRenderingServer::sub_surface_scattering_set_quality(SubSurfaceScatteringQuality p_quality)  {
	return execute<void, SubSurfaceScatteringQuality>(&FilamentRenderingServerBackend::sub_surface_scattering_set_quality, p_quality);
};

void FilamentRenderingServer::sub_surface_scattering_set_scale(float p_scale, float p_depth_scale)  {
	return execute<void, float, float>(&FilamentRenderingServerBackend::sub_surface_scattering_set_scale, p_scale, p_depth_scale);
};

RID FilamentRenderingServer::camera_attributes_create()  {

	auto output = FilamentObjectManager::allocate();

	execute<void, RID>(&FilamentRenderingServerBackend::camera_attributes_create, output);

	return output;
};

void FilamentRenderingServer::camera_attributes_set_dof_blur_quality(DOFBlurQuality p_quality, bool p_use_jitter)  {
	return execute<void, DOFBlurQuality, bool>(&FilamentRenderingServerBackend::camera_attributes_set_dof_blur_quality, p_quality, p_use_jitter);
};

void FilamentRenderingServer::camera_attributes_set_dof_blur_bokeh_shape(DOFBokehShape p_shape)  {
	return execute<void, DOFBokehShape>(&FilamentRenderingServerBackend::camera_attributes_set_dof_blur_bokeh_shape, p_shape);
};

void FilamentRenderingServer::camera_attributes_set_dof_blur(RID p_camera_attributes, bool p_far_enable, float p_far_distance, float p_far_transition, bool p_near_enable, float p_near_distance, float p_near_transition, float p_amount)  {
	return execute<void, RID, bool, float, float, bool, float, float, float>(&FilamentRenderingServerBackend::camera_attributes_set_dof_blur, p_camera_attributes, p_far_enable, p_far_distance, p_far_transition, p_near_enable, p_near_distance, p_near_transition, p_amount);
};

void FilamentRenderingServer::camera_attributes_set_exposure(RID p_camera_attributes, float p_multiplier, float p_exposure_normalization)  {
	return execute<void, RID, float, float>(&FilamentRenderingServerBackend::camera_attributes_set_exposure, p_camera_attributes, p_multiplier, p_exposure_normalization);
};

void FilamentRenderingServer::camera_attributes_set_auto_exposure(RID p_camera_attributes, bool p_enable, float p_min_sensitivity, float p_max_sensitivity, float p_speed, float p_scale)  {
	return execute<void, RID, bool, float, float, float, float>(&FilamentRenderingServerBackend::camera_attributes_set_auto_exposure, p_camera_attributes, p_enable, p_min_sensitivity, p_max_sensitivity, p_speed, p_scale);
};

RID FilamentRenderingServer::scenario_create()  {

	auto output = FilamentObjectManager::allocate();

	execute<void, RID>(&FilamentRenderingServerBackend::scenario_create, output);

	return output;
};

void FilamentRenderingServer::scenario_set_environment(RID p_scenario, RID p_environment)  {
	return execute<void, RID, RID>(&FilamentRenderingServerBackend::scenario_set_environment, p_scenario, p_environment);
};

void FilamentRenderingServer::scenario_set_fallback_environment(RID p_scenario, RID p_environment)  {
	return execute<void, RID, RID>(&FilamentRenderingServerBackend::scenario_set_fallback_environment, p_scenario, p_environment);
};

void FilamentRenderingServer::scenario_set_camera_attributes(RID p_scenario, RID p_camera_attributes)  {
	return execute<void, RID, RID>(&FilamentRenderingServerBackend::scenario_set_camera_attributes, p_scenario, p_camera_attributes);
};

RID FilamentRenderingServer::instance_create()  {

	auto output = FilamentObjectManager::allocate();

	execute<void, RID>(&FilamentRenderingServerBackend::instance_create, output);

	return output;
};

void FilamentRenderingServer::instance_set_base(RID p_instance, RID p_base)  {
	return execute<void, RID, RID>(&FilamentRenderingServerBackend::instance_set_base, p_instance, p_base);
};

void FilamentRenderingServer::instance_set_scenario(RID p_instance, RID p_scenario)  {
	return execute<void, RID, RID>(&FilamentRenderingServerBackend::instance_set_scenario, p_instance, p_scenario);
};

void FilamentRenderingServer::instance_set_layer_mask(RID p_instance, uint32_t p_mask)  {
	return execute<void, RID, uint32_t>(&FilamentRenderingServerBackend::instance_set_layer_mask, p_instance, p_mask);
};

void FilamentRenderingServer::instance_set_pivot_data(RID p_instance, float p_sorting_offset, bool p_use_aabb_center)  {
	return execute<void, RID, float, bool>(&FilamentRenderingServerBackend::instance_set_pivot_data, p_instance, p_sorting_offset, p_use_aabb_center);
};

void FilamentRenderingServer::instance_set_transform(RID p_instance, const Transform3D & p_transform)  {
	return execute<void, RID, const Transform3D &>(&FilamentRenderingServerBackend::instance_set_transform, p_instance, p_transform);
};

void FilamentRenderingServer::instance_attach_object_instance_id(RID p_instance, ObjectID p_id)  {
	return execute<void, RID, ObjectID>(&FilamentRenderingServerBackend::instance_attach_object_instance_id, p_instance, p_id);
};

void FilamentRenderingServer::instance_set_blend_shape_weight(RID p_instance, int p_shape, float p_weight)  {
	return execute<void, RID, int, float>(&FilamentRenderingServerBackend::instance_set_blend_shape_weight, p_instance, p_shape, p_weight);
};

void FilamentRenderingServer::instance_set_surface_override_material(RID p_instance, int p_surface, RID p_material)  {
	return execute<void, RID, int, RID>(&FilamentRenderingServerBackend::instance_set_surface_override_material, p_instance, p_surface, p_material);
};

void FilamentRenderingServer::instance_set_visible(RID p_instance, bool p_visible)  {
	return execute<void, RID, bool>(&FilamentRenderingServerBackend::instance_set_visible, p_instance, p_visible);
};

void FilamentRenderingServer::instance_set_custom_aabb(RID p_instance, AABB aabb)  {
	return execute<void, RID, AABB>(&FilamentRenderingServerBackend::instance_set_custom_aabb, p_instance, aabb);
};

void FilamentRenderingServer::instance_attach_skeleton(RID p_instance, RID p_skeleton)  {
	return execute<void, RID, RID>(&FilamentRenderingServerBackend::instance_attach_skeleton, p_instance, p_skeleton);
};

void FilamentRenderingServer::instance_set_extra_visibility_margin(RID p_instance, real_t p_margin)  {
	return execute<void, RID, real_t>(&FilamentRenderingServerBackend::instance_set_extra_visibility_margin, p_instance, p_margin);
};

void FilamentRenderingServer::instance_set_visibility_parent(RID p_instance, RID p_parent_instance)  {
	return execute<void, RID, RID>(&FilamentRenderingServerBackend::instance_set_visibility_parent, p_instance, p_parent_instance);
};

void FilamentRenderingServer::instance_set_ignore_culling(RID p_instance, bool p_enabled)  {
	return execute<void, RID, bool>(&FilamentRenderingServerBackend::instance_set_ignore_culling, p_instance, p_enabled);
};

Vector<ObjectID> FilamentRenderingServer::instances_cull_aabb(const AABB & p_aabb, RID p_scenario) const {
	return execute<Vector<ObjectID>, const AABB &, RID>(&FilamentRenderingServerBackend::instances_cull_aabb, p_aabb, p_scenario);
};

Vector<ObjectID> FilamentRenderingServer::instances_cull_ray(const Vector3 & p_from, const Vector3 & p_to, RID p_scenario) const {
	return execute<Vector<ObjectID>, const Vector3 &, const Vector3 &, RID>(&FilamentRenderingServerBackend::instances_cull_ray, p_from, p_to, p_scenario);
};

Vector<ObjectID> FilamentRenderingServer::instances_cull_convex(const Vector<Plane> & p_convex, RID p_scenario) const {
	return execute<Vector<ObjectID>, const Vector<Plane> &, RID>(&FilamentRenderingServerBackend::instances_cull_convex, p_convex, p_scenario);
};

void FilamentRenderingServer::instance_geometry_set_flag(RID p_instance, InstanceFlags p_flags, bool p_enabled)  {
	return execute<void, RID, InstanceFlags, bool>(&FilamentRenderingServerBackend::instance_geometry_set_flag, p_instance, p_flags, p_enabled);
};

void FilamentRenderingServer::instance_geometry_set_cast_shadows_setting(RID p_instance, ShadowCastingSetting p_shadow_casting_setting)  {
	return execute<void, RID, ShadowCastingSetting>(&FilamentRenderingServerBackend::instance_geometry_set_cast_shadows_setting, p_instance, p_shadow_casting_setting);
};

void FilamentRenderingServer::instance_geometry_set_material_override(RID p_instance, RID p_material)  {
	return execute<void, RID, RID>(&FilamentRenderingServerBackend::instance_geometry_set_material_override, p_instance, p_material);
};

void FilamentRenderingServer::instance_geometry_set_material_overlay(RID p_instance, RID p_material)  {
	return execute<void, RID, RID>(&FilamentRenderingServerBackend::instance_geometry_set_material_overlay, p_instance, p_material);
};

void FilamentRenderingServer::instance_geometry_set_visibility_range(RID p_instance, float p_min, float p_max, float p_min_margin, float p_max_margin, VisibilityRangeFadeMode p_fade_mode)  {
	return execute<void, RID, float, float, float, float, VisibilityRangeFadeMode>(&FilamentRenderingServerBackend::instance_geometry_set_visibility_range, p_instance, p_min, p_max, p_min_margin, p_max_margin, p_fade_mode);
};

void FilamentRenderingServer::instance_geometry_set_lightmap(RID p_instance, RID p_lightmap, const Rect2 & p_lightmap_uv_scale, int p_lightmap_slice)  {
	return execute<void, RID, RID, const Rect2 &, int>(&FilamentRenderingServerBackend::instance_geometry_set_lightmap, p_instance, p_lightmap, p_lightmap_uv_scale, p_lightmap_slice);
};

void FilamentRenderingServer::instance_geometry_set_lod_bias(RID p_instance, float p_lod_bias)  {
	return execute<void, RID, float>(&FilamentRenderingServerBackend::instance_geometry_set_lod_bias, p_instance, p_lod_bias);
};

void FilamentRenderingServer::instance_geometry_set_transparency(RID p_instance, float p_transparency)  {
	return execute<void, RID, float>(&FilamentRenderingServerBackend::instance_geometry_set_transparency, p_instance, p_transparency);
};

void FilamentRenderingServer::instance_geometry_set_shader_parameter(RID p_instance, const StringName & anonarg, const Variant & p_value)  {
	return execute<void, RID, const StringName &, const Variant &>(&FilamentRenderingServerBackend::instance_geometry_set_shader_parameter, p_instance, anonarg, p_value);
};

Variant FilamentRenderingServer::instance_geometry_get_shader_parameter(RID p_instance, const StringName & anonarg) const {
	return execute<Variant, RID, const StringName &>(&FilamentRenderingServerBackend::instance_geometry_get_shader_parameter, p_instance, anonarg);
};

Variant FilamentRenderingServer::instance_geometry_get_shader_parameter_default_value(RID p_instance, const StringName & anonarg) const {
	return execute<Variant, RID, const StringName &>(&FilamentRenderingServerBackend::instance_geometry_get_shader_parameter_default_value, p_instance, anonarg);
};

void FilamentRenderingServer::instance_geometry_get_shader_parameter_list(RID p_instance, List<PropertyInfo> * p_parameters) const {
	return execute<void, RID, List<PropertyInfo> *>(&FilamentRenderingServerBackend::instance_geometry_get_shader_parameter_list, p_instance, p_parameters);
};

TypedArray<Image> FilamentRenderingServer::bake_render_uv2(RID p_base, const TypedArray<RID> & p_material_overrides, const Size2i & p_image_size)  {
	return execute<TypedArray<Image>, RID, const TypedArray<RID> &, const Size2i &>(&FilamentRenderingServerBackend::bake_render_uv2, p_base, p_material_overrides, p_image_size);
};

RID FilamentRenderingServer::canvas_create()  {

	auto output = FilamentObjectManager::allocate();

	execute<RID>(&FilamentRenderingServerBackend::canvas_create, output);

	return output;
};

void FilamentRenderingServer::canvas_set_item_mirroring(RID p_canvas, RID p_item, const Point2 & p_mirroring)  {
	return execute<void, RID, RID, const Point2 &>(&FilamentRenderingServerBackend::canvas_set_item_mirroring, p_canvas, p_item, p_mirroring);
};

void FilamentRenderingServer::canvas_set_modulate(RID p_canvas, const Color & p_color)  {
	return execute<void, RID, const Color &>(&FilamentRenderingServerBackend::canvas_set_modulate, p_canvas, p_color);
};

void FilamentRenderingServer::canvas_set_parent(RID p_canvas, RID p_parent, float p_scale)  {
	return execute<void, RID, RID, float>(&FilamentRenderingServerBackend::canvas_set_parent, p_canvas, p_parent, p_scale);
};

void FilamentRenderingServer::canvas_set_disable_scale(bool p_disable)  {
	return execute<void, bool>(&FilamentRenderingServerBackend::canvas_set_disable_scale, p_disable);
};

RID FilamentRenderingServer::canvas_texture_create()  {

	auto output = FilamentObjectManager::allocate();

	execute<void, RID>(&FilamentRenderingServerBackend::canvas_texture_create, output);

	return output;
};

void FilamentRenderingServer::canvas_texture_set_channel(RID p_canvas_texture, CanvasTextureChannel p_channel, RID p_texture)  {
	return execute<void, RID, CanvasTextureChannel, RID>(&FilamentRenderingServerBackend::canvas_texture_set_channel, p_canvas_texture, p_channel, p_texture);
};

void FilamentRenderingServer::canvas_texture_set_shading_parameters(RID p_canvas_texture, const Color & p_base_color, float p_shininess)  {
	return execute<void, RID, const Color &, float>(&FilamentRenderingServerBackend::canvas_texture_set_shading_parameters, p_canvas_texture, p_base_color, p_shininess);
};

void FilamentRenderingServer::canvas_texture_set_texture_filter(RID p_canvas_texture, CanvasItemTextureFilter p_filter)  {
	return execute<void, RID, CanvasItemTextureFilter>(&FilamentRenderingServerBackend::canvas_texture_set_texture_filter, p_canvas_texture, p_filter);
};

void FilamentRenderingServer::canvas_texture_set_texture_repeat(RID p_canvas_texture, CanvasItemTextureRepeat p_repeat)  {
	return execute<void, RID, CanvasItemTextureRepeat>(&FilamentRenderingServerBackend::canvas_texture_set_texture_repeat, p_canvas_texture, p_repeat);
};

RID FilamentRenderingServer::canvas_item_create()  {
	auto output = FilamentObjectManager::allocate();

	execute<void, RID>(&FilamentRenderingServerBackend::canvas_item_create, output);

	return output;
};

void FilamentRenderingServer::canvas_item_set_parent(RID p_item, RID p_parent)  {
	return execute<void, RID, RID>(&FilamentRenderingServerBackend::canvas_item_set_parent, p_item, p_parent);
};

void FilamentRenderingServer::canvas_item_set_default_texture_filter(RID p_item, CanvasItemTextureFilter p_filter)  {
	return execute<void, RID, CanvasItemTextureFilter>(&FilamentRenderingServerBackend::canvas_item_set_default_texture_filter, p_item, p_filter);
};

void FilamentRenderingServer::canvas_item_set_default_texture_repeat(RID p_item, CanvasItemTextureRepeat p_repeat)  {
	return execute<void, RID, CanvasItemTextureRepeat>(&FilamentRenderingServerBackend::canvas_item_set_default_texture_repeat, p_item, p_repeat);
};

void FilamentRenderingServer::canvas_item_set_visible(RID p_item, bool p_visible)  {
	return execute<void, RID, bool>(&FilamentRenderingServerBackend::canvas_item_set_visible, p_item, p_visible);
};

void FilamentRenderingServer::canvas_item_set_light_mask(RID p_item, int p_mask)  {
	return execute<void, RID, int>(&FilamentRenderingServerBackend::canvas_item_set_light_mask, p_item, p_mask);
};

void FilamentRenderingServer::canvas_item_set_update_when_visible(RID p_item, bool p_update)  {
	return execute<void, RID, bool>(&FilamentRenderingServerBackend::canvas_item_set_update_when_visible, p_item, p_update);
};

void FilamentRenderingServer::canvas_item_set_transform(RID p_item, const Transform2D & p_transform)  {
	return execute<void, RID, const Transform2D &>(&FilamentRenderingServerBackend::canvas_item_set_transform, p_item, p_transform);
};

void FilamentRenderingServer::canvas_item_set_clip(RID p_item, bool p_clip)  {
	return execute<void, RID, bool>(&FilamentRenderingServerBackend::canvas_item_set_clip, p_item, p_clip);
};

void FilamentRenderingServer::canvas_item_set_distance_field_mode(RID p_item, bool p_enable)  {
	return execute<void, RID, bool>(&FilamentRenderingServerBackend::canvas_item_set_distance_field_mode, p_item, p_enable);
};

void FilamentRenderingServer::canvas_item_set_custom_rect(RID p_item, bool p_custom_rect, const Rect2 & p_rect)  {
	return execute<void, RID, bool, const Rect2 &>(&FilamentRenderingServerBackend::canvas_item_set_custom_rect, p_item, p_custom_rect, p_rect);
};

void FilamentRenderingServer::canvas_item_set_modulate(RID p_item, const Color & p_color)  {
	return execute<void, RID, const Color &>(&FilamentRenderingServerBackend::canvas_item_set_modulate, p_item, p_color);
};

void FilamentRenderingServer::canvas_item_set_self_modulate(RID p_item, const Color & p_color)  {
	return execute<void, RID, const Color &>(&FilamentRenderingServerBackend::canvas_item_set_self_modulate, p_item, p_color);
};

void FilamentRenderingServer::canvas_item_set_visibility_layer(RID p_item, uint32_t p_visibility_layer)  {
	return execute<void, RID, uint32_t>(&FilamentRenderingServerBackend::canvas_item_set_visibility_layer, p_item, p_visibility_layer);
};

void FilamentRenderingServer::canvas_item_set_draw_behind_parent(RID p_item, bool p_enable)  {
	return execute<void, RID, bool>(&FilamentRenderingServerBackend::canvas_item_set_draw_behind_parent, p_item, p_enable);
};

void FilamentRenderingServer::canvas_item_add_line(RID p_item, const Point2 & p_from, const Point2 & p_to, const Color & p_color, float p_width, bool p_antialiased)  {
	return execute<void, RID, const Point2 &, const Point2 &, const Color &, float, bool>(&FilamentRenderingServerBackend::canvas_item_add_line, p_item, p_from, p_to, p_color, p_width, p_antialiased);
};

void FilamentRenderingServer::canvas_item_add_polyline(RID p_item, const Vector<Point2> & p_points, const Vector<Color> & p_colors, float p_width, bool p_antialiased)  {
	return execute<void, RID, const Vector<Point2> &, const Vector<Color> &, float, bool>(&FilamentRenderingServerBackend::canvas_item_add_polyline, p_item, p_points, p_colors, p_width, p_antialiased);
};

void FilamentRenderingServer::canvas_item_add_multiline(RID p_item, const Vector<Point2> & p_points, const Vector<Color> & p_colors, float p_width)  {
	return execute<void, RID, const Vector<Point2> &, const Vector<Color> &, float>(&FilamentRenderingServerBackend::canvas_item_add_multiline, p_item, p_points, p_colors, p_width);
};

void FilamentRenderingServer::canvas_item_add_rect(RID p_item, const Rect2 & p_rect, const Color & p_color)  {
	return execute<void, RID, const Rect2 &, const Color &>(&FilamentRenderingServerBackend::canvas_item_add_rect, p_item, p_rect, p_color);
};

void FilamentRenderingServer::canvas_item_add_circle(RID p_item, const Point2 & p_pos, float p_radius, const Color & p_color)  {
	return execute<void, RID, const Point2 &, float, const Color &>(&FilamentRenderingServerBackend::canvas_item_add_circle, p_item, p_pos, p_radius, p_color);
};

void FilamentRenderingServer::canvas_item_add_texture_rect(RID p_item, const Rect2 & p_rect, RID p_texture, bool p_tile, const Color & p_modulate, bool p_transpose)  {
	return execute<void, RID, const Rect2 &, RID, bool, const Color &, bool>(&FilamentRenderingServerBackend::canvas_item_add_texture_rect, p_item, p_rect, p_texture, p_tile, p_modulate, p_transpose);
};

void FilamentRenderingServer::canvas_item_add_texture_rect_region(RID p_item, const Rect2 & p_rect, RID p_texture, const Rect2 & p_src_rect, const Color & p_modulate, bool p_transpose, bool p_clip_uv)  {
	return execute<void, RID, const Rect2 &, RID, const Rect2 &, const Color &, bool, bool>(&FilamentRenderingServerBackend::canvas_item_add_texture_rect_region, p_item, p_rect, p_texture, p_src_rect, p_modulate, p_transpose, p_clip_uv);
};

void FilamentRenderingServer::canvas_item_add_msdf_texture_rect_region(RID p_item, const Rect2 & p_rect, RID p_texture, const Rect2 & p_src_rect, const Color & p_modulate, int p_outline_size, float p_px_range, float p_scale)  {
	return execute<void, RID, const Rect2 &, RID, const Rect2 &, const Color &, int, float, float>(&FilamentRenderingServerBackend::canvas_item_add_msdf_texture_rect_region, p_item, p_rect, p_texture, p_src_rect, p_modulate, p_outline_size, p_px_range, p_scale);
};

void FilamentRenderingServer::canvas_item_add_lcd_texture_rect_region(RID p_item, const Rect2 & p_rect, RID p_texture, const Rect2 & p_src_rect, const Color & p_modulate)  {
	return execute<void, RID, const Rect2 &, RID, const Rect2 &, const Color &>(&FilamentRenderingServerBackend::canvas_item_add_lcd_texture_rect_region, p_item, p_rect, p_texture, p_src_rect, p_modulate);
};

void FilamentRenderingServer::canvas_item_add_nine_patch(RID p_item, const Rect2 & p_rect, const Rect2 & p_source, RID p_texture, const Vector2 & p_topleft, const Vector2 & p_bottomright, NinePatchAxisMode p_x_axis_mode, NinePatchAxisMode p_y_axis_mode, bool p_draw_center, const Color & p_modulate)  {
	return execute<void, RID, const Rect2 &, const Rect2 &, RID, const Vector2 &, const Vector2 &, NinePatchAxisMode, NinePatchAxisMode, bool, const Color &>(&FilamentRenderingServerBackend::canvas_item_add_nine_patch, p_item, p_rect, p_source, p_texture, p_topleft, p_bottomright, p_x_axis_mode, p_y_axis_mode, p_draw_center, p_modulate);
};

void FilamentRenderingServer::canvas_item_add_primitive(RID p_item, const Vector<Point2> & p_points, const Vector<Color> & p_colors, const Vector<Point2> & p_uvs, RID p_texture)  {
	return execute<void, RID, const Vector<Point2> &, const Vector<Color> &, const Vector<Point2> &, RID>(&FilamentRenderingServerBackend::canvas_item_add_primitive, p_item, p_points, p_colors, p_uvs, p_texture);
};

void FilamentRenderingServer::canvas_item_add_polygon(RID p_item, const Vector<Point2> & p_points, const Vector<Color> & p_colors, const Vector<Point2> & p_uvs, RID p_texture)  {
	return execute<void, RID, const Vector<Point2> &, const Vector<Color> &, const Vector<Point2> &, RID>(&FilamentRenderingServerBackend::canvas_item_add_polygon, p_item, p_points, p_colors, p_uvs, p_texture);
};

void FilamentRenderingServer::canvas_item_add_triangle_array(RID p_item, const Vector<int> & p_indices, const Vector<Point2> & p_points, const Vector<Color> & p_colors, const Vector<Point2> & p_uvs, const Vector<int> & p_bones, const Vector<float> & p_weights, RID p_texture, int p_count)  {
	return execute<void, RID, const Vector<int> &, const Vector<Point2> &, const Vector<Color> &, const Vector<Point2> &, const Vector<int> &, const Vector<float> &, RID, int>(&FilamentRenderingServerBackend::canvas_item_add_triangle_array, p_item, p_indices, p_points, p_colors, p_uvs, p_bones, p_weights, p_texture, p_count);
};

void FilamentRenderingServer::canvas_item_add_mesh(RID p_item, const RID & p_mesh, const Transform2D & p_transform, const Color & p_modulate, RID p_texture)  {
	return execute<void, RID, const RID &, const Transform2D &, const Color &, RID>(&FilamentRenderingServerBackend::canvas_item_add_mesh, p_item, p_mesh, p_transform, p_modulate, p_texture);
};

void FilamentRenderingServer::canvas_item_add_multimesh(RID p_item, RID p_mesh, RID p_texture)  {
	return execute<void, RID, RID, RID>(&FilamentRenderingServerBackend::canvas_item_add_multimesh, p_item, p_mesh, p_texture);
};

void FilamentRenderingServer::canvas_item_add_particles(RID p_item, RID p_particles, RID p_texture)  {
	return execute<void, RID, RID, RID>(&FilamentRenderingServerBackend::canvas_item_add_particles, p_item, p_particles, p_texture);
};

void FilamentRenderingServer::canvas_item_add_set_transform(RID p_item, const Transform2D & p_transform)  {
	return execute<void, RID, const Transform2D &>(&FilamentRenderingServerBackend::canvas_item_add_set_transform, p_item, p_transform);
};

void FilamentRenderingServer::canvas_item_add_clip_ignore(RID p_item, bool p_ignore)  {
	return execute<void, RID, bool>(&FilamentRenderingServerBackend::canvas_item_add_clip_ignore, p_item, p_ignore);
};

void FilamentRenderingServer::canvas_item_add_animation_slice(RID p_item, double p_animation_length, double p_slice_begin, double p_slice_end, double p_offset)  {
	return execute<void, RID, double, double, double, double>(&FilamentRenderingServerBackend::canvas_item_add_animation_slice, p_item, p_animation_length, p_slice_begin, p_slice_end, p_offset);
};

void FilamentRenderingServer::canvas_item_set_sort_children_by_y(RID p_item, bool p_enable)  {
	return execute<void, RID, bool>(&FilamentRenderingServerBackend::canvas_item_set_sort_children_by_y, p_item, p_enable);
};

void FilamentRenderingServer::canvas_item_set_z_index(RID p_item, int p_z)  {
	return execute<void, RID, int>(&FilamentRenderingServerBackend::canvas_item_set_z_index, p_item, p_z);
};

void FilamentRenderingServer::canvas_item_set_z_as_relative_to_parent(RID p_item, bool p_enable)  {
	return execute<void, RID, bool>(&FilamentRenderingServerBackend::canvas_item_set_z_as_relative_to_parent, p_item, p_enable);
};

void FilamentRenderingServer::canvas_item_set_copy_to_backbuffer(RID p_item, bool p_enable, const Rect2 & p_rect)  {
	return execute<void, RID, bool, const Rect2 &>(&FilamentRenderingServerBackend::canvas_item_set_copy_to_backbuffer, p_item, p_enable, p_rect);
};

void FilamentRenderingServer::canvas_item_attach_skeleton(RID p_item, RID p_skeleton)  {
	return execute<void, RID, RID>(&FilamentRenderingServerBackend::canvas_item_attach_skeleton, p_item, p_skeleton);
};

void FilamentRenderingServer::canvas_item_clear(RID p_item)  {
	return execute<void, RID>(&FilamentRenderingServerBackend::canvas_item_clear, p_item);
};

void FilamentRenderingServer::canvas_item_set_draw_index(RID p_item, int p_index)  {
	return execute<void, RID, int>(&FilamentRenderingServerBackend::canvas_item_set_draw_index, p_item, p_index);
};

void FilamentRenderingServer::canvas_item_set_material(RID p_item, RID p_material)  {
	return execute<void, RID, RID>(&FilamentRenderingServerBackend::canvas_item_set_material, p_item, p_material);
};

void FilamentRenderingServer::canvas_item_set_use_parent_material(RID p_item, bool p_enable)  {
	return execute<void, RID, bool>(&FilamentRenderingServerBackend::canvas_item_set_use_parent_material, p_item, p_enable);
};

void FilamentRenderingServer::canvas_item_set_visibility_notifier(RID p_item, bool p_enable, const Rect2 & p_area, const Callable & p_enter_callbable, const Callable & p_exit_callable)  {
	return execute<void, RID, bool, const Rect2 &, const Callable &, const Callable &>(&FilamentRenderingServerBackend::canvas_item_set_visibility_notifier, p_item, p_enable, p_area, p_enter_callbable, p_exit_callable);
};

void FilamentRenderingServer::canvas_item_set_canvas_group_mode(RID p_item, CanvasGroupMode p_mode, float p_clear_margin, bool p_fit_empty, float p_fit_margin, bool p_blur_mipmaps)  {
	return execute<void, RID, CanvasGroupMode, float, bool, float, bool>(&FilamentRenderingServerBackend::canvas_item_set_canvas_group_mode, p_item, p_mode, p_clear_margin, p_fit_empty, p_fit_margin, p_blur_mipmaps);
};

void FilamentRenderingServer::canvas_item_set_debug_redraw(bool p_enabled)  {
	return execute<void, bool>(&FilamentRenderingServerBackend::canvas_item_set_debug_redraw, p_enabled);
};

bool FilamentRenderingServer::canvas_item_get_debug_redraw() const {
	return execute<bool>(&FilamentRenderingServerBackend::canvas_item_get_debug_redraw);
};

RID FilamentRenderingServer::canvas_light_create()  {

	auto output = FilamentObjectManager::allocate();

	execute<void, RID>(&FilamentRenderingServerBackend::canvas_light_create, output);

	return output;
};

void FilamentRenderingServer::canvas_light_set_mode(RID p_light, CanvasLightMode p_mode)  {
	return execute<void, RID, CanvasLightMode>(&FilamentRenderingServerBackend::canvas_light_set_mode, p_light, p_mode);
};

void FilamentRenderingServer::canvas_light_attach_to_canvas(RID p_light, RID p_canvas)  {
	return execute<void, RID, RID>(&FilamentRenderingServerBackend::canvas_light_attach_to_canvas, p_light, p_canvas);
};

void FilamentRenderingServer::canvas_light_set_enabled(RID p_light, bool p_enabled)  {
	return execute<void, RID, bool>(&FilamentRenderingServerBackend::canvas_light_set_enabled, p_light, p_enabled);
};

void FilamentRenderingServer::canvas_light_set_transform(RID p_light, const Transform2D & p_transform)  {
	return execute<void, RID, const Transform2D &>(&FilamentRenderingServerBackend::canvas_light_set_transform, p_light, p_transform);
};

void FilamentRenderingServer::canvas_light_set_color(RID p_light, const Color & p_color)  {
	return execute<void, RID, const Color &>(&FilamentRenderingServerBackend::canvas_light_set_color, p_light, p_color);
};

void FilamentRenderingServer::canvas_light_set_height(RID p_light, float p_height)  {
	return execute<void, RID, float>(&FilamentRenderingServerBackend::canvas_light_set_height, p_light, p_height);
};

void FilamentRenderingServer::canvas_light_set_energy(RID p_light, float p_energy)  {
	return execute<void, RID, float>(&FilamentRenderingServerBackend::canvas_light_set_energy, p_light, p_energy);
};

void FilamentRenderingServer::canvas_light_set_z_range(RID p_light, int p_min_z, int p_max_z)  {
	return execute<void, RID, int, int>(&FilamentRenderingServerBackend::canvas_light_set_z_range, p_light, p_min_z, p_max_z);
};

void FilamentRenderingServer::canvas_light_set_layer_range(RID p_light, int p_min_layer, int p_max_layer)  {
	return execute<void, RID, int, int>(&FilamentRenderingServerBackend::canvas_light_set_layer_range, p_light, p_min_layer, p_max_layer);
};

void FilamentRenderingServer::canvas_light_set_item_cull_mask(RID p_light, int p_mask)  {
	return execute<void, RID, int>(&FilamentRenderingServerBackend::canvas_light_set_item_cull_mask, p_light, p_mask);
};

void FilamentRenderingServer::canvas_light_set_item_shadow_cull_mask(RID p_light, int p_mask)  {
	return execute<void, RID, int>(&FilamentRenderingServerBackend::canvas_light_set_item_shadow_cull_mask, p_light, p_mask);
};

void FilamentRenderingServer::canvas_light_set_directional_distance(RID p_light, float p_distance)  {
	return execute<void, RID, float>(&FilamentRenderingServerBackend::canvas_light_set_directional_distance, p_light, p_distance);
};

void FilamentRenderingServer::canvas_light_set_texture_scale(RID p_light, float p_scale)  {
	return execute<void, RID, float>(&FilamentRenderingServerBackend::canvas_light_set_texture_scale, p_light, p_scale);
};

void FilamentRenderingServer::canvas_light_set_texture(RID p_light, RID p_texture)  {
	return execute<void, RID, RID>(&FilamentRenderingServerBackend::canvas_light_set_texture, p_light, p_texture);
};

void FilamentRenderingServer::canvas_light_set_texture_offset(RID p_light, const Vector2 & p_offset)  {
	return execute<void, RID, const Vector2 &>(&FilamentRenderingServerBackend::canvas_light_set_texture_offset, p_light, p_offset);
};

void FilamentRenderingServer::canvas_light_set_blend_mode(RID p_light, CanvasLightBlendMode p_mode)  {
	return execute<void, RID, CanvasLightBlendMode>(&FilamentRenderingServerBackend::canvas_light_set_blend_mode, p_light, p_mode);
};

void FilamentRenderingServer::canvas_light_set_shadow_enabled(RID p_light, bool p_enabled)  {
	return execute<void, RID, bool>(&FilamentRenderingServerBackend::canvas_light_set_shadow_enabled, p_light, p_enabled);
};

void FilamentRenderingServer::canvas_light_set_shadow_filter(RID p_light, CanvasLightShadowFilter p_filter)  {
	return execute<void, RID, CanvasLightShadowFilter>(&FilamentRenderingServerBackend::canvas_light_set_shadow_filter, p_light, p_filter);
};

void FilamentRenderingServer::canvas_light_set_shadow_color(RID p_light, const Color & p_color)  {
	return execute<void, RID, const Color &>(&FilamentRenderingServerBackend::canvas_light_set_shadow_color, p_light, p_color);
};

void FilamentRenderingServer::canvas_light_set_shadow_smooth(RID p_light, float p_smooth)  {
	return execute<void, RID, float>(&FilamentRenderingServerBackend::canvas_light_set_shadow_smooth, p_light, p_smooth);
};

RID FilamentRenderingServer::canvas_light_occluder_create()  {

	auto output = FilamentObjectManager::allocate();

	execute<void, RID>(&FilamentRenderingServerBackend::canvas_light_occluder_create, output);

	return output;
};

void FilamentRenderingServer::canvas_light_occluder_attach_to_canvas(RID p_occluder, RID p_canvas)  {
	return execute<void, RID, RID>(&FilamentRenderingServerBackend::canvas_light_occluder_attach_to_canvas, p_occluder, p_canvas);
};

void FilamentRenderingServer::canvas_light_occluder_set_enabled(RID p_occluder, bool p_enabled)  {
	return execute<void, RID, bool>(&FilamentRenderingServerBackend::canvas_light_occluder_set_enabled, p_occluder, p_enabled);
};

void FilamentRenderingServer::canvas_light_occluder_set_polygon(RID p_occluder, RID p_polygon)  {
	return execute<void, RID, RID>(&FilamentRenderingServerBackend::canvas_light_occluder_set_polygon, p_occluder, p_polygon);
};

void FilamentRenderingServer::canvas_light_occluder_set_as_sdf_collision(RID p_occluder, bool p_enable)  {
	return execute<void, RID, bool>(&FilamentRenderingServerBackend::canvas_light_occluder_set_as_sdf_collision, p_occluder, p_enable);
};

void FilamentRenderingServer::canvas_light_occluder_set_transform(RID p_occluder, const Transform2D & p_xform)  {
	return execute<void, RID, const Transform2D &>(&FilamentRenderingServerBackend::canvas_light_occluder_set_transform, p_occluder, p_xform);
};

void FilamentRenderingServer::canvas_light_occluder_set_light_mask(RID p_occluder, int p_mask)  {
	return execute<void, RID, int>(&FilamentRenderingServerBackend::canvas_light_occluder_set_light_mask, p_occluder, p_mask);
};

RID FilamentRenderingServer::canvas_occluder_polygon_create()  {

	auto output = FilamentObjectManager::allocate();

	execute<void, RID>(&FilamentRenderingServerBackend::canvas_occluder_polygon_create, output);

	return output;
};

void FilamentRenderingServer::canvas_occluder_polygon_set_shape(RID p_occluder_polygon, const Vector<Vector2> & p_shape, bool p_closed)  {
	return execute<void, RID, const Vector<Vector2> &, bool>(&FilamentRenderingServerBackend::canvas_occluder_polygon_set_shape, p_occluder_polygon, p_shape, p_closed);
};

void FilamentRenderingServer::canvas_occluder_polygon_set_cull_mode(RID p_occluder_polygon, CanvasOccluderPolygonCullMode p_mode)  {
	return execute<void, RID, CanvasOccluderPolygonCullMode>(&FilamentRenderingServerBackend::canvas_occluder_polygon_set_cull_mode, p_occluder_polygon, p_mode);
};

void FilamentRenderingServer::canvas_set_shadow_texture_size(int p_size)  {
	return execute<void, int>(&FilamentRenderingServerBackend::canvas_set_shadow_texture_size, p_size);
};

void FilamentRenderingServer::global_shader_parameter_add(const StringName & p_name, GlobalShaderParameterType p_type, const Variant & p_value)  {
	return execute<void, const StringName &, GlobalShaderParameterType, const Variant &>(&FilamentRenderingServerBackend::global_shader_parameter_add, p_name, p_type, p_value);
};

void FilamentRenderingServer::global_shader_parameter_remove(const StringName & p_name)  {
	return execute<void, const StringName &>(&FilamentRenderingServerBackend::global_shader_parameter_remove, p_name);
};

Vector<StringName> FilamentRenderingServer::global_shader_parameter_get_list() const {
	return execute<Vector<StringName>>(&FilamentRenderingServerBackend::global_shader_parameter_get_list);
};

void FilamentRenderingServer::global_shader_parameter_set(const StringName & p_name, const Variant & p_value)  {
	return execute<void, const StringName &, const Variant &>(&FilamentRenderingServerBackend::global_shader_parameter_set, p_name, p_value);
};

void FilamentRenderingServer::global_shader_parameter_set_override(const StringName & p_name, const Variant & p_value)  {
	return execute<void, const StringName &, const Variant &>(&FilamentRenderingServerBackend::global_shader_parameter_set_override, p_name, p_value);
};

Variant FilamentRenderingServer::global_shader_parameter_get(const StringName & p_name) const {
	return execute<Variant, const StringName &>(&FilamentRenderingServerBackend::global_shader_parameter_get, p_name);
};

RenderingServer::GlobalShaderParameterType FilamentRenderingServer::global_shader_parameter_get_type(const StringName & p_name) const {
	return execute<GlobalShaderParameterType, const StringName &>(&FilamentRenderingServerBackend::global_shader_parameter_get_type, p_name);
};

void FilamentRenderingServer::global_shader_parameters_load_settings(bool p_load_textures)  {
	return execute<void, bool>(&FilamentRenderingServerBackend::global_shader_parameters_load_settings, p_load_textures);
};

void FilamentRenderingServer::global_shader_parameters_clear()  {
	return execute<void>(&FilamentRenderingServerBackend::global_shader_parameters_clear);
};

void FilamentRenderingServer::free(RID p_rid)  {
	return execute<void, RID>(&FilamentRenderingServerBackend::free, p_rid);
};

void FilamentRenderingServer::request_frame_drawn_callback(const Callable & p_callable)  {
	return execute<void, const Callable &>(&FilamentRenderingServerBackend::request_frame_drawn_callback, p_callable);
};

void FilamentRenderingServer::draw(bool p_swap_buffers, double frame_step)  {
	return execute<void, bool, double>(&FilamentRenderingServerBackend::draw, p_swap_buffers, frame_step);
};

void FilamentRenderingServer::sync()  {
	/*
	 * Dummy return to make the call synchronous.
	 */
	execute<bool>(&FilamentRenderingServerBackend::sync);
};

bool FilamentRenderingServer::has_changed() const {
	/*
	 * This controls whether the main loop with draw a new frame, or only
	 * call sync(). Currently, we always request a frame, but this needs
	 * revision later.
	 */
	return true;//return execute<bool>(&FilamentRenderingServerBackend::has_changed);
};

void FilamentRenderingServer::finish()  {
	/*
	 * Shutdown request - we handle that in the destructor.
	 */
};

uint64_t FilamentRenderingServer::get_rendering_info(RenderingInfo p_info)  {
	return execute<uint64_t, RenderingInfo>(&FilamentRenderingServerBackend::get_rendering_info, p_info);
};

String FilamentRenderingServer::get_video_adapter_name() const {
	return execute<String>(&FilamentRenderingServerBackend::get_video_adapter_name);
};

String FilamentRenderingServer::get_video_adapter_vendor() const {
	return execute<String>(&FilamentRenderingServerBackend::get_video_adapter_vendor);
};

RenderingDevice::DeviceType FilamentRenderingServer::get_video_adapter_type() const {
	return execute<RenderingDevice::DeviceType>(&FilamentRenderingServerBackend::get_video_adapter_type);
};

String FilamentRenderingServer::get_video_adapter_api_version() const {
	return execute<String>(&FilamentRenderingServerBackend::get_video_adapter_api_version);
};

void FilamentRenderingServer::set_frame_profiling_enabled(bool p_enable)  {
	return execute<void, bool>(&FilamentRenderingServerBackend::set_frame_profiling_enabled, p_enable);
};

Vector<RenderingServer::FrameProfileArea> FilamentRenderingServer::get_frame_profile()  {
	return execute<Vector<FrameProfileArea>>(&FilamentRenderingServerBackend::get_frame_profile);
};

uint64_t FilamentRenderingServer::get_frame_profile_frame()  {
	return execute<uint64_t>(&FilamentRenderingServerBackend::get_frame_profile_frame);
};

double FilamentRenderingServer::get_frame_setup_time_cpu() const {
	return execute<double>(&FilamentRenderingServerBackend::get_frame_setup_time_cpu);
};

void FilamentRenderingServer::gi_set_use_half_resolution(bool p_enable)  {
	return execute<void, bool>(&FilamentRenderingServerBackend::gi_set_use_half_resolution, p_enable);
};

RID FilamentRenderingServer::get_test_cube()  {
	return execute<RID>(&FilamentRenderingServerBackend::get_test_cube);
};

void FilamentRenderingServer::sdfgi_set_debug_probe_select(const Vector3 & p_position, const Vector3 & p_dir)  {
	return execute<void, const Vector3 &, const Vector3 &>(&FilamentRenderingServerBackend::sdfgi_set_debug_probe_select, p_position, p_dir);
};

void FilamentRenderingServer::set_boot_image(const Ref<Image> & p_image, const Color & p_color, bool p_scale, bool p_use_filter)  {
	return execute<void, const Ref<Image> &, const Color &, bool, bool>(&FilamentRenderingServerBackend::set_boot_image, p_image, p_color, p_scale, p_use_filter);
};

Color FilamentRenderingServer::get_default_clear_color()  {
	return execute<Color>(&FilamentRenderingServerBackend::get_default_clear_color);
};

void FilamentRenderingServer::set_default_clear_color(const Color & p_color)  {
	return execute<void, const Color &>(&FilamentRenderingServerBackend::set_default_clear_color, p_color);
};

bool FilamentRenderingServer::has_feature(Features p_feature) const {
	return execute<bool, Features>(&FilamentRenderingServerBackend::has_feature, p_feature);
};

bool FilamentRenderingServer::has_os_feature(const String & p_feature) const {
	return execute<bool, const String &>(&FilamentRenderingServerBackend::has_os_feature, p_feature);
};

void FilamentRenderingServer::set_debug_generate_wireframes(bool p_generate)  {
	return execute<void, bool>(&FilamentRenderingServerBackend::set_debug_generate_wireframes, p_generate);
};

void FilamentRenderingServer::call_set_vsync_mode(DisplayServer::VSyncMode p_mode, DisplayServer::WindowID p_window)  {
	return execute<void, DisplayServer::VSyncMode, DisplayServer::WindowID>(&FilamentRenderingServerBackend::call_set_vsync_mode, p_mode, p_window);
};

bool FilamentRenderingServer::is_low_end() const {
	return execute<bool>(&FilamentRenderingServerBackend::is_low_end);
};

void FilamentRenderingServer::set_print_gpu_profile(bool p_enable)  {
	return execute<void, bool>(&FilamentRenderingServerBackend::set_print_gpu_profile, p_enable);
};

Size2i FilamentRenderingServer::get_maximum_viewport_size() const {
	return execute<Size2i>(&FilamentRenderingServerBackend::get_maximum_viewport_size);
};

void FilamentRenderingServer::call_on_render_thread(const Callable & p_callable)  {
	return execute<void, const Callable &>(&FilamentRenderingServerBackend::call_on_render_thread, p_callable);
};

Error FilamentRenderingServer::display_server_initialize() {
	return execute<Error>(&FilamentRenderingServerBackend::display_server_initialize);
}

Error FilamentRenderingServer::window_create(DisplayServer::WindowID p_window_id, void *p_native_window) {
	return execute<Error, DisplayServer::WindowID, void *>(&FilamentRenderingServerBackend::window_create, p_window_id, p_native_window);
}

void FilamentRenderingServer::window_destroy(DisplayServer::WindowID p_window_id) {
	return execute<void, DisplayServer::WindowID>(&FilamentRenderingServerBackend::window_destroy, p_window_id);
}
