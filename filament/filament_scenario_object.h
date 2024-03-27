#ifndef FILAMENT_SCENARIO_OBJECT_H
#define FILAMENT_SCENARIO_OBJECT_H

#include "filament_object.h"

#include "filament/filament_engine_object.h"

namespace filament {
	class Scene;
}

class FilamentScenarioObject final : public FilamentObject {
public:
	FilamentScenarioObject();
	~FilamentScenarioObject() override;

	inline filament::Scene *scene() const {
		return m_scene.get();
	}

private:
	FilamentEngineObject<filament::Scene> m_scene;
};

#endif
