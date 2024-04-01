#include "filament/filament_canvas_item_container.h"
#include "filament/filament_canvas_item.h"

#include <algorithm>

FilamentCanvasItemContainer::FilamentCanvasItemContainer() = default;

FilamentCanvasItemContainer::~FilamentCanvasItemContainer() = default;

void FilamentCanvasItemContainer::addChild(const std::shared_ptr<FilamentCanvasItem> &item) {
	m_children.emplace_back(item);
}

void FilamentCanvasItemContainer::removeChild(const std::shared_ptr<FilamentCanvasItem> &item) {
	auto end = std::remove_if(m_children.begin(), m_children.end(), [&item](const auto &ptr) {
		auto locked = ptr.lock();

		return !locked || locked == item;
	});

	m_children.erase(end, m_children.end());
}

void FilamentCanvasItemContainer::collectItems(FilamentCanvasRenderOrderCollector &collector, int32_t parentZOrder) {
	auto thisItemZOrder = calculateZOrder(parentZOrder);

	for(const auto &childWeak: m_children) {
		auto child = childWeak.lock();

		if(child && child->drawBehindParent()) {
			child->collectItems(collector, thisItemZOrder);
		}
	}

	collectSelf(collector, thisItemZOrder);

	for(const auto &childWeak: m_children) {
		auto child = childWeak.lock();

		if(child && !child->drawBehindParent()) {
			child->collectItems(collector, thisItemZOrder);
		}
	}
}

int32_t FilamentCanvasItemContainer::calculateZOrder(int32_t parentZOrder) const {
	return parentZOrder;
}

void FilamentCanvasItemContainer::collectSelf(FilamentCanvasRenderOrderCollector &collector, int32_t calculatedZOrder) {
	(void)collector;
	(void)calculatedZOrder;
}
