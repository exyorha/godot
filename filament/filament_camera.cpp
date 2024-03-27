#include "filament/filament_camera.h"
#include "filament/filament_rendering_server_backend.h"

#include <filament/Engine.h>
#include <filament/Camera.h>

FilamentCamera::FilamentCamera() : m_camera(FilamentRenderingServerBackend::filamentEngine()->createCamera(entity())) {
	if(!m_camera) {
		throw std::bad_alloc();
	}

	m_camera->setProjection(75.0f, 4.0/3.0, 0.1, 10000.0);
	m_camera->setExposure(1.0f);
}

FilamentCamera::~FilamentCamera() = default;
