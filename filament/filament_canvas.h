#ifndef FILAMENT_CANVAS_H
#define FILAMENT_CANVAS_H

#include "filament/filament_canvas_item_container.h"
#include "filament/filament_object.h"

class FilamentScenarioObject;

class FilamentCanvas final : public FilamentObject, public FilamentCanvasItemContainer {
public:
	FilamentCanvas();
	~FilamentCanvas() override;

	static std::shared_ptr<FilamentCanvas> getCanvas(const std::shared_ptr<FilamentCanvasItemContainer> &parent);

	inline const std::shared_ptr<FilamentScenarioObject> &getBackingScene() const {
		return m_backingScene;
	}

	bool isEffectivelyVisible() const override;

private:
	std::shared_ptr<FilamentScenarioObject> m_backingScene;
};

#endif

