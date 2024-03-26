#include "filament_object_manager.h"
#include "core/templates/rid.h"

#include <cstdio>
#include <stdexcept>
#include <cinttypes>

std::atomic<uint64_t> FilamentObjectManager::m_lastObjectId = 0;

FilamentObjectManager::FilamentObjectManager() = default;

FilamentObjectManager::~FilamentObjectManager() {
	if(!m_objectTable.empty()) {
		fprintf(stderr, "FilamentObjectManager: %zu FilamentServer RIDs are still alive at the shutdown\n", m_objectTable.size());
	}
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

void FilamentObjectManager::associate(RID rid, const std::shared_ptr<FilamentObject> &object) {
	if(rid.is_null()) {
		throw std::logic_error("FilamentObjectManager::associate called with a null RID");
	}

	if(!object) {
		throw std::logic_error("FilamentObjectManager::associate: attempted to associate a RID with a null pointer");
	}

	auto result = m_objectTable.emplace(rid.get_id(), object);
	if(!result.second) {
		result.first->second = object;
	}
}

template<>
std::shared_ptr<FilamentObject> FilamentObjectManager::resolve<FilamentObject>(RID rid) {
	if(rid.is_null()) {
		return nullptr;
	}

	auto it = m_objectTable.find(rid.get_id());
	if(it == m_objectTable.end()) {
		fprintf(stderr, "FilamentObjectManager::resolve: RID %" PRIu64 " doesn't have an object associated\n", rid.get_id());
		return nullptr;
	}

	return it->second;
}

void FilamentObjectManager::badRIDCast(RID rid, const std::type_info &requiredType, const std::type_info &passedType) {
	fprintf(stderr, "FilamentObjectManager::resolve: the passed RID %" PRIu64 " has an unexpected type: %s is required, but %s was passed\n",
			rid.get_id(), requiredType.name(), passedType.name());
}
