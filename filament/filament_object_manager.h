#ifndef FILAMENT_OBJECT_MANAGER_H
#define FILAMENT_OBJECT_MANAGER_H

#include <atomic>
#include <cstdint>

#include <unordered_map>
#include <memory>

class RID;
class FilamentObject;

class FilamentObjectManager {
public:
	FilamentObjectManager();
	~FilamentObjectManager();

	FilamentObjectManager(const FilamentObjectManager &other) = delete;
	FilamentObjectManager &operator =(const FilamentObjectManager &other) = delete;

	void free(RID rid);

	void clear();

	static RID allocate();

private:
	static std::atomic<uint64_t> m_lastObjectId;
	std::unordered_map<uint64_t, std::shared_ptr<FilamentObject>> m_objectTable;
};

#endif
