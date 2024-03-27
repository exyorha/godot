#include "filament_window.h"
#include "filament_rendering_server.h"
#include "filament/filament_viewport_object.h"

#include <filament/Engine.h>
#include <filament/Renderer.h>

#include <algorithm>

FilamentWindow::FilamentWindow(void *nativeWindow) :
	m_swapchain(FilamentRenderingServer::filamentEngine()->createSwapChain(nativeWindow)),
	m_renderer(FilamentRenderingServer::filamentEngine()->createRenderer()) {

	if(!m_swapchain) {
		throw std::runtime_error("failed to create the Filament SwapChain");
	}

	filament::Renderer::ClearOptions options;
	options.clear = true;
	options.discard = false;
	options.clearColor = filament::math::float4(0.0f, 0.0f, 0.0f, 1.0f);

	m_renderer->setClearOptions(options);

}

FilamentWindow::~FilamentWindow() {
	for(auto viewport: m_viewports) {
		viewport->dissociateFromDestroyedWindow(this);
	}
}

void FilamentWindow::renderWindow() {
	if(!m_renderer->beginFrame(m_swapchain.get())) {
	   return;
	}

	for(auto viewport: m_viewports) {
		viewport->render(m_renderer.get());
	}

	m_renderer->endFrame();
}

void FilamentWindow::registerViewport(FilamentViewportObject *viewport) {
	m_viewports.emplace_back(viewport);
}

void FilamentWindow::unregisterViewport(FilamentViewportObject *viewport) noexcept {
	m_viewports.erase(std::remove(m_viewports.begin(), m_viewports.end(), viewport), m_viewports.end());
}
