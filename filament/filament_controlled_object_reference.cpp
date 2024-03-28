#include "filament_controlled_object_reference.h"
#include "filament_object.h"

FilamentControlledObjectReferenceOwner::FilamentControlledObjectReferenceOwner() = default;

FilamentControlledObjectReferenceOwner::~FilamentControlledObjectReferenceOwner() = default;

FilamentControlledObjectReferenceBase::FilamentControlledObjectReferenceBase(FilamentControlledObjectReferenceOwner *owner) noexcept : m_owner(owner) {

}

FilamentControlledObjectReferenceBase::~FilamentControlledObjectReferenceBase() = default;

FilamentControlledObjectReferenceBase::FilamentControlledObjectReferenceBase(FilamentControlledObjectReferenceBase &&other) noexcept = default;

FilamentControlledObjectReferenceBase &FilamentControlledObjectReferenceBase::operator =(FilamentControlledObjectReferenceBase &&other) noexcept = default;

void FilamentControlledObjectReferenceBase::assignReference(const std::shared_ptr<FilamentObject> &reference) noexcept {
	remove();
	m_reference.reset();

	m_reference = reference;
	if(reference) {
		reference->addReference(*this);
	}
}
