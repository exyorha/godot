#include "filament/register_filament_types.h"
#include "filament/filament_rendering_server.h"
#include "filament/resources/filament_material.h"
#include "filament/resources/filament_uber_material.h"

#if defined(TOOLS_ENABLED)
#include "filament/shader_compiler/resource_importer_filament_material.h"
#include "filament/shader_compiler/resource_importer_filament_uber_material.h"

#include <filamat/MaterialBuilder.h>
#endif

static Ref<ResourceFormatLoaderFilamentMaterial> resource_loader_filament_material;

static Ref<ResourceFormatSaverFilamentMaterial> resource_saver_filament_material;

static Ref<ResourceFormatLoaderFilamentUberMaterial> resource_loader_filament_uber_material;

static Ref<ResourceFormatSaverFilamentUberMaterial> resource_saver_filament_uber_material;

static Ref<ResourceImporterFilamentMaterial> resource_importer_filament_material;

static Ref<ResourceImporterFilamentUberMaterial> resource_importer_filament_uber_material;

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

	GDREGISTER_CLASS(FilamentUberMaterial);
	resource_loader_filament_uber_material.instantiate();
	ResourceLoader::add_resource_format_loader(resource_loader_filament_uber_material, true);
	resource_saver_filament_uber_material.instantiate();
	ResourceSaver::add_resource_format_saver(resource_saver_filament_uber_material, true);

#if defined(TOOLS_ENABLED)
	filamat::MaterialBuilder::init();

	GDREGISTER_CLASS(ResourceImporterFilamentMaterial);
	GDREGISTER_CLASS(ResourceImporterFilamentUberMaterial);

	resource_importer_filament_material.instantiate();
	ResourceFormatImporter::get_singleton()->add_importer(resource_importer_filament_material);

	resource_importer_filament_uber_material.instantiate();
	ResourceFormatImporter::get_singleton()->add_importer(resource_importer_filament_uber_material);
#endif
}

void unregister_filament_types() {
#if defined(TOOLS_ENABLED)

	ResourceFormatImporter::get_singleton()->remove_importer(resource_importer_filament_uber_material);
	resource_importer_filament_uber_material.unref();


	ResourceFormatImporter::get_singleton()->remove_importer(resource_importer_filament_material);
	resource_importer_filament_material.unref();

	filamat::MaterialBuilder::shutdown();
#endif

	ResourceSaver::remove_resource_format_saver(resource_saver_filament_uber_material);
	resource_saver_filament_uber_material.unref();
	ResourceLoader::remove_resource_format_loader(resource_loader_filament_uber_material);
	resource_loader_filament_uber_material.unref();

	ResourceSaver::remove_resource_format_saver(resource_saver_filament_material);
	resource_saver_filament_material.unref();
	ResourceLoader::remove_resource_format_loader(resource_loader_filament_material);
	resource_loader_filament_material.unref();
}
