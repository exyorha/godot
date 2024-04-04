#include "filamat_extra_enums.h"

namespace filamat_extra_enums {
	 std::unordered_map<std::string, filamat::MaterialBuilder::Shading> shading {
		{ "cloth",              filamat::MaterialBuilder::Shading::CLOTH },
		{ "lit",                filamat::MaterialBuilder::Shading::LIT },
		{ "subsurface",         filamat::MaterialBuilder::Shading::SUBSURFACE },
		{ "unlit",              filamat::MaterialBuilder::Shading::UNLIT },
		{ "specularGlossiness", filamat::MaterialBuilder::Shading::SPECULAR_GLOSSINESS },
	 };

	 std::unordered_map<std::string, filamat::MaterialBuilder::Interpolation> interpolation{
		{ "smooth", filamat::MaterialBuilder::Interpolation::SMOOTH },
        { "flat", filamat::MaterialBuilder::Interpolation::FLAT },
	 };

	 std::unordered_map<std::string, filamat::MaterialBuilder::VertexAttribute> vertexAttribute{
        { "color", filamat::MaterialBuilder::VertexAttribute::COLOR },
        { "position", filamat::MaterialBuilder::VertexAttribute::POSITION },
        { "tangents", filamat::MaterialBuilder::VertexAttribute::TANGENTS },
        { "uv0", filamat::MaterialBuilder::VertexAttribute::UV0 },
        { "uv1", filamat::MaterialBuilder::VertexAttribute::UV1 },
        { "custom0", filamat::MaterialBuilder::VertexAttribute::CUSTOM0 },
        { "custom1", filamat::MaterialBuilder::VertexAttribute(filamat::MaterialBuilder::VertexAttribute::CUSTOM0 + 1) },
        { "custom2", filamat::MaterialBuilder::VertexAttribute(filamat::MaterialBuilder::VertexAttribute::CUSTOM0 + 2) },
        { "custom3", filamat::MaterialBuilder::VertexAttribute(filamat::MaterialBuilder::VertexAttribute::CUSTOM0 + 3) },
        { "custom4", filamat::MaterialBuilder::VertexAttribute(filamat::MaterialBuilder::VertexAttribute::CUSTOM0 + 4) },
        { "custom5", filamat::MaterialBuilder::VertexAttribute(filamat::MaterialBuilder::VertexAttribute::CUSTOM0 + 5) },
        { "custom6", filamat::MaterialBuilder::VertexAttribute(filamat::MaterialBuilder::VertexAttribute::CUSTOM0 + 6) },
        { "custom7", filamat::MaterialBuilder::VertexAttribute(filamat::MaterialBuilder::VertexAttribute::CUSTOM0 + 7) },
	 };

	 std::unordered_map<std::string, filamat::MaterialBuilder::MaterialDomain> materialDomain{
        { "surface",            filamat::MaterialBuilder::MaterialDomain::SURFACE },
        { "postprocess",        filamat::MaterialBuilder::MaterialDomain::POST_PROCESS },
        { "compute",            filamat::MaterialBuilder::MaterialDomain::COMPUTE },
	 };

	 std::unordered_map<std::string, filamat::MaterialBuilder::ShaderQuality> shaderQuality{
        { "low", filamat::MaterialBuilder::ShaderQuality::LOW },
        { "normal", filamat::MaterialBuilder::ShaderQuality::NORMAL },
        { "high", filamat::MaterialBuilder::ShaderQuality::HIGH },
        { "default", filamat::MaterialBuilder::ShaderQuality::DEFAULT },
	 };

	 std::unordered_map<std::string, filamat::MaterialBuilder::FeatureLevel> featureLevel{
		 { "0", filamat::MaterialBuilder::FeatureLevel::FEATURE_LEVEL_0 },
		 { "1", filamat::MaterialBuilder::FeatureLevel::FEATURE_LEVEL_1 },
		 { "2", filamat::MaterialBuilder::FeatureLevel::FEATURE_LEVEL_2 },
		 { "3", filamat::MaterialBuilder::FeatureLevel::FEATURE_LEVEL_3 },
	 };

	 std::unordered_map<std::string, filamat::MaterialBuilder::BlendingMode> blendingMode{
        { "add", filamat::MaterialBuilder::BlendingMode::ADD },
        { "masked", filamat::MaterialBuilder::BlendingMode::MASKED },
        { "opaque", filamat::MaterialBuilder::BlendingMode::OPAQUE },
        { "transparent", filamat::MaterialBuilder::BlendingMode::TRANSPARENT },
        { "fade", filamat::MaterialBuilder::BlendingMode::FADE },
        { "multiply", filamat::MaterialBuilder::BlendingMode::MULTIPLY },
        { "screen", filamat::MaterialBuilder::BlendingMode::SCREEN },
	 };

	 std::unordered_map<std::string, filamat::MaterialBuilder::VertexDomain> vertexDomain{
        { "device", filamat::MaterialBuilder::VertexDomain::DEVICE},
        { "object", filamat::MaterialBuilder::VertexDomain::OBJECT},
        { "world", filamat::MaterialBuilder::VertexDomain::WORLD},
        { "view", filamat::MaterialBuilder::VertexDomain::VIEW},
	 };

	 std::unordered_map<std::string, filamat::MaterialBuilder::CullingMode> cullingMode{
        { "back", filamat::MaterialBuilder::CullingMode::BACK },
        { "front", filamat::MaterialBuilder::CullingMode::FRONT },
        { "frontAndBack", filamat::MaterialBuilder::CullingMode::FRONT_AND_BACK },
        { "none", filamat::MaterialBuilder::CullingMode::NONE },
	 };

	 std::unordered_map<std::string, filamat::MaterialBuilder::SpecularAmbientOcclusion> specularAmbientOcclusion{
		{ "none",        filamat::MaterialBuilder::SpecularAmbientOcclusion::NONE },
		{ "simple",      filamat::MaterialBuilder::SpecularAmbientOcclusion::SIMPLE },
		{ "bentNormals", filamat::MaterialBuilder::SpecularAmbientOcclusion::BENT_NORMALS }
	 };

	 std::unordered_map<std::string, filamat::MaterialBuilder::RefractionType> refractionType{
		{ "solid", filamat::MaterialBuilder::RefractionType::SOLID },
		{ "thin",  filamat::MaterialBuilder::RefractionType::THIN },
	 };

	 std::unordered_map<std::string, filamat::MaterialBuilder::RefractionMode> refractionMode{
		{ "none",        filamat::MaterialBuilder::RefractionMode::NONE },
		{ "cubemap",     filamat::MaterialBuilder::RefractionMode::CUBEMAP },
		{ "screenspace", filamat::MaterialBuilder::RefractionMode::SCREEN_SPACE },
	 };

	 std::unordered_map<std::string, filamat::MaterialBuilder::ReflectionMode> reflectionMode{
		{ "default", filamat::MaterialBuilder::ReflectionMode ::DEFAULT },
		{ "screenspace", filamat::MaterialBuilder::ReflectionMode::SCREEN_SPACE },
	 };

	 std::unordered_map<std::string, filamat::MaterialBuilder::TransparencyMode> transparencyMode{
        { "default", filamat::MaterialBuilder::TransparencyMode::DEFAULT },
        { "twoPassesOneSide", filamat::MaterialBuilder::TransparencyMode::TWO_PASSES_ONE_SIDE },
        { "twoPassesTwoSides", filamat::MaterialBuilder::TransparencyMode::TWO_PASSES_TWO_SIDES },
	 };

	 std::unordered_map<std::string, filamat::MaterialBuilder::Platform> platform{
		 { "desktop", filamat::MaterialBuilder::Platform::DESKTOP },
		 { "mobile", filamat::MaterialBuilder::Platform::MOBILE },
		 { "all", filamat::MaterialBuilder::Platform::ALL },
	 };

	 std::unordered_map<std::string, filamat::MaterialBuilder::TargetApi> targetApi{
		 { "opengl", filamat::MaterialBuilder::TargetApi::OPENGL },
		 { "vulkan", filamat::MaterialBuilder::TargetApi::VULKAN },
		 { "metal", filamat::MaterialBuilder::TargetApi::METAL },
		 { "all", filamat::MaterialBuilder::TargetApi::ALL },
	 };

	 std::unordered_map<std::string, filamat::MaterialBuilder::Optimization> optimization {
		 { "none", filamat::MaterialBuilder::Optimization::NONE },
		 { "preprocessor", filamat::MaterialBuilder::Optimization::PREPROCESSOR },
		 { "size", filamat::MaterialBuilder::Optimization::SIZE },
		 { "performance", filamat::MaterialBuilder::Optimization::PERFORMANCE },
	 };

	 std::unordered_map<std::string, filamat::MaterialBuilder::Variable> variable {
		 { "custom0", filamat::MaterialBuilder::Variable::CUSTOM0 },
		 { "custom1", filamat::MaterialBuilder::Variable::CUSTOM1 },
		 { "custom2", filamat::MaterialBuilder::Variable::CUSTOM2 },
		 { "custom3", filamat::MaterialBuilder::Variable::CUSTOM3 }
	 };
}
