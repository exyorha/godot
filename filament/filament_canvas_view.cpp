#include "filament/filament_canvas_view.h"
#include "filament/filament_camera.h"
#include "filament/filament_rendering_server_backend.h"
#include "filament/filament_canvas.h"
#include "filament/filament_scenario_object.h"

#include <filament/Engine.h>
#include <filament/View.h>
#include <filament/Renderer.h>
#include <filament/Camera.h>

FilamentCanvasView::FilamentCanvasView(const std::shared_ptr<FilamentCanvas> &canvas) : m_canvas(canvas),
	m_camera(std::make_shared<FilamentCamera>()),
	m_view(FilamentRenderingServerBackend::filamentEngine()->createView()) {

	if(!m_view) {
		throw std::bad_alloc();
	}

	m_view->setPostProcessingEnabled(false);
	m_view->setScene(m_canvas->getBackingScene()->scene());
	m_view->setCamera(m_camera->camera());
    m_view->setShadowingEnabled(false);
}

FilamentCanvasView::~FilamentCanvasView() = default;

void FilamentCanvasView::render(filament::Renderer *renderer, const filament::Viewport &viewport) {
	m_camera->camera()->setProjection(filament::Camera::Projection::ORTHO,
									  viewport.left, viewport.right(), viewport.height, 0,
									  -100.0, 100.0);

	m_view->setViewport(viewport);
	renderer->render(m_view.get());
}
