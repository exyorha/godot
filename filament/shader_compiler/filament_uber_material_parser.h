#ifndef FILAMENT_UBER_MATERIAL_PARSER_H
#define FILAMENT_UBER_MATERIAL_PARSER_H

#include <filesystem>
#include <vector>

#include <glsp/preprocess.hpp>

#include <utils/FixedCapacityVector.h>

#include "filament_ubershader_variant.h"

namespace utils {
	class JobSystem;
}

class FilamentUberMaterialParser final : public glshader::process::pragma_processor_interface {
public:
	static utils::FixedCapacityVector<uint8_t> compileUberMaterialFile(const std::filesystem::path &filename, utils::JobSystem &jobs);

private:
	FilamentUberMaterialParser();
	~FilamentUberMaterialParser();

	bool process_pragma(glshader::process::pragma_evaluation_context &&context) override;

	std::vector<FilamentUbershaderVariant> m_variants;
};

#endif
