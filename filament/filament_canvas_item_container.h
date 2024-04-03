#ifndef FILAMENT_CANVAS_ITEM_CONTAINER_H
#define FILAMENT_CANVAS_ITEM_CONTAINER_H

#include <vector>
#include <memory>
#include <optional>

class FilamentCanvasItem;
class FilamentCanvasRenderOrderCollector;
struct Rect2i;

class FilamentCanvasItemContainer {
protected:
	FilamentCanvasItemContainer();
	~FilamentCanvasItemContainer();

	FilamentCanvasItemContainer(const FilamentCanvasItemContainer &other) = delete;
	FilamentCanvasItemContainer &operator =(const FilamentCanvasItemContainer &other) = delete;

public:

	void addChild(const std::shared_ptr<FilamentCanvasItem> &item);
	void removeChild(const std::shared_ptr<FilamentCanvasItem> &item);

	inline void childrenNeedSorting() {
		m_childrenNeedSorting = true;
	}

	std::optional<size_t> collectItems(FilamentCanvasRenderOrderCollector &collector, int32_t parentZOrder);

	virtual bool isEffectivelyVisible() const = 0;

	virtual std::optional<Rect2i> calculateClipRectangle(const std::optional<Rect2i> &childRectangle) const = 0;

	void updateClipping();

protected:
	inline const std::vector<std::weak_ptr<FilamentCanvasItem>> &children() const {
		return m_children;
	}

	virtual int32_t calculateZOrder(int32_t parentZOrder) const;
	virtual std::optional<size_t> collectSelf(FilamentCanvasRenderOrderCollector &collector, int32_t calculatedZOrder);

	virtual void updateSelfClipping();

private:
	std::vector<std::weak_ptr<FilamentCanvasItem>> m_children;
	bool m_childrenNeedSorting;
};

#endif
