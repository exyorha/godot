
#include <charconv>
#include <optional>
#include <variant>
#include <sstream>

#include "filament_material_parser.h"
#include "filament_ubershader_variant.h"

FilamentMaterialParser::FilamentMaterialParser(const FilamentUbershaderVariant *ubershaderVariant) :
	m_ubershaderVariant(ubershaderVariant),
	m_outputStream(OutputStream::Fragment) {

	if(m_ubershaderVariant) {
		m_builder.name(("unnamed_" + m_ubershaderVariant->name()).c_str());
	}
}

FilamentMaterialParser::~FilamentMaterialParser() = default;

filamat::Package FilamentMaterialParser::finalize(std::string &&output, utils::JobSystem &jobSystem) {
	if(m_outputStream == OutputStream::Fragment) {
		m_fragmentStage.emplace(std::move(output));
	} else {
		m_vertexStage.emplace(std::move(output));
	}

	if(m_fragmentStage.has_value()) {
		m_builder.material(m_fragmentStage->c_str());
	}

	if(m_vertexStage.has_value()) {
		m_builder.materialVertex(m_vertexStage->c_str());
	}

	return m_builder.build(jobSystem);
}

template<typename T>
void FilamentMaterialParser::parseSingleEnumParameter(glshader::process::pragma_evaluation_context &context,
													  filamat::MaterialBuilder & (filamat::MaterialBuilder::*setter)(T value)) {


	if(context.is_at_end()) {
		context.syntax_error("value expected");
		return;
	}

	const auto &token = context.get();

	if(!filamat::Enums::isValid<T>(token)) {
		context.syntax_error("unsupported value");
		return;
	}

	(m_builder.*setter)(filamat::Enums::toEnum<T>(token));
}

void FilamentMaterialParser::parseBooleanParameter(glshader::process::pragma_evaluation_context &context,
													  filamat::MaterialBuilder & (filamat::MaterialBuilder::*setter)(bool value)) {


	if(context.is_at_end()) {
		context.syntax_error("value expected");
		return;
	}

	const auto &token = context.get();

	bool value;

	if(token == "enabled") {
		value = true;
	} else if(token == "disabled") {
		value = false;
	} else {
		context.syntax_error("unsupported value");
		return;
	}

	(m_builder.*setter)(value);
}
void FilamentMaterialParser::parseFloatParameter(glshader::process::pragma_evaluation_context &context,
												 filamat::MaterialBuilder & (filamat::MaterialBuilder::*setter)(float value)) {


	float value;

	if(!convertNumber(context, value)) {
		return;
	}

	(m_builder.*setter)(value);
}

template<typename T>
bool FilamentMaterialParser::convertNumber(glshader::process::pragma_evaluation_context &context, T &value) {
	if(context.is_at_end()) {
		context.syntax_error("value expected");
		return false;
	}

	auto token = context.get();

	return convertNumber(context, token, value);
}


template<typename T>
bool FilamentMaterialParser::convertNumber(glshader::process::pragma_evaluation_context &context, const std::string_view &token, T &value) {

	auto result = std::from_chars(token.data(), token.data() + token.size(), value);
	if(result.ec != std::errc() || result.ptr != token.data() + token.size() || token.empty()) {
		context.syntax_error("not a number");
		return false;
	}

	return true;
}

template<>
bool FilamentMaterialParser::convertNumber<float>(glshader::process::pragma_evaluation_context &context, float &value) {
	if(context.is_at_end()) {
		context.syntax_error("value expected");
		return false;
	}

	auto token = context.get();

	char *endptr;

	errno = 0;
	value = strtof(token.c_str(), &endptr);
	if(errno != 0 || endptr != token.data() + token.size() || token.empty()) {
		context.syntax_error("not a number");
		return false;
	}

	return true;
}

bool FilamentMaterialParser::process_pragma(glshader::process::pragma_evaluation_context &&context) {
	if(context.is_at_end()) {
		return false;
	}

	const auto &pragma = context.get();
	if(m_ubershaderVariant && pragma == "uber") {
		return true;
	}

	if(pragma != "material") {
		return false;
	}

	if(context.is_at_end()) {
		context.syntax_error("'#pragma material' should be followed by a material directive");
		return false;
	}

	const auto &directive = context.get();

	if(directive == "no_sampler_validation") {
		m_builder.noSamplerValidation(true);

	} else if(directive == "include_essl1") {
		m_builder.includeEssl1(true);

	} else if(directive == "name") {

		if(context.is_at_end()) {
			context.syntax_error("name expected");
			return true;
		}

		std::string name;
		do {
			if(!name.empty()) {
				name.push_back(' ');
			}

			name.append(context.get());
		} while(!context.is_at_end());

		if(m_ubershaderVariant) {
			name.push_back('_');
			name.append(m_ubershaderVariant->name());
		}

		m_builder.name(name.c_str());

	} else if(directive == "shading") {
		parseSingleEnumParameter(context, &filamat::MaterialBuilder::shading);

	} else if(directive == "interpolation") {
		parseSingleEnumParameter(context, &filamat::MaterialBuilder::interpolation);

	} else if(directive == "parameter") {

		if(context.is_at_end()) {
			context.syntax_error("one or more specifiers are expected");
			return true;
		}

		std::string name;
		std::variant<std::monostate, filamat::MaterialBuilder::UniformType, filamat::MaterialBuilder::SamplerType> valueType;
		std::optional<filamat::MaterialBuilder::Precision> precision;
		auto samplerFormat = filamat::MaterialBuilder::SamplerFormat::FLOAT;

		do {
			const auto &fullToken = context.get();
			if(context.is_at_end()) {
				name = fullToken;
			} else {
				const std::string *token = &fullToken;
				const std::string *detailPtr = nullptr;
				std::string type;
				std::string detail;

				auto delimiter = fullToken.find('_');
				if(delimiter != std::string::npos) {
					type = fullToken.substr(0, delimiter);
					detail = fullToken.substr(delimiter + 1);

					token = &type;
					detailPtr = &detail;
				}

				if(filamat::Enums::isValid<filamat::MaterialBuilder::UniformType>(*token)) {
					if(!std::holds_alternative<std::monostate>(valueType)) {
						context.syntax_error("only one type may be specified");

						return true;
					}

					valueType.emplace<filamat::MaterialBuilder::UniformType>(filamat::Enums::toEnum<filamat::MaterialBuilder::UniformType>(*token));

					if(detailPtr) {
						context.syntax_error("this specifier doesn't take a detail specifier");

						return true;
					}

				} else if(filamat::Enums::isValid<filamat::MaterialBuilder::SamplerType>(*token)) {
					if(!std::holds_alternative<std::monostate>(valueType)) {
						context.syntax_error("only one type may be specified");

						return true;
					}

					valueType.emplace<filamat::MaterialBuilder::SamplerType>(filamat::Enums::toEnum<filamat::MaterialBuilder::SamplerType>(*token));

					if(detailPtr) {
						if(!filamat::Enums::isValid<filamat::MaterialBuilder::SamplerFormat>(*detailPtr)) {
							context.syntax_error("sampler format specification is not valie");

							return true;
						}

						samplerFormat = filamat::Enums::toEnum<filamat::MaterialBuilder::SamplerFormat>(*detailPtr);
					}
				} else if(filamat::Enums::isValid<filamat::MaterialBuilder::Precision>(*token)) {
					if(precision.has_value()) {
						context.syntax_error("only one precision specifier may be specified");

						return true;
					}

					precision.emplace(filamat::Enums::toEnum<filamat::MaterialBuilder::Precision>(*token));

					if(detailPtr) {
						context.syntax_error("this specifier doesn't take a detail specifier");

						return true;
					}
				} else {
					context.syntax_error("invalid type specifier " + *token);

					return true;
				}
			}

		} while(!context.is_at_end());

		if(std::holds_alternative<std::monostate>(valueType)) {
			context.syntax_error("uniform type must be specified");
			return true;
		}

		auto arraySize = name.find('[');
		if(arraySize != std::string::npos) {
			size_t size;

			auto arraySizeEnd = name.find(']', arraySize + 1);
			if(arraySizeEnd != std::string::npos) {
				context.syntax_error("bad array size syntax");
				return true;
			}

			std::string_view arraySizeSpec = std::string_view(name).substr(arraySize + 1, arraySizeEnd - (arraySize + 1));
			if(arraySizeSpec.empty()) {
				size = std::numeric_limits<size_t>::max();
			} else {
				if(!convertNumber(context, arraySizeSpec, size)) {
					return true;
				}
			}

			name.erase(arraySize);

			if(std::holds_alternative<filamat::UniformType>(valueType)) {

				m_builder.parameter(name.c_str(), arraySize, std::get<filamat::UniformType>(valueType),
									precision.value_or(filamat::MaterialBuilder::Precision::DEFAULT));
			} else {
				context.syntax_error("sampler arrays are not supported");
				return true;
			}

		} else {

			if(std::holds_alternative<filamat::UniformType>(valueType)) {
				m_builder.parameter(name.c_str(), std::get<filamat::UniformType>(valueType),
									precision.value_or(filamat::MaterialBuilder::Precision::DEFAULT));
			} else {
				m_builder.parameter(name.c_str(), std::get<filamat::SamplerType>(valueType),
									samplerFormat,
									precision.value_or(filamat::MaterialBuilder::Precision::DEFAULT));

			}
		}


	// TODO: constant
	// TODO: buffer, possibly?

	} else if(directive == "variable") {
		if(context.is_at_end()) {
			context.syntax_error("variable slot expected");
			return true;
		}

		const auto &slotToken = context.get();

		if(!filamat::Enums::isValid<filamat::MaterialBuilder::Variable>(slotToken)) {
			context.syntax_error("incorrect variable slot specified");
			return true;
		}

		if(context.is_at_end()) {
			context.syntax_error("variable name expected");
		}

		const auto &nameToken = context.get();

		m_builder.variable(filamat::Enums::toEnum<filamat::MaterialBuilder::Variable>(slotToken), nameToken.c_str());

	} else if(directive == "require") {
		parseSingleEnumParameter(context, &filamat::MaterialBuilder::require);

	} else if(directive == "domain") {
		parseSingleEnumParameter(context, &filamat::MaterialBuilder::materialDomain);

	} else if(directive == "quality") {
		parseSingleEnumParameter(context, &filamat::MaterialBuilder::quality);

	} else if(directive == "feature_level") {
		parseSingleEnumParameter(context, &filamat::MaterialBuilder::featureLevel);

	} else if(directive == "blending") {
		parseSingleEnumParameter(context, &filamat::MaterialBuilder::blending);

	} else if(directive == "post_lighting_blending") {
		parseSingleEnumParameter(context, &filamat::MaterialBuilder::postLightingBlending);

	} else if(directive == "vertex_domain") {
		parseSingleEnumParameter(context, &filamat::MaterialBuilder::vertexDomain);

	} else if(directive == "culling") {
		parseSingleEnumParameter(context, &filamat::MaterialBuilder::culling);

	} else if(directive == "color_write") {
		parseBooleanParameter(context, &filamat::MaterialBuilder::colorWrite);

	} else if(directive == "depth_write") {
		parseBooleanParameter(context, &filamat::MaterialBuilder::depthWrite);

	} else if(directive == "depth_culling") {
		parseBooleanParameter(context, &filamat::MaterialBuilder::depthCulling);

	} else if(directive == "instanced") {
		parseBooleanParameter(context, &filamat::MaterialBuilder::instanced);

	} else if(directive == "double_sided") {
		parseBooleanParameter(context, &filamat::MaterialBuilder::doubleSided);

	} else if(directive == "mask_threshold") {
		parseFloatParameter(context, &filamat::MaterialBuilder::maskThreshold);

	} else if(directive == "alpha_to_coverage") {
		parseBooleanParameter(context, &filamat::MaterialBuilder::alphaToCoverage);

	} else if(directive == "shadow_multiplier") {
		parseBooleanParameter(context, &filamat::MaterialBuilder::shadowMultiplier);

	} else if(directive == "transparent_shadow") {
		parseBooleanParameter(context, &filamat::MaterialBuilder::transparentShadow);

	} else if(directive == "specular_anti_aliasing") {
		parseBooleanParameter(context, &filamat::MaterialBuilder::specularAntiAliasing);

	} else if(directive == "specular_anti_aliasing_variance") {
		parseFloatParameter(context, &filamat::MaterialBuilder::specularAntiAliasingVariance);

	} else if(directive == "specular_anti_aliasing_threshold") {
		parseFloatParameter(context, &filamat::MaterialBuilder::specularAntiAliasingThreshold);

	} else if(directive == "clear_coat_ior_change") {
		parseBooleanParameter(context, &filamat::MaterialBuilder::clearCoatIorChange);

	} else if(directive == "flip_uv") {
		parseBooleanParameter(context, &filamat::MaterialBuilder::flipUV);

	} else if(directive == "multi_bounce_ao") {
		parseBooleanParameter(context, &filamat::MaterialBuilder::multiBounceAmbientOcclusion);

	} else if(directive == "specular_ao") {
		parseSingleEnumParameter(context, &filamat::MaterialBuilder::specularAmbientOcclusion);

	} else if(directive == "refraction_type") {
		parseSingleEnumParameter(context, &filamat::MaterialBuilder::refractionType);

	} else if(directive == "refraction_mode") {
		parseSingleEnumParameter(context, &filamat::MaterialBuilder::refractionMode);

	} else if(directive == "reflection_mode") {
		parseSingleEnumParameter(context, &filamat::MaterialBuilder::reflectionMode);

	} else if(directive == "transparency_mode") {
		parseSingleEnumParameter(context, &filamat::MaterialBuilder::transparencyMode);

	} else if(directive == "custom_surface_shading") {
		parseBooleanParameter(context, &filamat::MaterialBuilder::customSurfaceShading);

	} else if(directive == "platform") {
		parseSingleEnumParameter(context, &filamat::MaterialBuilder::platform);

	} else if(directive == "target_api") {
		parseSingleEnumParameter(context, &filamat::MaterialBuilder::targetApi);

	} else if(directive == "optimization") {
		parseSingleEnumParameter(context, &filamat::MaterialBuilder::optimization);

	// TODO: post processing outputs?

	} else if(directive == "enable_framebuffer_fetch") {
		m_builder.enableFramebufferFetch();

	} else if(directive == "vertex_domain_device_jittered") {
		parseBooleanParameter(context, &filamat::MaterialBuilder::vertexDomainDeviceJittered);

	} else if(directive == "use_legacy_morphing") {
		m_builder.useLegacyMorphing();

	} else if(directive == "group_size") {
		filament::math::uint3 groupSize;

		if(!convertNumber(context, groupSize.x)) {
			return true;
		}

		if(!convertNumber(context, groupSize.y)) {
			return true;
		}

		if(!convertNumber(context, groupSize.z)) {
			return true;
		}

		m_builder.groupSize(groupSize);

	} else if(directive == "fragment") {
		switchOutputStream(context, OutputStream::Fragment);

	} else if(directive == "vertex") {
		switchOutputStream(context, OutputStream::Vertex);

	} else {

		context.syntax_error("unsupported '#pragma material' directive '" + directive + "'");
	}

	return true;
}

void FilamentMaterialParser::switchOutputStream(glshader::process::pragma_evaluation_context &context, OutputStream stream) {
	if(m_outputStream == stream) {
		return;
	}

	m_outputStream = stream;

	if(stream == OutputStream::Vertex) {
		m_fragmentStage.emplace(context.output.str());
		context.output.str(m_vertexStage.value_or(std::string()));
	} else {
		m_vertexStage.emplace(context.output.str());
		context.output.str(m_fragmentStage.value_or(std::string()));
	}
}


filamat::Package FilamentMaterialParser::compileMaterialFile(const std::filesystem::path &filename, utils::JobSystem &jobs,
															 const FilamentUbershaderVariant *ubershaderVariant) {

	glshader::process::state state;
	state.add_include_dir(filename.parent_path());

	FilamentMaterialParser parser(ubershaderVariant);

	glshader::process::preprocess_file_info file;
	file.file_path = filename;
	file.pragma_processor = &parser;

	if(ubershaderVariant) {
		file.definitions = ubershaderVariant->definitions();
	}

	auto processed = state.preprocess_file(file);
	if(!processed.valid()) {
		fprintf(stderr, "The material has failed the preprocessing stage:\n");
		for(const auto &error : processed.errors) {
			fprintf(stderr, "%s\n", error.c_str());

		}

		return filamat::Package::invalidPackage();
	}

	return parser.finalize(std::move(processed.contents), jobs);
}

