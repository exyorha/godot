#include "filament/filament_shader_object.h"

#include "filament/filament_rendering_server_backend.h"

#include <filament/Engine.h>
#include <filament/Material.h>

FilamentShaderObject::FilamentShaderObject() {

}

FilamentShaderObject::~FilamentShaderObject() = default;

void FilamentShaderObject::createBuiltin(const unsigned char *filamentMaterialData, size_t size) {
	objectAboutToInvalidate();

	if(size == 0) {
		m_material = FilamentEngineObject<filament::Material>();
	} else {
		m_material = FilamentEngineObject<filament::Material>(
			filament::Material::Builder()
				.package(filamentMaterialData, size)
				.build(*FilamentRenderingServerBackend::filamentEngine())
		);

		if(!m_material) {
			fprintf(stderr, "failed to create a Filament material from a package");
		}
	}
}

FilamentEngineObject<filament::MaterialInstance> FilamentShaderObject::createNewMaterialInstance() const {
	if(!m_material) {
		return FilamentEngineObject<filament::MaterialInstance>();
	}

	FilamentEngineObject<filament::MaterialInstance> result(m_material->createInstance());

	if(!result) {
		throw std::runtime_error("failed to create the material instance");
	}

	m_properties.apply(result.get());

	return result;
}

void FilamentShaderObject::applyOntoExistingMaterialInstance(filament::MaterialInstance *instance) const {
	m_properties.apply(instance);
}

Variant FilamentShaderObject::getParam(const StringName &name) {
	return m_properties.getParam(name);
}

void FilamentShaderObject::setParam(const StringName &name, const Variant &value) {
	m_properties.setParam(name, value);
	recalculateDerivedMaterials();
}

void FilamentShaderObject::getShaderParameterList(List<PropertyInfo> &parameterList) {
	if(!m_material) {
		return;
	}

	std::vector<filament::Material::ParameterInfo> parameters(m_material->getParameterCount());

	m_material->getParameters(parameters.data(), parameters.size());

	for(const auto &parameter: parameters) {
		if(parameter.isSubpass) {
			fprintf(stderr, "FilamentShaderObject::getShaderParameterList: material '%s' has a subpass parameter '%s', this is not currently supported\n",
					m_material->getName(), parameter.name);
			continue;
		}

		PropertyInfo out;
		out.name = parameter.name;

		if(parameter.isSampler) {
			const char *textureClass;

			switch(parameter.samplerType) {
				case filament::Material::SamplerType::SAMPLER_2D:
					textureClass = "Texture2D";
					break;

				case filament::Material::SamplerType::SAMPLER_2D_ARRAY:
				case filament::Material::SamplerType::SAMPLER_CUBEMAP:
				case filament::Material::SamplerType::SAMPLER_CUBEMAP_ARRAY:
					textureClass = "TextureLayered";
					break;

				case filament::Material::SamplerType::SAMPLER_3D:
					textureClass = "Texture3D";
					break;

				default:
					fprintf(stderr, "FilamentShaderObject::getShaderParameterList: material '%s' has an unsupported texture sampler %s of type %d\n",
							m_material->getName(), parameter.name, static_cast<int32_t>(parameter.samplerType));
					continue;
			}

			if(parameter.count > 1) {
				out.type = Variant::ARRAY;
				out.hint = PROPERTY_HINT_ARRAY_TYPE;
				out.hint_string = MAKE_RESOURCE_TYPE_HINT(textureClass);
			} else {
				out.type = Variant::OBJECT;
				out.hint = PROPERTY_HINT_RESOURCE_TYPE;
				out.hint_string = textureClass;
			}
		} else {
			switch(parameter.type) {
				case filament::Material::ParameterType::BOOL:
					if (parameter.count > 1) {
						out.type = Variant::PACKED_INT32_ARRAY;
					} else {
						out.type = Variant::BOOL;
					}
					break;

				case filament::Material::ParameterType::BOOL2:
					if (parameter.count > 1) {
						out.type = Variant::PACKED_INT32_ARRAY;
					} else {
						out.type = Variant::INT;
						out.hint = PROPERTY_HINT_FLAGS;
						out.hint_string = "x,y";
					}
					break;

				case filament::Material::ParameterType::BOOL3:
					if (parameter.count > 1) {
						out.type = Variant::PACKED_INT32_ARRAY;
					} else {
						out.type = Variant::INT;
						out.hint = PROPERTY_HINT_FLAGS;
						out.hint_string = "x,y,z";
					}
					break;

				case filament::Material::ParameterType::BOOL4:
					if (parameter.count > 1) {
						out.type = Variant::PACKED_INT32_ARRAY;
					} else {
						out.type = Variant::INT;
						out.hint = PROPERTY_HINT_FLAGS;
						out.hint_string = "x,y,z,w";
					}
					break;

				case filament::Material::ParameterType::FLOAT:
					if (parameter.count > 1) {
						out.type = Variant::PACKED_FLOAT32_ARRAY;
					} else {
						out.type = Variant::FLOAT;
					}
					break;

				case filament::Material::ParameterType::FLOAT2:
					if (parameter.count > 1) {
						out.type = Variant::PACKED_VECTOR2_ARRAY;
					} else {
						out.type = Variant::VECTOR2;
					}
					break;

				case filament::Material::ParameterType::FLOAT3:
					if (parameter.count > 1) {
						out.type = Variant::PACKED_VECTOR3_ARRAY;
					} else {
						out.type = Variant::VECTOR3;
					}
					break;

				case filament::Material::ParameterType::FLOAT4:
					if (parameter.count > 1) {
						out.type = Variant::PACKED_FLOAT32_ARRAY;
					} else {
						out.type = Variant::VECTOR4;
					}
					break;

				case filament::Material::ParameterType::UINT:
				case filament::Material::ParameterType::INT:
					if (parameter.count > 1) {
						out.type = Variant::PACKED_INT32_ARRAY;
					} else {
						out.type = Variant::INT;
					}
					break;

				case filament::Material::ParameterType::UINT2:
				case filament::Material::ParameterType::INT2:
					if (parameter.count > 1) {
						out.type = Variant::PACKED_INT32_ARRAY;
					} else {
						out.type = Variant::VECTOR2I;
					}
					break;

				case filament::Material::ParameterType::UINT3:
				case filament::Material::ParameterType::INT3:
					if (parameter.count > 1) {
						out.type = Variant::PACKED_INT32_ARRAY;
					} else {
						out.type = Variant::VECTOR3I;
					}
					break;

				case filament::Material::ParameterType::UINT4:
				case filament::Material::ParameterType::INT4:
					if (parameter.count > 1) {
						out.type = Variant::PACKED_INT32_ARRAY;
					} else {
						out.type = Variant::VECTOR4I;
					}
					break;

				case filament::Material::ParameterType::MAT3:
					if (parameter.count > 1) {
						out.type = Variant::PACKED_FLOAT32_ARRAY;
					} else {
						out.type = Variant::BASIS;
					}
					break;

				case filament::Material::ParameterType::MAT4:
					if (parameter.count > 1) {
						out.type = Variant::PACKED_FLOAT32_ARRAY;
					} else {
						out.type = Variant::PROJECTION;
					}
					break;

				// We don't support:
				// STRUCT
				default:
					fprintf(stderr, "FilamentShaderObject::getShaderParameterList: material '%s' has an unsupported parameter %s of type %d\n",
							m_material->getName(), parameter.name, static_cast<int32_t>(parameter.type));
					continue;
			}
		}

		parameterList.push_back(out);
	}

}

String FilamentShaderObject::name() const {
	if(m_material) {
		return String::utf8(m_material->getName());
	} else {
		return String();
	}
}
