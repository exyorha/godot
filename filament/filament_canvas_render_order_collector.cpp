#include "filament/filament_canvas_render_order_collector.h"
#include "filament/filament_canvas_item.h"

#include <algorithm>

FilamentCanvasRenderOrderCollector::FilamentCanvasRenderOrderCollector() = default;

FilamentCanvasRenderOrderCollector::~FilamentCanvasRenderOrderCollector() = default;

void FilamentCanvasRenderOrderCollector::collectItem(const std::shared_ptr<FilamentCanvasItem> &item, int32_t zOrder) {
	auto index = m_items.size();

	m_items.emplace_back(Item{
		.item = item,
		.inOrderDrawingIndex = index,
		.zOrder = zOrder
	});
}

void FilamentCanvasRenderOrderCollector::finalize() {

	std::sort(m_items.begin(), m_items.end(), [](const Item &a, const Item &b) -> bool {
		if(a.zOrder == b.zOrder) {
			return a.inOrderDrawingIndex < b.inOrderDrawingIndex;
		} else {
			return a.zOrder < b.zOrder;
		}
	});

	for(size_t drawingIndex = 0, count = m_items.size(); drawingIndex < count; drawingIndex++) {
		m_items[drawingIndex].item->setBlendOrder(std::min<size_t>(drawingIndex, 32767));
	}
}
