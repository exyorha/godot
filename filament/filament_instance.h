#ifndef FILAMENT_INSTANCE_H
#define FILAMENT_INSTANCE_H

#include "filament/filament_entity_object.h"

namespace filament {
	class Scene;
}

class FilamentRenderableBase;
class FilamentScenarioObject;
class FilamentSkeletonObject;

class FilamentInstance final : public FilamentEntityObject {
public:
	FilamentInstance();
	~FilamentInstance();

	void setBase(const std::shared_ptr<FilamentRenderableBase> &base);

	void setSkeleton(const std::shared_ptr<FilamentSkeletonObject> &skeleton);

	void setScenario(const std::shared_ptr<FilamentScenarioObject> &scenario);

	inline bool isVisible() const {
		return m_visible;
	}

	void setVisible(bool visible);

protected:
	void doClean() override;
	void controlledObjectAboutToInvalidate(FilamentControlledObjectReferenceBase *linkedViaReference) override;

private:
	void purgeInstance();

	FilamentControlledObjectReference<FilamentRenderableBase> m_associatedBase;
	FilamentControlledObjectReference<FilamentSkeletonObject> m_skeleton;
	filament::Scene *m_associatedScene;
	bool m_visible;
};

#endif

