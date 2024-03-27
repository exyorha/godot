#ifndef FILAMENT_CAMERA_H
#define FILAMENT_CAMERA_H

#include "filament_entity_object.h"

#include <utils/Entity.h>

namespace filament {
	class Camera;
}

class FilamentCamera final : public FilamentEntityObject {
public:
	FilamentCamera();
	~FilamentCamera() override;

	inline filament::Camera *camera() const {
		return m_camera;
	}

private:
	filament::Camera *m_camera;
};

#endif
