#include "filament/filament_canvas_item_container.h"
#include "filament/filament_canvas_item.h"
#include "filament/filament_canvas_render_order_collector.h"

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

std::optional<size_t> FilamentCanvasItemContainer::collectItems(FilamentCanvasRenderOrderCollector &collector, int32_t parentZOrder) {
	auto thisItemZOrder = calculateZOrder(parentZOrder);

	std::vector<size_t> earlyChildren;
	for(const auto &childWeak: m_children) {
		auto child = childWeak.lock();

		if(child && child->drawBehindParent()) {
			auto childId = child->collectItems(collector, thisItemZOrder);
			if(childId.has_value()) {
				earlyChildren.push_back(*childId);
			}
		}
	}

	auto selfId = collectSelf(collector, thisItemZOrder);
	if(selfId.has_value()) {
		for(auto earlyChild: earlyChildren) {
			collector.setItemParent(earlyChild, *selfId);
		}
	}

	for(const auto &childWeak: m_children) {
		auto child = childWeak.lock();

		if(child && !child->drawBehindParent()) {
			auto childId = child->collectItems(collector, thisItemZOrder);
			if(childId.has_value() && selfId.has_value()) {
				collector.setItemParent(*childId, *selfId);
			}
		}
	}

	return selfId;
}

int32_t FilamentCanvasItemContainer::calculateZOrder(int32_t parentZOrder) const {
	return parentZOrder;
}

std::optional<size_t> FilamentCanvasItemContainer::collectSelf(FilamentCanvasRenderOrderCollector &collector, int32_t calculatedZOrder) {
	(void)collector;
	(void)calculatedZOrder;
	return std::nullopt;
}
