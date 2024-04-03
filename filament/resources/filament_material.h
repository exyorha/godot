#ifndef FILAMENT_RESOURCES_FILAMENT_MATERIAL_H
#define FILAMENT_RESOURCES_FILAMENT_MATERIAL_H

#include "scene/resources/shader.h"

class FilamentMaterial final : public BaseShader {

	GDCLASS(FilamentMaterial, BaseShader)

private:
	Vector<uint8_t> package;

protected:
	static void _bind_methods();

public:
	FilamentMaterial();
	~FilamentMaterial();

	const Vector<uint8_t> &get_package() const;
	void set_package(const Vector<uint8_t> &package);
};

class ResourceFormatLoaderFilamentMaterial : public ResourceFormatLoader {
public:
	virtual Ref<Resource> load(const String &p_path, const String &p_original_path = "", Error *r_error = nullptr, bool p_use_sub_threads = false, float *r_progress = nullptr, CacheMode p_cache_mode = CACHE_MODE_REUSE);
	virtual void get_recognized_extensions(List<String> *p_extensions) const;
	virtual bool handles_type(const String &p_type) const;
	virtual String get_resource_type(const String &p_path) const;
};

class ResourceFormatSaverFilamentMaterial : public ResourceFormatSaver {
public:
	virtual Error save(const Ref<Resource> &p_resource, const String &p_path, uint32_t p_flags = 0);
	virtual void get_recognized_extensions(const Ref<Resource> &p_resource, List<String> *p_extensions) const;
	virtual bool recognize(const Ref<Resource> &p_resource) const;
};


#endif
