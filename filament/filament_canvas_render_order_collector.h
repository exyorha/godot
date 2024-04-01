#ifndef FILAMENT_CANVAS_RENDER_ORDER_COLLECTOR_H
#define FILAMENT_CANVAS_RENDER_ORDER_COLLECTOR_H

#include <cstdint>
#include <memory>
#include <vector>

class FilamentCanvasItem;

class FilamentCanvasRenderOrderCollector {
public:
	FilamentCanvasRenderOrderCollector();
	~FilamentCanvasRenderOrderCollector();

	FilamentCanvasRenderOrderCollector(const FilamentCanvasRenderOrderCollector &other) = delete;
	FilamentCanvasRenderOrderCollector &operator =(const FilamentCanvasRenderOrderCollector &other) = delete;

	size_t collectItem(const std::shared_ptr<FilamentCanvasItem> &item, int32_t zOrder);

	void finalize();

	void setItemParent(size_t itemId, size_t parentId);

private:
	struct Item {
		std::shared_ptr<FilamentCanvasItem> item;
		size_t inOrderDrawingIndex;
		size_t parentId;
		int32_t zOrder;
	};

	std::vector<Item> m_items;
};

#endif
