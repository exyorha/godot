#ifndef FILAMENT_RENDERER_FOG_H
#define FILAMENT_RENDERER_FOG_H

#include "servers/rendering/environment/renderer_fog.h"

class FilamentRendererFog final : public RendererFog {
public:
	FilamentRendererFog();
	~FilamentRendererFog() override;

	RID fog_volume_allocate() override;
	void fog_volume_initialize(RID p_rid) override;
	void fog_volume_free(RID p_rid) override;

	void fog_volume_set_shape(RID p_fog_volume, RS::FogVolumeShape p_shape) override;
	void fog_volume_set_size(RID p_fog_volume, const Vector3 &p_size) override;
	void fog_volume_set_material(RID p_fog_volume, RID p_material) override;
	AABB fog_volume_get_aabb(RID p_fog_volume) const override;
	RS::FogVolumeShape fog_volume_get_shape(RID p_fog_volume) const override;
};

#endif
