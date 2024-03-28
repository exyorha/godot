#include "filament_dirty_list.h"
#include "filament_dirty_list_entry.h"


FilamentDirtyList::FilamentDirtyList() = default;

FilamentDirtyList::~FilamentDirtyList() = default;

void FilamentDirtyList::clean() {
	while(!m_list.isEmpty()) {
		static_cast<FilamentDirtyListEntry *>(m_list.next())->clean();
	}
}

void FilamentDirtyList::add(FilamentDirtyListEntry *entry) noexcept {
	m_list.insertTail(entry);
}
