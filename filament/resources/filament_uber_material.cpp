#include "filament/resources/filament_uber_material.h"
#include "core/object/object.h"
#include "filament/resources/filament_material.h"
#include "core/error/error_macros.h"

#include <uberz/ReadableArchive.h>

#include <zstd.h>

FilamentUberMaterial::FilamentUberMaterial() = default;

FilamentUberMaterial::~FilamentUberMaterial() = default;

const Vector<uint8_t> &FilamentUberMaterial::get_package() const {
	return package;
}

bool FilamentUberMaterial::deserialize(const Vector<uint8_t> &p_package) {

	Vector<uint8_t> uncompressedData;

	if(p_package.size() >= 1 && p_package.get(0) == 0x28) {
		/*
		 * This is Zstandard-compressed data.
		 */

		auto decompressedSize = ZSTD_getFrameContentSize(p_package.ptr(), p_package.size());

		ERR_FAIL_COND_V_MSG(
			(decompressedSize == ZSTD_CONTENTSIZE_UNKNOWN || decompressedSize == ZSTD_CONTENTSIZE_ERROR),
			false, "bad Zstandard-compressed data");

		uncompressedData.resize(decompressedSize);

		auto result = ZSTD_decompress(uncompressedData.ptrw(), uncompressedData.size(), p_package.ptr(), p_package.size());

		ERR_FAIL_COND_V_MSG(result != decompressedSize, false, "bad Zstandard-compressed data");
	} else {
		/*
		 * Assume that this is uncompressed data.
		 */

		uncompressedData = p_package;
	}

	if(!uncompressedData.is_empty()) {
		auto archive = reinterpret_cast<filament::uberz::ReadableArchive *>(uncompressedData.ptrw());

		/*
		 * Filament does not define this anywhere but the WritableArchive
		 * implementation, unfortunately.
		 */

		ERR_FAIL_COND_V_MSG(archive->magic != 'UBER' || archive->version != 0, false, "Bad ubershader archive");

		filament::uberz::convertOffsetsToPointers(archive);

		for(uint64_t specIndex = 0, specCount = archive->specsCount; specIndex < specCount; specIndex++) {
			const auto &spec = archive->specs[specIndex];

			Vector<uint8_t> materialPackage;
			materialPackage.resize(spec.packageByteCount);
			memcpy(materialPackage.ptrw(), spec.package, spec.packageByteCount);


			Ref<FilamentMaterial> material;
			material.instantiate();
			material->set_package(materialPackage);

			materials.append(material);
		}
	}

	return true;
}

bool FilamentUberMaterial::set_package(const Vector<uint8_t> &p_package) {
	materials.clear();

	bool result = deserialize(p_package);

	if(result) {
		package = p_package;
	} else {
		package.clear();
	}

	emit_changed();

	return result;
}

TypedArray<FilamentMaterial> FilamentUberMaterial::get_materials() const {
	return materials;
}

void FilamentUberMaterial::set_materials(const TypedArray<FilamentMaterial> &p_materials) {
	materials = p_materials;
}

void FilamentUberMaterial::_bind_methods() {

	ClassDB::bind_method(D_METHOD("set_package", "package"), &FilamentUberMaterial::set_package);
	ClassDB::bind_method(D_METHOD("get_package"), &FilamentUberMaterial::get_package);

	ADD_PROPERTY(PropertyInfo(Variant::PACKED_BYTE_ARRAY, "package", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NO_EDITOR), "set_package", "get_package");

	ClassDB::bind_method(D_METHOD("set_materials", "materials"), &FilamentUberMaterial::set_materials);
	ClassDB::bind_method(D_METHOD("get_materials"), &FilamentUberMaterial::get_materials);

	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "materials", PROPERTY_HINT_ARRAY_TYPE, MAKE_RESOURCE_TYPE_HINT("FilamentMaterial"), PROPERTY_USAGE_EDITOR),
				 "set_materials", "get_materials");
}

Ref<Resource> ResourceFormatLoaderFilamentUberMaterial::load(const String &p_path, const String &p_original_path, Error *r_error, bool p_use_sub_threads,
															 float *r_progress, CacheMode p_cache_mode) {
	if (r_error) {
		*r_error = ERR_FILE_CANT_OPEN;
	}

	Error error = OK;
	Vector<uint8_t> buffer = FileAccess::get_file_as_bytes(p_path, &error);
	ERR_FAIL_COND_V_MSG(error, nullptr, "Cannot load shader: " + p_path);

	Ref<FilamentUberMaterial> shader;
	shader.instantiate();

	shader->set_package(buffer);

	if (r_error) {
		*r_error = OK;
	}

	return shader;
}

void ResourceFormatLoaderFilamentUberMaterial::get_recognized_extensions(List<String> *p_extensions) const {
	p_extensions->push_back("fumat");
}

bool ResourceFormatLoaderFilamentUberMaterial::handles_type(const String &p_type) const {
	return ClassDB::is_parent_class("FilamentUberMaterial", p_type);
}

String ResourceFormatLoaderFilamentUberMaterial::get_resource_type(const String &p_path) const {
	String el = p_path.get_extension().to_lower();
	if (el == "fumat") {
		return "FilamentUberMaterial";
	}
	return "";
}

Error ResourceFormatSaverFilamentUberMaterial::save(const Ref<Resource> &p_resource, const String &p_path, uint32_t p_flags) {
	Ref<FilamentUberMaterial> shader = p_resource;
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

void ResourceFormatSaverFilamentUberMaterial::get_recognized_extensions(const Ref<Resource> &p_resource, List<String> *p_extensions) const {
	if (const FilamentUberMaterial *shader = Object::cast_to<FilamentUberMaterial>(*p_resource)) {
		p_extensions->push_back("fumat");
	}
}

bool ResourceFormatSaverFilamentUberMaterial::recognize(const Ref<Resource> &p_resource) const {
	return ClassDB::is_parent_class("FilamentUberMaterial", p_resource->get_class_name()); //only filament material and parent classes, not inherited
}
