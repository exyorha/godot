#ifndef FILAMENT_DIRTY_LIST_H
#define FILAMENT_DIRTY_LIST_H

#include "filament_linked_list_entry.h"

class FilamentDirtyListEntry;

class FilamentDirtyList {
public:
	FilamentDirtyList();
	~FilamentDirtyList();

	FilamentDirtyList(const FilamentDirtyList &other) = delete;
	FilamentDirtyList &operator =(const FilamentDirtyList &other) = delete;

	inline bool isDirty() const {
		return !m_list.isEmpty();
	}

	void clean();

	void add(FilamentDirtyListEntry *entry) noexcept;

private:
	FilamentLinkedListEntry m_list;
};

#endif

