#include "filament/filament_instance.h"
#include "filament/filament_scenario_object.h"
#include "filament/filament_rendering_server_backend.h"
#include "filament/filament_renderable_base.h"

#include <utils/EntityManager.h>

#include <filament/Scene.h>
#include <filament/Engine.h>
#include <filament/RenderableManager.h>
#include <filament/LightManager.h>

FilamentInstance::FilamentInstance() : m_associatedScene(nullptr), m_visible(true) {

}

FilamentInstance::~FilamentInstance() {
	setBase(nullptr);
	setScenario(nullptr);
}


void FilamentInstance::setBase(const std::shared_ptr<FilamentRenderableBase> &base) {
	if(m_associatedBase.lock() == base) {
		return;
	}

	auto &renderableManager = FilamentRenderingServerBackend::filamentEngine()->getRenderableManager();
	if(renderableManager.hasComponent(entity())) {
		renderableManager.destroy(entity());
	}
	auto &lightManager = FilamentRenderingServerBackend::filamentEngine()->getLightManager();
	if(lightManager.hasComponent(entity())) {
		lightManager.destroy(entity());
	}

	m_associatedBase.reset();

	m_associatedBase = base;

	if(base) {
		base->constructInstance(entity());

		applyVisible();
	}
}

void FilamentInstance::setScenario(const std::shared_ptr<FilamentScenarioObject> &scenario) {
	filament::Scene *newScene = nullptr;
	if(scenario) {
		newScene = scenario->scene();
	}

	if(m_associatedScene) {
		m_associatedScene->remove(entity());
		m_associatedScene = nullptr;
	}

	if(newScene) {
		printf("!!! ADDING INSTANCE TO SCENARIO\n");
		newScene->addEntity(entity());
		m_associatedScene = newScene;
	}
}

void FilamentInstance::setVisible(bool visible) {
	if(m_visible != visible) {
		m_visible = visible;

		applyVisible();
	}
}

void FilamentInstance::applyVisible() {
	auto &renderableManager = FilamentRenderingServerBackend::filamentEngine()->getRenderableManager();
	auto renderableInstance = renderableManager.getInstance(entity());
	if(renderableInstance) {
		if(m_visible) {
			renderableManager.setLayerMask(renderableInstance, 0xFF, 0x01);
		} else {
			renderableManager.setLayerMask(renderableInstance, 0xFF, 0x00);
		}
	}

	auto &lightManager = FilamentRenderingServerBackend::filamentEngine()->getLightManager();
	auto lightInstance = lightManager.getInstance(entity());
	if(lightInstance) {
		lightManager.setLightChannel(lightInstance, 0, m_visible);
	}
}
