#ifndef FILAMENT_OBJECT_H
#define FILAMENT_OBJECT_H

#include <memory>

class FilamentObject : public std::enable_shared_from_this<FilamentObject> {
protected:
	FilamentObject();

public:
	virtual ~FilamentObject();

	FilamentObject(const FilamentObject &other) = delete;
	FilamentObject &operator =(const FilamentObject &other) = delete;
};

#endif
