#ifndef FILAMENT_MATERIAL_PARSER_H
#define FILAMENT_MATERIAL_PARSER_H

#include "filamat_extra_enums.h"

#include <glsp/preprocess.hpp>

#include <filamat/MaterialBuilder.h>

#include <optional>

class FilamentMaterialParser final : private glshader::process::pragma_processor_interface {
public:

	static filamat::Package compileMaterialFile(const std::filesystem::path &filename, utils::JobSystem &jobs);

private:
	FilamentMaterialParser();
	~FilamentMaterialParser();

	bool process_pragma(glshader::process::pragma_evaluation_context &&context) override;

	filamat::Package finalize(std::string &&output, utils::JobSystem &jobSystem);

	enum class OutputStream {
		Fragment,
		Vertex
	};

	template<typename T>
	void parseSingleEnumParameter(glshader::process::pragma_evaluation_context &context,
								  filamat::MaterialBuilder & (filamat::MaterialBuilder::*setter)(T value));

	void parseBooleanParameter(glshader::process::pragma_evaluation_context &context,
							   filamat::MaterialBuilder & (filamat::MaterialBuilder::*setter)(bool value));

	void parseFloatParameter(glshader::process::pragma_evaluation_context &context,
							   filamat::MaterialBuilder & (filamat::MaterialBuilder::*setter)(float value));

	template<typename T>
	static bool convertNumber(glshader::process::pragma_evaluation_context &context,
							  T &value);

	template<typename T>
	static bool convertNumber(glshader::process::pragma_evaluation_context &context,
							  const std::string_view &string,
							  T &value);

	template<>
	bool convertNumber<float>(glshader::process::pragma_evaluation_context &context,
							  float &value);

	void switchOutputStream(glshader::process::pragma_evaluation_context &context, OutputStream stream);

	OutputStream m_outputStream;
	filamat::MaterialBuilder m_builder;
	std::optional<std::string> m_vertexStage;
	std::optional<std::string> m_fragmentStage;
};

#endif
