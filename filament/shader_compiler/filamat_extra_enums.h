#ifndef FILAMAT_EXTRA_ENUMS_H
#define FILAMAT_EXTRA_ENUMS_H

#include <filamat/Enums.h>

namespace filamat_extra_enums {
	extern std::unordered_map<std::string, filamat::MaterialBuilder::Shading> shading;
	extern std::unordered_map<std::string, filamat::MaterialBuilder::Interpolation> interpolation;
	extern std::unordered_map<std::string, filamat::MaterialBuilder::VertexAttribute> vertexAttribute;
	extern std::unordered_map<std::string, filamat::MaterialBuilder::MaterialDomain> materialDomain;
	extern std::unordered_map<std::string, filamat::MaterialBuilder::ShaderQuality> shaderQuality;
	extern std::unordered_map<std::string, filamat::MaterialBuilder::FeatureLevel> featureLevel;
	extern std::unordered_map<std::string, filamat::MaterialBuilder::BlendingMode> blendingMode;
	extern std::unordered_map<std::string, filamat::MaterialBuilder::VertexDomain> vertexDomain;
	extern std::unordered_map<std::string, filamat::MaterialBuilder::CullingMode> cullingMode;
	extern std::unordered_map<std::string, filamat::MaterialBuilder::SpecularAmbientOcclusion> specularAmbientOcclusion;
	extern std::unordered_map<std::string, filamat::MaterialBuilder::RefractionType> refractionType;
	extern std::unordered_map<std::string, filamat::MaterialBuilder::RefractionMode> refractionMode;
	extern std::unordered_map<std::string, filamat::MaterialBuilder::ReflectionMode> reflectionMode;
	extern std::unordered_map<std::string, filamat::MaterialBuilder::TransparencyMode> transparencyMode;
	extern std::unordered_map<std::string, filamat::MaterialBuilder::Platform> platform;
	extern std::unordered_map<std::string, filamat::MaterialBuilder::TargetApi> targetApi;
	extern std::unordered_map<std::string, filamat::MaterialBuilder::Optimization> optimization;
	extern std::unordered_map<std::string, filamat::MaterialBuilder::Variable> variable;
}

template<>
inline std::unordered_map<std::string, filamat::MaterialBuilder::Shading>& filamat::Enums::getMap<filamat::MaterialBuilder::Shading>() noexcept {
	return filamat_extra_enums::shading;
}

template<>
inline std::unordered_map<std::string, filamat::MaterialBuilder::Interpolation>& filamat::Enums::getMap<filamat::MaterialBuilder::Interpolation>() noexcept {
	return filamat_extra_enums::interpolation;
}

template<>
inline std::unordered_map<std::string, filamat::MaterialBuilder::VertexAttribute>& filamat::Enums::getMap<filamat::MaterialBuilder::VertexAttribute>() noexcept {
	return filamat_extra_enums::vertexAttribute;
}

template<>
inline std::unordered_map<std::string, filamat::MaterialBuilder::MaterialDomain>& filamat::Enums::getMap<filamat::MaterialBuilder::MaterialDomain>() noexcept {
	return filamat_extra_enums::materialDomain;
}

template<>
inline std::unordered_map<std::string, filamat::MaterialBuilder::ShaderQuality>& filamat::Enums::getMap<filamat::MaterialBuilder::ShaderQuality>() noexcept {
	return filamat_extra_enums::shaderQuality;
}

template<>
inline std::unordered_map<std::string, filamat::MaterialBuilder::FeatureLevel>& filamat::Enums::getMap<filamat::MaterialBuilder::FeatureLevel>() noexcept {
	return filamat_extra_enums::featureLevel;
}

template<>
inline std::unordered_map<std::string, filamat::MaterialBuilder::BlendingMode>& filamat::Enums::getMap<filamat::MaterialBuilder::BlendingMode>() noexcept {
	return filamat_extra_enums::blendingMode;
}

template<>
inline std::unordered_map<std::string, filamat::MaterialBuilder::VertexDomain>& filamat::Enums::getMap<filamat::MaterialBuilder::VertexDomain>() noexcept {
	return filamat_extra_enums::vertexDomain;
}

template<>
inline std::unordered_map<std::string, filamat::MaterialBuilder::CullingMode>& filamat::Enums::getMap<filamat::MaterialBuilder::CullingMode>() noexcept {
	return filamat_extra_enums::cullingMode;
}

template<>
inline std::unordered_map<std::string, filamat::MaterialBuilder::SpecularAmbientOcclusion>&
	filamat::Enums::getMap<filamat::MaterialBuilder::SpecularAmbientOcclusion>() noexcept {
	return filamat_extra_enums::specularAmbientOcclusion;
}

template<>
inline std::unordered_map<std::string, filamat::MaterialBuilder::RefractionType>& filamat::Enums::getMap<filamat::MaterialBuilder::RefractionType>() noexcept {
	return filamat_extra_enums::refractionType;
}

template<>
inline std::unordered_map<std::string, filamat::MaterialBuilder::RefractionMode>& filamat::Enums::getMap<filamat::MaterialBuilder::RefractionMode>() noexcept {
	return filamat_extra_enums::refractionMode;
}

template<>
inline std::unordered_map<std::string, filamat::MaterialBuilder::ReflectionMode>& filamat::Enums::getMap<filamat::MaterialBuilder::ReflectionMode>() noexcept {
	return filamat_extra_enums::reflectionMode;
}

template<>
inline std::unordered_map<std::string, filamat::MaterialBuilder::TransparencyMode>&
	filamat::Enums::getMap<filamat::MaterialBuilder::TransparencyMode>() noexcept {

	return filamat_extra_enums::transparencyMode;
}

template<>
inline std::unordered_map<std::string, filamat::MaterialBuilder::Platform>& filamat::Enums::getMap<filamat::MaterialBuilder::Platform>() noexcept {
	return filamat_extra_enums::platform;
}

template<>
inline std::unordered_map<std::string, filamat::MaterialBuilder::TargetApi>& filamat::Enums::getMap<filamat::MaterialBuilder::TargetApi>() noexcept {
	return filamat_extra_enums::targetApi;
}

template<>
inline std::unordered_map<std::string, filamat::MaterialBuilder::Optimization>& filamat::Enums::getMap<filamat::MaterialBuilder::Optimization>() noexcept {
	return filamat_extra_enums::optimization;
}

template<>
inline std::unordered_map<std::string, filamat::MaterialBuilder::Variable>& filamat::Enums::getMap<filamat::MaterialBuilder::Variable>() noexcept {
	return filamat_extra_enums::variable;
}

/*
 * For some reason, the linker can't find the definitions of the following
 * specializations in filamat (and only *these* symbols) when linking Godot.
 * I can't figure it out, so I'm duplicating them belong as inlines.
 */

template <>
inline std::unordered_map<std::string, filamat::MaterialBuilder::UniformType>& filamat::Enums::getMap<filamat::MaterialBuilder::UniformType>() noexcept {
    return mStringToUniformType;
};


#endif
