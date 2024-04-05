#ifndef FILAMENT_RESOURCES_FILAMENT_UBER_MATERIAL_H
#define FILAMENT_RESOURCES_FILAMENT_UBER_MATERIAL_H

#include "core/io/resource.h"
#include "core/io/resource_loader.h"
#include "core/io/resource_saver.h"

#include "filament_material.h"

class FilamentUberMaterial final : public Resource {

	GDCLASS(FilamentUberMaterial, Resource)

private:
	Vector<uint8_t> package;

protected:
	static void _bind_methods();

public:
	FilamentUberMaterial();
	~FilamentUberMaterial();

	const Vector<uint8_t> &get_package() const;
	bool set_package(const Vector<uint8_t> &package);

	TypedArray<FilamentMaterial> get_materials() const;
	void set_materials(const TypedArray<FilamentMaterial> &materials);

private:
	bool deserialize(const Vector<uint8_t> &package);

	TypedArray<FilamentMaterial> materials;
};

class ResourceFormatLoaderFilamentUberMaterial : public ResourceFormatLoader {
public:
	virtual Ref<Resource> load(const String &p_path, const String &p_original_path = "", Error *r_error = nullptr, bool p_use_sub_threads = false, float *r_progress = nullptr, CacheMode p_cache_mode = CACHE_MODE_REUSE);
	virtual void get_recognized_extensions(List<String> *p_extensions) const;
	virtual bool handles_type(const String &p_type) const;
	virtual String get_resource_type(const String &p_path) const;
};

class ResourceFormatSaverFilamentUberMaterial : public ResourceFormatSaver {
public:
	virtual Error save(const Ref<Resource> &p_resource, const String &p_path, uint32_t p_flags = 0);
	virtual void get_recognized_extensions(const Ref<Resource> &p_resource, List<String> *p_extensions) const;
	virtual bool recognize(const Ref<Resource> &p_resource) const;
};


#endif

