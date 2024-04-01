#include "filament/filament_material_property_bundle.h"
#include "filament/filament_rendering_server_backend.h"
#include "filament/filament_texture_reference_object.h"

#include <filament/MaterialInstance.h>
#include <filament/Material.h>

#include <filament/TextureSampler.h>

#include <cstdio>

FilamentMaterialPropertyBundle::FilamentMaterialPropertyBundle() = default;

FilamentMaterialPropertyBundle::~FilamentMaterialPropertyBundle() = default;


FilamentMaterialPropertyBundle::FilamentMaterialPropertyBundle(FilamentMaterialPropertyBundle &&other) noexcept = default;

FilamentMaterialPropertyBundle &FilamentMaterialPropertyBundle::operator =(FilamentMaterialPropertyBundle &&other) noexcept = default;


void FilamentMaterialPropertyBundle::apply(filament::MaterialInstance *materialInstance) const {
	auto material = materialInstance->getMaterial();

	for(int index = 0, size = m_params.size(); index < size; index++) {
		auto key = static_cast<String>(m_params.get_key_at_index(index)).utf8();
		auto value = m_params.get_value_at_index(index);

		if(!material->hasParameter(key.ptr())) {
			fprintf(stderr, "FilamentMaterialPropertyBundle::apply: material instance %s doesn't have a parameter named %s\n",
					materialInstance->getName(), key.ptr());
			continue;
		}


		switch(value.get_type()) {
			case Variant::RID:
			{
				if(!material->isSampler(key.ptr())) {
					fprintf(stderr, "FilamentMaterialPropertyBundle::apply: material instance %s's parameter %s is not a sampler\n",
							materialInstance->getName(), key.ptr());
					continue;
				}

				filament::Texture * texture = nullptr;
				auto textureObject = FilamentRenderingServerBackend::get()->objects().resolve<FilamentTextureReferenceObject>(static_cast<RID>(value));
				if(textureObject) {
					texture = textureObject->texture();
				}
				/*
				 * TODO: some sort of Sampler management.
				 */
				filament::TextureSampler sampler(
					filament::TextureSampler::MinFilter::LINEAR_MIPMAP_LINEAR,
					filament::TextureSampler::MagFilter::LINEAR,
					filament::TextureSampler::WrapMode::REPEAT,
					filament::TextureSampler::WrapMode::REPEAT,
					filament::TextureSampler::WrapMode::REPEAT);
				sampler.setAnisotropy(4.0f);

				materialInstance->setParameter(key.ptr(), key.length(), texture, sampler);
				break;
			}

			default:
				fprintf(stderr, "FilamentMaterialPropertyBundle::apply: unsupported value %s of type %d, applying onto %s\n", key.ptr(), static_cast<int32_t>(value.get_type()), materialInstance->getName());
		}
	}
}

Variant FilamentMaterialPropertyBundle::getParam(const StringName &name) const {
	return m_params[name];
}

void FilamentMaterialPropertyBundle::setParam(const StringName &name, const Variant &value) {
	m_params[name] = value;

	/*
	 * TODO: special texture handling: textures need to be tracked via ControlledReferences)
	 */
}
