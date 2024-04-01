#include "filament/filament_material_object.h"
#include "filament/filament_shader_object.h"

#include <filament/MaterialInstance.h>

FilamentMaterialObject::FilamentMaterialObject() : m_parent(this), m_fallbackParent(this), m_createdFromFallback(false) {

}

FilamentMaterialObject::~FilamentMaterialObject() = default;

void FilamentMaterialObject::resetMaterialInstance() {

	if(m_material) {
		objectAboutToInvalidate();
		m_material = FilamentEngineObject<filament::MaterialInstance>();
	}

	markDirty();
}

void FilamentMaterialObject::setParent(const std::shared_ptr<FilamentMaterialInstanceSource> &parent) {
	if(m_parent != parent) {
		resetMaterialInstance();

		m_parent = parent;
	}
}

void FilamentMaterialObject::setFallbackParent(const std::shared_ptr<FilamentMaterialInstanceSource> &parent) {
	if(m_fallbackParent != parent) {
		resetMaterialInstance();

		m_fallbackParent = parent;
	}
}

filament::MaterialInstance *FilamentMaterialObject::materialInstance() {
	clean();

	return m_material.get();
}

void FilamentMaterialObject::doClean() {
	if(m_material) {
		if(m_createdFromFallback) {
			if(m_fallbackParent) {
				m_fallbackParent->applyOntoExistingMaterialInstance(m_material.get());
			}
		} else {
			if(m_parent) {
				m_parent->applyOntoExistingMaterialInstance(m_material.get());
			}
		}
	} else {
		m_createdFromFallback = false;
		if(m_parent) {
			m_material = m_parent->createNewMaterialInstance();
		}

		if(!m_material && m_fallbackParent) {
			m_createdFromFallback = true;

			m_material = m_fallbackParent->createNewMaterialInstance();
		}
	}

	if(m_material) {
		m_properties.apply(m_material.get());
	}
}

void FilamentMaterialObject::controlledObjectAboutToInvalidate(FilamentControlledObjectReferenceBase *linkedViaReference) {
	FilamentObject::controlledObjectAboutToInvalidate(linkedViaReference);

	if(linkedViaReference == &m_parent) {
		if(!m_createdFromFallback) {
			resetMaterialInstance();
		}
	} else if(linkedViaReference == &m_fallbackParent) {
		if(m_createdFromFallback) {
			resetMaterialInstance();
		}
	}
}

FilamentEngineObject<filament::MaterialInstance> FilamentMaterialObject::createNewMaterialInstance() const {
	FilamentEngineObject<filament::MaterialInstance> materialInstance;

	if(m_parent) {
		materialInstance = m_parent->createNewMaterialInstance();
	}

	if(m_fallbackParent && !materialInstance) {
		materialInstance = m_fallbackParent->createNewMaterialInstance();
	}

	if(materialInstance) {
		m_properties.apply(materialInstance.get());
	}

	return materialInstance;
}

void FilamentMaterialObject::applyOntoExistingMaterialInstance(filament::MaterialInstance *instance) const {

	if(m_parent) {
		m_parent->applyOntoExistingMaterialInstance(instance);
	}

	m_properties.apply(instance);
}

Variant FilamentMaterialObject::getParam(const StringName &name) {
	auto value = m_properties.getParam(name);
	if(!value.is_null()) {
		return value;
	}

	clean();
	if(m_createdFromFallback) {
		if(m_fallbackParent) {
			return m_fallbackParent->getParam(name);
		}
	} else {
		if(m_parent) {
			return m_parent->getParam(name);
		}
	}

	return Variant();
}

void FilamentMaterialObject::setParam(const StringName &name, const Variant &value) {

	markDirty();

	m_properties.setParam(name, value);

	recalculateDerivedMaterials();
}
