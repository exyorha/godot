#include "filament/filament_canvas_item_container.h"

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
