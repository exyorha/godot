#include "filament/filament_viewport_object.h"
#include "filament/filament_rendering_server_backend.h"
#include "filament/filament_window.h"
#include "filament/filament_scenario_object.h"
#include "filament/filament_camera.h"
#include "filament/filament_canvas_view.h"

#include <filament/Engine.h>
#include <filament/Renderer.h>
#include <filament/View.h>
#include <filament/Viewport.h>

FilamentViewportObject::FilamentViewportObject() :
	m_view(FilamentRenderingServerBackend::filamentEngine()->createView()),
	m_registeredWithWindow(nullptr) {

	if(!m_view) {
		throw std::bad_alloc();
	}

	m_view->setPostProcessingEnabled(false);
	m_view->setFrontFaceWindingInverted(true);
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

	// TODO: sorting!
	auto viewport = m_view->getViewport();
	for(const auto &viewPtr: m_attachedCanvases) {
		viewPtr->render(renderer, viewport);
	}

	for(const auto &childPtr: m_childViewports) {
		auto child = childPtr.lock();

		if(child) {
			child->m_view->setViewport(viewport);
			child->render(renderer);
		}
	}
}

void FilamentViewportObject::setScenario(const std::shared_ptr<FilamentScenarioObject> &scenario) {
	filament::Scene *scene = nullptr;
	if(scenario) {
		scene = scenario->scene();
	}

	m_view->setScene(scene);
}

void FilamentViewportObject::setCamera(const std::shared_ptr<FilamentCamera> &cameraObject) {
	m_camera = cameraObject;
}

void FilamentViewportObject::attachCanvas(const std::shared_ptr<FilamentCanvas> &canvas) {
	if(!canvas) {
		return;
	}

	auto existing = getViewOfCanvas(canvas);
	if(existing) {
		return;
	}

	m_attachedCanvases.emplace_back(std::make_unique<FilamentCanvasView>(canvas));
}

void FilamentViewportObject::removeCanvas(const std::shared_ptr<FilamentCanvas> &canvas) {
	if(!canvas) {
		return;
	}

	auto end = std::remove_if(m_attachedCanvases.begin(), m_attachedCanvases.end(), [&canvas](const auto &view) { return view->canvas() == canvas; });
	m_attachedCanvases.erase(end, m_attachedCanvases.end());
}

FilamentCanvasView *FilamentViewportObject::getViewOfCanvas(const std::shared_ptr<FilamentCanvas> &canvas) {
	if(!canvas) {
		return nullptr;
	}

	for(const auto &viewPtr:m_attachedCanvases) {
		if(viewPtr->canvas() == canvas) {
			return viewPtr.get();
		}
	}

	return nullptr;
}

void FilamentViewportObject::setParentViewport(const std::shared_ptr<FilamentViewportObject> &parent) {
	if(m_parentViewport != parent) {
		auto sharedThis = std::static_pointer_cast<FilamentViewportObject>(shared_from_this());

		if(m_parentViewport) {
			m_parentViewport->removeChild(sharedThis);
			m_parentViewport.reset();
		}

		m_parentViewport = parent;

		if(m_parentViewport) {
			m_parentViewport->addChild(sharedThis);
		}
	}
}

void FilamentViewportObject::addChild(const std::shared_ptr<FilamentViewportObject> &child) {
	if(!child) {
		return;
	}

	for(const auto &pointer: m_childViewports) {
		if(pointer.lock() == child) {
			return;
		}
	}

	m_childViewports.emplace_back(child);
}


void FilamentViewportObject::removeChild(const std::shared_ptr<FilamentViewportObject> &child) {
	if(!child) {
		return;
	}

	auto end = std::remove_if(m_childViewports.begin(), m_childViewports.end(),
							  [&child](const std::weak_ptr<FilamentViewportObject> &item) {
								  auto locked = item.lock();
								  return !locked || locked == child;
							  });

	m_childViewports.erase(end, m_childViewports.end());
}
