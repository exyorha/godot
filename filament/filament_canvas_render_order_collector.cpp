#include "filament/filament_canvas_render_order_collector.h"
#include "filament/filament_canvas_item.h"

#include <algorithm>

FilamentCanvasRenderOrderCollector::FilamentCanvasRenderOrderCollector() = default;

FilamentCanvasRenderOrderCollector::~FilamentCanvasRenderOrderCollector() = default;

size_t FilamentCanvasRenderOrderCollector::collectItem(const std::shared_ptr<FilamentCanvasItem> &item, int32_t zOrder) {
	auto index = m_items.size();

	m_items.emplace_back(Item{
		.item = item,
		.inOrderDrawingIndex = index,
		.parentId = std::numeric_limits<size_t>::max(),
		.zOrder = zOrder
	});

	return index;
}

void FilamentCanvasRenderOrderCollector::setItemParent(size_t itemId, size_t parentId) {
	m_items.at(itemId).parentId = parentId;
}

void FilamentCanvasRenderOrderCollector::finalize() {

	std::sort(m_items.begin(), m_items.end(), [](const Item &a, const Item &b) -> bool {
		if(a.zOrder == b.zOrder) {
			return a.inOrderDrawingIndex > b.inOrderDrawingIndex;
		} else {
			return a.zOrder > b.zOrder;
		}
	});

	std::vector<size_t> itemBlendOrders;
	itemBlendOrders.resize(m_items.size());
	for(size_t index = 0, count = m_items.size(); index < count; index++) {
		itemBlendOrders[m_items[index].inOrderDrawingIndex] = index;
	}

	/*
	 * TODO: blend order value needs to be relative to the parent
	 */

	for(size_t drawingIndex = 0, count = m_items.size(); drawingIndex < count; drawingIndex++) {
		const auto &data = m_items[drawingIndex];

		int32_t drawingOrder = static_cast<int32_t>(drawingIndex);
		if(data.parentId != std::numeric_limits<size_t>::max()) {
			drawingOrder -= itemBlendOrders.at(data.parentId);
		}

		data.item->setBlendOrder(drawingOrder * 1.0f / static_cast<int32_t>(count));
	}
}
