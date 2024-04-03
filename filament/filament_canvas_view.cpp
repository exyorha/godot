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

void FilamentCanvasView::render(filament::Renderer *renderer, filament::RenderTarget *renderTarget, const filament::Viewport &viewport,
								bool standalone, bool yFlip) {
	double bottom = viewport.height;
	double top = 0;

	if(yFlip) {
		/*
		 * When rendering into a render target, we want to flip the output
		 * image, so it'll match the sampling orientation.
		 */
		std::swap(bottom, top);
	}

	m_camera->camera()->setProjection(filament::Camera::Projection::ORTHO,
									  viewport.left, viewport.right(), bottom, top,
									  -1.0, 1.0);

	m_view->setViewport(viewport);
	m_view->setRenderTarget(renderTarget);

	if(standalone) {
		renderer->renderStandaloneView(m_view.get());

		renderer->setClearOptions(filament::Renderer::ClearOptions{
			.clear = false,
			.discard = false
		});
	} else {
		renderer->render(m_view.get());
	}
}
