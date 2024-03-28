#ifndef FILAMENT_OBJECT_H
#define FILAMENT_OBJECT_H

#include <memory>

#include "filament_dirty_list_entry.h"
#include "filament_controlled_object_reference.h"

class FilamentObject : public std::enable_shared_from_this<FilamentObject>, protected FilamentDirtyListEntry, protected FilamentControlledObjectReferenceOwner {
protected:
	FilamentObject();

public:
	virtual ~FilamentObject();

	FilamentObject(const FilamentObject &other) = delete;
	FilamentObject &operator =(const FilamentObject &other) = delete;

	void addReference(FilamentControlledObjectReferenceBase &reference) noexcept;

protected:
	void objectAboutToInvalidate();

	void controlledObjectAboutToInvalidate(FilamentControlledObjectReferenceBase *linkedViaReference) override;

private:
	FilamentLinkedListEntry m_referencesToThisObject;
};

#endif
