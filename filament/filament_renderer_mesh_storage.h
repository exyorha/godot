#ifndef FILAMENT_RENDERER_MESH_STORAGE_H
#define FILAMENT_RENDERER_MESH_STORAGE_H

#include "servers/rendering/storage/mesh_storage.h"

class FilamentRendererMeshStorage final : public RendererMeshStorage {
public:
	FilamentRendererMeshStorage();
	~FilamentRendererMeshStorage();

	RID mesh_allocate() override;
	void mesh_initialize(RID p_rid) override;
	void mesh_free(RID p_rid) override;

	void mesh_set_blend_shape_count(RID p_mesh, int p_blend_shape_count) override;

	/// Returns stride
	void mesh_add_surface(RID p_mesh, const RS::SurfaceData &p_surface) override;

	int mesh_get_blend_shape_count(RID p_mesh) const override;

	void mesh_set_blend_shape_mode(RID p_mesh, RS::BlendShapeMode p_mode) override;
	RS::BlendShapeMode mesh_get_blend_shape_mode(RID p_mesh) const override;

	void mesh_surface_update_vertex_region(RID p_mesh, int p_surface, int p_offset, const Vector<uint8_t> &p_data) override;
	void mesh_surface_update_attribute_region(RID p_mesh, int p_surface, int p_offset, const Vector<uint8_t> &p_data) override;
	void mesh_surface_update_skin_region(RID p_mesh, int p_surface, int p_offset, const Vector<uint8_t> &p_data) override;

	void mesh_surface_set_material(RID p_mesh, int p_surface, RID p_material) override;
	RID mesh_surface_get_material(RID p_mesh, int p_surface) const override;

	RS::SurfaceData mesh_get_surface(RID p_mesh, int p_surface) const override;

	int mesh_get_surface_count(RID p_mesh) const override;

	void mesh_set_custom_aabb(RID p_mesh, const AABB &p_aabb) override;
	AABB mesh_get_custom_aabb(RID p_mesh) const override;

	AABB mesh_get_aabb(RID p_mesh, RID p_skeleton) override;

	void mesh_set_shadow_mesh(RID p_mesh, RID p_shadow_mesh) override;

	void mesh_clear(RID p_mesh) override;

	bool mesh_needs_instance(RID p_mesh, bool p_has_skeleton) override;

	RID mesh_instance_create(RID p_base) override;
	void mesh_instance_free(RID p_rid) override;
	void mesh_instance_set_skeleton(RID p_mesh_instance, RID p_skeleton) override;
	void mesh_instance_set_blend_shape_weight(RID p_mesh_instance, int p_shape, float p_weight) override;
	void mesh_instance_check_for_update(RID p_mesh_instance) override;
	void mesh_instance_set_canvas_item_transform(RID p_mesh_instance, const Transform2D &p_transform) override;
	void update_mesh_instances() override;

	RID multimesh_allocate() override;
	void multimesh_initialize(RID p_rid) override;
	void multimesh_free(RID p_rid) override;

	void multimesh_allocate_data(RID p_multimesh, int p_instances, RS::MultimeshTransformFormat p_transform_format, bool p_use_colors, bool p_use_custom_data) override;

	int multimesh_get_instance_count(RID p_multimesh) const override;

	void multimesh_set_mesh(RID p_multimesh, RID p_mesh) override;
	void multimesh_instance_set_transform(RID p_multimesh, int p_index, const Transform3D &p_transform) override;
	void multimesh_instance_set_transform_2d(RID p_multimesh, int p_index, const Transform2D &p_transform) override;
	void multimesh_instance_set_color(RID p_multimesh, int p_index, const Color &p_color) override;
	void multimesh_instance_set_custom_data(RID p_multimesh, int p_index, const Color &p_color) override;

	RID multimesh_get_mesh(RID p_multimesh) const override;

	Transform3D multimesh_instance_get_transform(RID p_multimesh, int p_index) const override;
	Transform2D multimesh_instance_get_transform_2d(RID p_multimesh, int p_index) const override;
	Color multimesh_instance_get_color(RID p_multimesh, int p_index) const override;
	Color multimesh_instance_get_custom_data(RID p_multimesh, int p_index) const override;

	void multimesh_set_buffer(RID p_multimesh, const Vector<float> &p_buffer) override;
	Vector<float> multimesh_get_buffer(RID p_multimesh) const override;

	void multimesh_set_visible_instances(RID p_multimesh, int p_visible) override;
	int multimesh_get_visible_instances(RID p_multimesh) const override;

	AABB multimesh_get_aabb(RID p_multimesh) const override;

	RID skeleton_allocate() override;
	void skeleton_initialize(RID p_rid) override;
	void skeleton_free(RID p_rid) override;

	void skeleton_allocate_data(RID p_skeleton, int p_bones, bool p_2d_skeleton) override;
	int skeleton_get_bone_count(RID p_skeleton) const override;
	void skeleton_bone_set_transform(RID p_skeleton, int p_bone, const Transform3D &p_transform) override;
	Transform3D skeleton_bone_get_transform(RID p_skeleton, int p_bone) const override;
	void skeleton_bone_set_transform_2d(RID p_skeleton, int p_bone, const Transform2D &p_transform) override;
	Transform2D skeleton_bone_get_transform_2d(RID p_skeleton, int p_bone) const override;
	void skeleton_set_base_transform_2d(RID p_skeleton, const Transform2D &p_base_transform) override;

	void skeleton_update_dependency(RID p_base, DependencyTracker *p_instance) override;
};

#endif
