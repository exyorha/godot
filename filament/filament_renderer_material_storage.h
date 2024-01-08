#ifndef FILAMENT_RENDERER_MATERIAL_STORAGE_H
#define FILAMENT_RENDERER_MATERIAL_STORAGE_H

#include "servers/rendering/storage/material_storage.h"

class FilamentRendererMaterialStorage final : public RendererMaterialStorage {
public:
	FilamentRendererMaterialStorage();
	~FilamentRendererMaterialStorage() override;

	void global_shader_parameter_add(const StringName &p_name, RS::GlobalShaderParameterType p_type, const Variant &p_value) override;
	void global_shader_parameter_remove(const StringName &p_name) override;
	Vector<StringName> global_shader_parameter_get_list() const  override;

	void global_shader_parameter_set(const StringName &p_name, const Variant &p_value) override;
	void global_shader_parameter_set_override(const StringName &p_name, const Variant &p_value) override;
	Variant global_shader_parameter_get(const StringName &p_name) const  override;
	RS::GlobalShaderParameterType global_shader_parameter_get_type(const StringName &p_name) const  override;

	void global_shader_parameters_load_settings(bool p_load_texture) override;
	void global_shader_parameters_clear() override;

	int32_t global_shader_parameters_instance_allocate(RID p_instance) override;
	void global_shader_parameters_instance_free(RID p_instance) override;
	void global_shader_parameters_instance_update(RID p_instance, int p_index, const Variant &p_value, int p_flags_count) override;

	/* SHADER API */
	RID shader_allocate() override;
	void shader_initialize(RID p_rid) override;
	void shader_free(RID p_rid) override;

	void shader_set_code(RID p_shader, const String &p_code) override;
	void shader_set_path_hint(RID p_shader, const String &p_path) override;
	String shader_get_code(RID p_shader) const  override;
	void get_shader_parameter_list(RID p_shader, List<PropertyInfo> *p_param_list) const  override;

	void shader_set_default_texture_parameter(RID p_shader, const StringName &p_name, RID p_texture, int p_index) override;
	RID shader_get_default_texture_parameter(RID p_shader, const StringName &p_name, int p_index) const  override;
	Variant shader_get_parameter_default(RID p_material, const StringName &p_param) const  override;

	RS::ShaderNativeSourceCode shader_get_native_source_code(RID p_shader) const  override;

	/* MATERIAL API */

	RID material_allocate() override;
	void material_initialize(RID p_rid) override;
	void material_free(RID p_rid) override;

	void material_set_render_priority(RID p_material, int priority) override;
	void material_set_shader(RID p_shader_material, RID p_shader) override;

	void material_set_param(RID p_material, const StringName &p_param, const Variant &p_value) override;
	Variant material_get_param(RID p_material, const StringName &p_param) const  override;

	void material_set_next_pass(RID p_material, RID p_next_material) override;

	bool material_is_animated(RID p_material) override;
	bool material_casts_shadows(RID p_material) override;

	struct InstanceShaderParam {
		PropertyInfo info;
		int index;
		Variant default_value;
	};

	void material_get_instance_shader_parameters(RID p_material, List<RendererMaterialStorage::InstanceShaderParam> *r_parameters) override;

	void material_update_dependency(RID p_material, DependencyTracker *p_instance) override;
};

#endif
