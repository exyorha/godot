#include "filament/filament_canvas.h"

#include "filament/filament_canvas_item_container.h"
#include "filament/filament_scenario_object.h"
#include "filament/filament_canvas_item.h"


FilamentCanvas::FilamentCanvas() {
	m_backingScene = std::make_shared<FilamentScenarioObject>();
}

FilamentCanvas::~FilamentCanvas() = default;

std::shared_ptr<FilamentCanvas> FilamentCanvas::getCanvas(const std::shared_ptr<FilamentCanvasItemContainer> &parent) {
	auto currentParent = parent;

	while(currentParent) {
		auto canvas = std::dynamic_pointer_cast<FilamentCanvas>(currentParent);
		if(canvas) {
			return canvas;
		}

		std::shared_ptr<FilamentCanvasItemContainer> nextParent;

		auto thisItem = std::dynamic_pointer_cast<FilamentCanvasItem>(currentParent);
		if(thisItem) {
			nextParent = thisItem->parent();
		}

		currentParent = std::move(nextParent);
	}

	return nullptr;
}

bool FilamentCanvas::isEffectivelyVisible() const {
	return true;
}
