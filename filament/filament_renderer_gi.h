#ifndef FILAMENT_RENDERER_GI_H
#define FILAMENT_RENDERER_GI_H

#include "servers/rendering/environment/renderer_gi.h"

class FilamentRendererGI final : public RendererGI {
public:
	FilamentRendererGI();
	~FilamentRendererGI() override;

	/* VOXEL GI API */

	RID voxel_gi_allocate() override;
	void voxel_gi_free(RID p_rid) override;
	void voxel_gi_initialize(RID p_rid) override;

	void voxel_gi_allocate_data(RID p_voxel_gi, const Transform3D &p_to_cell_xform, const AABB &p_aabb, const Vector3i &p_octree_size, const Vector<uint8_t> &p_octree_cells, const Vector<uint8_t> &p_data_cells, const Vector<uint8_t> &p_distance_field, const Vector<int> &p_level_counts) override;

	AABB voxel_gi_get_bounds(RID p_voxel_gi) const override;
	Vector3i voxel_gi_get_octree_size(RID p_voxel_gi) const override;
	Vector<uint8_t> voxel_gi_get_octree_cells(RID p_voxel_gi) const override;
	Vector<uint8_t> voxel_gi_get_data_cells(RID p_voxel_gi) const override;
	Vector<uint8_t> voxel_gi_get_distance_field(RID p_voxel_gi) const override;

	Vector<int> voxel_gi_get_level_counts(RID p_voxel_gi) const override;
	Transform3D voxel_gi_get_to_cell_xform(RID p_voxel_gi) const override;

	void voxel_gi_set_dynamic_range(RID p_voxel_gi, float p_range) override;
	float voxel_gi_get_dynamic_range(RID p_voxel_gi) const override;

	void voxel_gi_set_propagation(RID p_voxel_gi, float p_range) override;
	float voxel_gi_get_propagation(RID p_voxel_gi) const override;

	void voxel_gi_set_energy(RID p_voxel_gi, float p_energy) override;
	float voxel_gi_get_energy(RID p_voxel_gi) const override;

	void voxel_gi_set_baked_exposure_normalization(RID p_voxel_gi, float p_baked_exposure) override;
	float voxel_gi_get_baked_exposure_normalization(RID p_voxel_gi) const override;

	void voxel_gi_set_bias(RID p_voxel_gi, float p_bias) override;
	float voxel_gi_get_bias(RID p_voxel_gi) const override;

	void voxel_gi_set_normal_bias(RID p_voxel_gi, float p_range) override;
	float voxel_gi_get_normal_bias(RID p_voxel_gi) const override;

	void voxel_gi_set_interior(RID p_voxel_gi, bool p_enable) override;
	bool voxel_gi_is_interior(RID p_voxel_gi) const override;

	void voxel_gi_set_use_two_bounces(RID p_voxel_gi, bool p_enable) override;
	bool voxel_gi_is_using_two_bounces(RID p_voxel_gi) const override;

	uint32_t voxel_gi_get_version(RID p_probe) const override;

	void sdfgi_reset() override;
};

#endif

