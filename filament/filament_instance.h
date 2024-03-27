#ifndef FILAMENT_INSTANCE_H
#define FILAMENT_INSTANCE_H

#include "filament/filament_entity_object.h"

namespace filament {
	class Scene;
}

class FilamentRenderableBase;
class FilamentScenarioObject;

class FilamentInstance final : public FilamentEntityObject {
public:
	FilamentInstance();
	~FilamentInstance();

	void setBase(const std::shared_ptr<FilamentRenderableBase> &base);

	void setScenario(const std::shared_ptr<FilamentScenarioObject> &scenario);

	inline bool isVisible() const {
		return m_visible;
	}

	void setVisible(bool visible);

private:
	void applyVisible();

	std::weak_ptr<FilamentRenderableBase> m_associatedBase;
	filament::Scene *m_associatedScene;
	bool m_visible;
};

#endif

