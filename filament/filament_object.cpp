#include "filament/filament_object.h"

FilamentObject::FilamentObject() = default;

FilamentObject::~FilamentObject() = default;

void FilamentObject::addReference(FilamentControlledObjectReferenceBase &reference) noexcept {
	m_referencesToThisObject.insertTail(&reference);
}

void FilamentObject::objectAboutToInvalidate() {
	for(auto ref = m_referencesToThisObject.next(); ref != &m_referencesToThisObject; ref = ref->next()) {
		static_cast<FilamentControlledObjectReferenceBase *>(ref)->referencedObjectIsAboutToInvalidate();
	}
}

void FilamentObject::controlledObjectAboutToInvalidate(FilamentControlledObjectReferenceBase *linkedViaReference) {
	(void)linkedViaReference;
}
