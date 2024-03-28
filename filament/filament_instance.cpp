#include "filament/filament_instance.h"
#include "filament/filament_scenario_object.h"
#include "filament/filament_rendering_server_backend.h"
#include "filament/filament_renderable_base.h"
#include "filament/filament_skeleton_object.h"

#include <utils/EntityManager.h>

#include <filament/Scene.h>
#include <filament/Engine.h>
#include <filament/RenderableManager.h>
#include <filament/LightManager.h>
#include <filament/SkinningBuffer.h>

FilamentInstance::FilamentInstance() : m_associatedBase(this), m_skeleton(this), m_associatedScene(nullptr), m_visible(true) {

}

FilamentInstance::~FilamentInstance() = default;

void FilamentInstance::setBase(const std::shared_ptr<FilamentRenderableBase> &base) {
	if(m_associatedBase == base) {
		return;
	}

	purgeInstance();

	m_associatedBase = base;
}

void FilamentInstance::purgeInstance() {
	if(isDirty()) {
		return;
	}

	objectAboutToInvalidate();

	auto &renderableManager = FilamentRenderingServerBackend::filamentEngine()->getRenderableManager();
	if(renderableManager.hasComponent(entity())) {
		renderableManager.destroy(entity());
	}
	auto &lightManager = FilamentRenderingServerBackend::filamentEngine()->getLightManager();
	if(lightManager.hasComponent(entity())) {
		lightManager.destroy(entity());
	}

	markDirty();
}

void FilamentInstance::doClean() {
	if(m_associatedBase && m_visible) {
		filament::SkinningBuffer *skinning = nullptr;
		if(m_skeleton) {
			skinning = m_skeleton->buffer();
		}

		m_associatedBase->constructInstance(entity(), skinning);
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
		newScene->addEntity(entity());
		m_associatedScene = newScene;
	}
}

void FilamentInstance::setVisible(bool visible) {
	if(m_visible != visible) {
		m_visible = visible;

		purgeInstance();
	}
}

void FilamentInstance::controlledObjectAboutToInvalidate(FilamentControlledObjectReferenceBase *linkedViaReference) {
	FilamentEntityObject::controlledObjectAboutToInvalidate(linkedViaReference);

	if(linkedViaReference == &m_associatedBase || linkedViaReference == &m_skeleton) {
		purgeInstance();
	}
}

void FilamentInstance::setSkeleton(const std::shared_ptr<FilamentSkeletonObject> &skeleton) {
	if(m_skeleton != skeleton) {

		purgeInstance();

		m_skeleton = skeleton;
	}
}
