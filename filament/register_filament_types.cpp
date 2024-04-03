#include "filament/register_filament_types.h"
#include "filament/filament_rendering_server.h"
#include "filament/resources/filament_material.h"

static Ref<ResourceFormatLoaderFilamentMaterial> resource_loader_filament_material;

static Ref<ResourceFormatSaverFilamentMaterial> resource_saver_filament_material;

void register_filament_types() {
	/*
	 * Abstract, because we don't want anyone to be creating the instances of it.
	 */
	GDREGISTER_ABSTRACT_CLASS(FilamentRenderingServer);

	GDREGISTER_CLASS(FilamentMaterial);
	resource_loader_filament_material.instantiate();
	ResourceLoader::add_resource_format_loader(resource_loader_filament_material, true);
	resource_saver_filament_material.instantiate();
	ResourceSaver::add_resource_format_saver(resource_saver_filament_material, true);

}

void unregister_filament_types() {
	ResourceSaver::remove_resource_format_saver(resource_saver_filament_material);
	resource_saver_filament_material.unref();
	ResourceLoader::remove_resource_format_loader(resource_loader_filament_material);
	resource_loader_filament_material.unref();
}
