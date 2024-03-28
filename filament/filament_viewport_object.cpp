#include "filament/filament_viewport_object.h"
#include "filament/filament_rendering_server_backend.h"
#include "filament/filament_window.h"
#include "filament/filament_scenario_object.h"
#include "filament/filament_camera.h"

#include <filament/Engine.h>
#include <filament/Renderer.h>
#include <filament/View.h>
#include <filament/Viewport.h>

FilamentViewportObject::FilamentViewportObject() : m_view(FilamentRenderingServerBackend::filamentEngine()->createView()), m_registeredWithWindow(nullptr) {
	if(!m_view) {
		throw std::bad_alloc();
	}

	m_view->setPostProcessingEnabled(false);
}

FilamentViewportObject::~FilamentViewportObject() {
	setWindow(nullptr, Rect2());
}

void FilamentViewportObject::setWindow(FilamentWindow *window, const Rect2 & p_rect) {
	if(m_registeredWithWindow) {
		m_registeredWithWindow->unregisterViewport(this);
		m_registeredWithWindow = nullptr;
	}

	if(window) {
		window->registerViewport(this);
		m_registeredWithWindow = window;
	}

	m_view->setViewport(filament::Viewport(
		// TODO: this is left, bottom; top left, top; verify the coordinate system
		p_rect.position.x, p_rect.position.y, p_rect.size.width, p_rect.size.height
	));
}

void FilamentViewportObject::dissociateFromDestroyedWindow(FilamentWindow *window) noexcept {
	if(m_registeredWithWindow == window) {
		m_registeredWithWindow = nullptr;
	}
}

void FilamentViewportObject::render(filament::Renderer *renderer) {
	auto cameraObject = m_camera.lock();

	filament::Camera *camera = nullptr;

	if(cameraObject) {
		cameraObject->configureForViewport(m_view->getViewport());
		camera = cameraObject->camera();
	}

	m_view->setCamera(camera);

	if(camera) {

		renderer->render(m_view.get());
	}
}

void FilamentViewportObject::setScenario(const std::shared_ptr<FilamentScenarioObject> &scenario) {
	filament::Scene *scene = nullptr;
	if(scenario) {
		scene = scenario->scene();
	}

	if(scene) {
		printf("!!!! SCENE SET ON VIEW\n");
	}

	m_view->setScene(scene);
}

void FilamentViewportObject::setCamera(const std::shared_ptr<FilamentCamera> &cameraObject) {
	m_camera = cameraObject;
}

