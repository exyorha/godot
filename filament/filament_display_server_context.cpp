#include "filament_display_server_context.h"
#include "filament_renderer_compositor.h"

#include <filament/Engine.h>

FilamentDisplayServerContext::FilamentDisplayServerContext() : m_engine(nullptr) {

}

FilamentDisplayServerContext::~FilamentDisplayServerContext() {
	FilamentRendererCompositor::clear_default();

	filament::Engine::destroy(&m_engine);
}

Error FilamentDisplayServerContext::initialize() {

	m_engine = filament::Engine::create();

	if(!m_engine) {
		return ERR_UNAVAILABLE;
	}

	FilamentRendererCompositor::set_default();

	return OK;
}
