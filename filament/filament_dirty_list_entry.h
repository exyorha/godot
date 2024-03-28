#ifndef FILAMENT_DIRTY_LIST_ENTRY_H
#define FILAMENT_DIRTY_LIST_ENTRY_H

#include "filament/filament_linked_list_entry.h"

class FilamentDirtyListEntry : public FilamentLinkedListEntry {
public:
	FilamentDirtyListEntry();
	~FilamentDirtyListEntry();

	FilamentDirtyListEntry(const FilamentDirtyListEntry &other) = delete;
	FilamentDirtyListEntry &operator =(const FilamentDirtyListEntry &other) = delete;

	FilamentDirtyListEntry(FilamentDirtyListEntry &&other) noexcept;
	FilamentDirtyListEntry &operator =(FilamentDirtyListEntry &&other) noexcept;

	inline bool isDirty() const noexcept {
		return !isEmpty();
	}

	void clean();

protected:
	void markDirty() noexcept;
	virtual void doClean();
};

#endif
