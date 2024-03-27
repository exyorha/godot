#include "filament/filament_scenario_object.h"
#include "filament/filament_rendering_server_backend.h"

#include <filament/Engine.h>

FilamentScenarioObject::FilamentScenarioObject() : m_scene(FilamentRenderingServerBackend::filamentEngine()->createScene()) {
	if(!m_scene) {
		throw std::bad_alloc();
	}
}

FilamentScenarioObject::~FilamentScenarioObject() = default;
