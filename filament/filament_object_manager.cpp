#include "filament_object_manager.h"
#include "core/templates/rid.h"

#include <cstdio>

std::atomic<uint64_t> FilamentObjectManager::m_lastObjectId = 0;

FilamentObjectManager::FilamentObjectManager() = default;

FilamentObjectManager::~FilamentObjectManager() {
	clear();
}

void FilamentObjectManager::clear() {
	if(!m_objectTable.empty()) {
		fprintf(stderr, "FilamentObjectManager: %zu FilamentServer RIDs are still alive at the shutdown", m_objectTable.size());
	}

	m_objectTable.clear();
}

RID FilamentObjectManager::allocate() {
	auto lastID = m_lastObjectId.fetch_add(1);

	return RID::from_uint64(lastID + 1);
}

void FilamentObjectManager::free(RID rid) {
	if(rid.is_null()) {
		return;
	}

	m_objectTable.erase(rid.get_id());
}
