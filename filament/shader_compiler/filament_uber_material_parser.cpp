#include "filament_uber_material_parser.h"
#include "filament_material_parser.h"

#include <uberz/WritableArchive.h>

FilamentUberMaterialParser::FilamentUberMaterialParser() = default;

FilamentUberMaterialParser::~FilamentUberMaterialParser() = default;

utils::FixedCapacityVector<uint8_t> FilamentUberMaterialParser::compileUberMaterialFile(const std::filesystem::path &filename, utils::JobSystem &jobs) {
	/*
	 * First pass: preprocess the source file and extract all 'pragma uber' with
	 * the shader variants.
	 */

	glshader::process::state state;
	state.add_include_dir(filename.parent_path());

	FilamentUberMaterialParser parser;

	glshader::process::preprocess_file_info file;
	file.file_path = filename;
	file.pragma_processor = &parser;

	auto processed = state.preprocess_file(file);

	if(parser.m_variants.empty()) {
		processed.syntax_error(filename, 1, "there's nothing to compile: at least one ubershader variant must be defined with #pragma uber");
	}

	if(!processed.valid()) {
		fprintf(stderr, "The material has failed the ubershader preprocessing stage:\n");
		for(const auto &error : processed.errors) {
			fprintf(stderr, "%s\n", error.c_str());
		}

		return utils::FixedCapacityVector<uint8_t>();
	}

	printf("Initial preprocessing is done!\n");

	filament::uberz::WritableArchive archive(parser.m_variants.size());
	for(const auto &variant: parser.m_variants) {
		printf("currently compiling %s::%s\n", filename.string().c_str(), variant.name().c_str());
		auto package = FilamentMaterialParser::compileMaterialFile(
			filename, jobs, &variant);

		if(!package.isValid()) {
			fprintf(stderr, "%s: the '%s' ubershader variant has failed to compile\n",
					filename.string().c_str(), variant.name().c_str());
			return utils::FixedCapacityVector<uint8_t>();
		}

		archive.addMaterial(variant.name().c_str(), package.getData(), package.getSize());
	}

	return archive.serialize();
}

bool FilamentUberMaterialParser::process_pragma(glshader::process::pragma_evaluation_context &&context) {
	if(context.is_at_end() || context.get() != "uber") {
		return false;
	}

	if(context.is_at_end()) {
		context.syntax_error("ubershader variant name is expected");
		return true;
	}

	const auto &name = context.get();

	std::vector<glshader::process::definition> definitions;

	while(!context.is_at_end()) {
		const auto &def = context.get();

		auto delim = def.find('=');
		if(delim == std::string::npos) {
			definitions.emplace_back(def);
		} else {
			auto name = def.substr(0, delim);
			auto value = context.expand(def.substr(delim + 1));

			definitions.emplace_back(std::move(name), std::move(value));
		}
	}

	m_variants.emplace_back(name, std::move(definitions));

	return false;

}
