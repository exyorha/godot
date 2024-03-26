#ifndef FILAMENT_OBJECT_MANAGER_H
#define FILAMENT_OBJECT_MANAGER_H

#include <atomic>
#include <cstdint>

#include <unordered_map>
#include <memory>

#include <core/templates/rid.h>

#include "filament/filament_object.h"

class RID;

class FilamentObjectManager {
public:
	FilamentObjectManager();
	~FilamentObjectManager();

	FilamentObjectManager(const FilamentObjectManager &other) = delete;
	FilamentObjectManager &operator =(const FilamentObjectManager &other) = delete;

	void associate(RID rid, const std::shared_ptr<FilamentObject> &object);

	void free(RID rid);

	static RID allocate();

	template<typename T>
	auto resolve(RID rid) -> typename std::enable_if<std::is_base_of_v<FilamentObject, T>, std::shared_ptr<T>>::type;

	template<>
	std::shared_ptr<FilamentObject> resolve<FilamentObject>(RID rid);

private:
	static void badRIDCast(RID rid, const std::type_info &requiredType, const std::type_info &passedType);

	static std::atomic<uint64_t> m_lastObjectId;
	std::unordered_map<uint64_t, std::shared_ptr<FilamentObject>> m_objectTable;
};

template<typename T>
auto FilamentObjectManager::resolve(RID rid) -> typename std::enable_if<std::is_base_of_v<FilamentObject, T>, std::shared_ptr<T>>::type {
	auto base = resolve<FilamentObject>(rid);
	if(!base) {
		return nullptr;
	}

	auto casted = std::dynamic_pointer_cast<T>(base);
	if(!casted) {
		const auto &object = *base;
		badRIDCast(rid, typeid(T), typeid(object));
	}

	return casted;
}

#endif
