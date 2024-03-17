#include "filament_window.h"
#include "filament_rendering_server.h"

#include <filament/Engine.h>
#include <filament/Renderer.h>

FilamentWindow::FilamentWindow(void *nativeWindow) :
	m_swapchain(FilamentRenderingServer::filamentEngine()->createSwapChain(nativeWindow)),
	m_renderer(FilamentRenderingServer::filamentEngine()->createRenderer()) {

	if(!m_swapchain) {
		throw std::runtime_error("failed to create the Filament SwapChain");
	}
}

FilamentWindow::~FilamentWindow() = default;

void FilamentWindow::renderWindow() {
	if(!m_renderer->beginFrame(m_swapchain.get())) {
	   return;
	}

	m_renderer->endFrame();
}

