#include "filament/resources/filament_material.h"
#include "filament/filament_rendering_server.h"

FilamentMaterial::FilamentMaterial() = default;

FilamentMaterial::~FilamentMaterial() = default;

const Vector<uint8_t> &FilamentMaterial::get_package() const {
	return package;
}

void FilamentMaterial::set_package(const Vector<uint8_t> &p_package) {
	package = p_package;

	auto server = cast_to<FilamentRenderingServer>(RenderingServer::get_singleton());
	ERR_FAIL_COND_MSG(!server, "FilamentMaterial is only available when using FilamentRenderingServer");

	server->shader_set_package(shader_rid(), p_package);

	emit_changed();
}

void FilamentMaterial::_bind_methods() {

	ClassDB::bind_method(D_METHOD("set_package", "package"), &FilamentMaterial::set_package);
	ClassDB::bind_method(D_METHOD("get_package"), &FilamentMaterial::get_package);

	ADD_PROPERTY(PropertyInfo(Variant::PACKED_BYTE_ARRAY, "package", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NO_EDITOR), "set_package", "get_package");
}

Ref<Resource> ResourceFormatLoaderFilamentMaterial::load(const String &p_path, const String &p_original_path, Error *r_error, bool p_use_sub_threads, float *r_progress, CacheMode p_cache_mode) {
	if (r_error) {
		*r_error = ERR_FILE_CANT_OPEN;
	}

	Error error = OK;
	Vector<uint8_t> buffer = FileAccess::get_file_as_bytes(p_path, &error);
	ERR_FAIL_COND_V_MSG(error, nullptr, "Cannot load shader: " + p_path);

	Ref<FilamentMaterial> shader;
	shader.instantiate();

	shader->set_package(buffer);

	if (r_error) {
		*r_error = OK;
	}

	return shader;
}

void ResourceFormatLoaderFilamentMaterial::get_recognized_extensions(List<String> *p_extensions) const {
	p_extensions->push_back("fmat");
}

bool ResourceFormatLoaderFilamentMaterial::handles_type(const String &p_type) const {
	return ClassDB::is_parent_class("FilamentMaterial", p_type);
}

String ResourceFormatLoaderFilamentMaterial::get_resource_type(const String &p_path) const {
	String el = p_path.get_extension().to_lower();
	if (el == "fmat") {
		return "FilamentMaterial";
	}
	return "";
}

Error ResourceFormatSaverFilamentMaterial::save(const Ref<Resource> &p_resource, const String &p_path, uint32_t p_flags) {
	Ref<FilamentMaterial> shader = p_resource;
	ERR_FAIL_COND_V(shader.is_null(), ERR_INVALID_PARAMETER);

	Vector<uint8_t> buffer = shader->get_package();

	Error err;
	Ref<FileAccess> file = FileAccess::open(p_path, FileAccess::WRITE, &err);

	ERR_FAIL_COND_V_MSG(err, err, "Cannot save shader '" + p_path + "'.");

	file->store_buffer(buffer);
	if (file->get_error() != OK && file->get_error() != ERR_FILE_EOF) {
		return ERR_CANT_CREATE;
	}

	return OK;
}

void ResourceFormatSaverFilamentMaterial::get_recognized_extensions(const Ref<Resource> &p_resource, List<String> *p_extensions) const {
	if (const FilamentMaterial *shader = Object::cast_to<FilamentMaterial>(*p_resource)) {
		p_extensions->push_back("fmat");
	}
}

bool ResourceFormatSaverFilamentMaterial::recognize(const Ref<Resource> &p_resource) const {
	return ClassDB::is_parent_class("FilamentMaterial", p_resource->get_class_name()); //only filament material and parent classes, not inherited
}
