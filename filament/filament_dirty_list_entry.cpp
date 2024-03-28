#include "filament_dirty_list_entry.h"
#include "filament_rendering_server_backend.h"

FilamentDirtyListEntry::FilamentDirtyListEntry() = default;

FilamentDirtyListEntry::~FilamentDirtyListEntry() = default;

FilamentDirtyListEntry::FilamentDirtyListEntry(FilamentDirtyListEntry &&other) noexcept = default;

FilamentDirtyListEntry &FilamentDirtyListEntry::operator =(FilamentDirtyListEntry &&other) noexcept = default;

void FilamentDirtyListEntry::clean() {
	if(!isDirty()) {
		return;
	}

	remove();

	doClean();
}

void FilamentDirtyListEntry::doClean() {

}

void FilamentDirtyListEntry::markDirty() noexcept {
	if(!isDirty()) {
		FilamentRenderingServerBackend::get()->markDirty(this);
	}
}
