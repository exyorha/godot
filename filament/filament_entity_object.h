#ifndef FILAMENT_FILAMENT_ENTITY_OBJECT_H
#define FILAMENT_FILAMENT_ENTITY_OBJECT_H

#include "filament/filament_object.h"

#include <utils/Entity.h>

struct Transform3D;

class FilamentEntityObject : public FilamentObject {
protected:
	FilamentEntityObject();

public:
	~FilamentEntityObject() override;

	inline const utils::Entity &entity() const {
		return m_entity;
	}

	void setTransform(const Transform3D &transform);

protected:
	void setParent(const std::shared_ptr<FilamentEntityObject> &parentEntity);

private:
	utils::Entity m_entity;
};

#endif
