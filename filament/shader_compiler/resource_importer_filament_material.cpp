/**************************************************************************/
/*  resource_importer_filament_material.cpp                                     */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "resource_importer_filament_material.h"

#include "filament/shader_compiler/filament_material_parser.h"
#include "filament/filament_rendering_server.h"

#include <filament/Engine.h>
#include <utils/JobSystem.h>

#include <filesystem>

#include "core/config/project_settings.h"
#include "core/error/error_list.h"
#include "core/io/file_access.h"
#include "editor/editor_node.h"

String ResourceImporterFilamentMaterial::get_importer_name() const {
	return "filamat";
}

String ResourceImporterFilamentMaterial::get_visible_name() const {
	return "Filament Material";
}

void ResourceImporterFilamentMaterial::get_recognized_extensions(List<String> *p_extensions) const {
	p_extensions->push_back("mat");
}

String ResourceImporterFilamentMaterial::get_save_extension() const {
	return "fmat";
}

String ResourceImporterFilamentMaterial::get_resource_type() const {
	return "FilamentMaterial";
}

int ResourceImporterFilamentMaterial::get_preset_count() const {
	return 0;
}

String ResourceImporterFilamentMaterial::get_preset_name(int p_idx) const {
	return String();
}

void ResourceImporterFilamentMaterial::get_import_options(const String &p_path, List<ImportOption> *r_options, int p_preset) const {
}

bool ResourceImporterFilamentMaterial::get_option_visibility(const String &p_path, const String &p_option, const HashMap<StringName, Variant> &p_options) const {
	return true;
}


Error ResourceImporterFilamentMaterial::import(const String &p_source_file, const String &p_save_path, const HashMap<StringName, Variant> &p_options, List<String> *r_platform_variants, List<String> *r_gen_files, Variant *r_metadata) {


	filamat::Package package;

	try {
		auto sourceFileInFS = ProjectSettings::get_singleton()->globalize_path(p_source_file);

		std::filesystem::path path;
	#if defined(_WIN32)
		path = sourceFileInFS.utf16().ptr();
	#else
		path = sourceFileInFS.utf8().ptr();
	#endif

		struct LocalJobSystem {
			LocalJobSystem() {
				js.adopt();
			}

			~LocalJobSystem() {
				js.emancipate();
			}

			utils::JobSystem js;
		} jobSystem;

		package = FilamentMaterialParser::compileMaterialFile(path, jobSystem.js);

	} catch(const std::exception &e) {
		callable_mp_static(&EditorNode::add_io_error)
			.bind(vformat(TTR("An attempt to import a Filament material '%s' has failed with an exception: %s."), p_source_file, e.what())).call_deferred();

		return ERR_COMPILATION_FAILED;
	}

	if(!package.isValid()) {
		callable_mp_static(&EditorNode::add_io_error)
			.bind(vformat(TTR("The Filament material '%s' has failed to compile, check the log."), p_source_file)).call_deferred();

		return ERR_COMPILATION_FAILED;
	}

	Error err;
	Ref<FileAccess> file = FileAccess::open(p_save_path + ".fmat", FileAccess::WRITE, &err);
	ERR_FAIL_COND_V(err != OK, ERR_CANT_OPEN);
	ERR_FAIL_COND_V(!file.is_valid(), ERR_CANT_OPEN);

	file->store_buffer(package.getData(), package.getSize());
	file->close();

	return OK;
}

ResourceImporterFilamentMaterial::ResourceImporterFilamentMaterial() {
}
