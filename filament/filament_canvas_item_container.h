#ifndef FILAMENT_CANVAS_ITEM_CONTAINER_H
#define FILAMENT_CANVAS_ITEM_CONTAINER_H

#include <vector>
#include <memory>

class FilamentCanvasItem;

class FilamentCanvasItemContainer {
protected:
	FilamentCanvasItemContainer();
	~FilamentCanvasItemContainer();

	FilamentCanvasItemContainer(const FilamentCanvasItemContainer &other) = delete;
	FilamentCanvasItemContainer &operator =(const FilamentCanvasItemContainer &other) = delete;

public:

	void addChild(const std::shared_ptr<FilamentCanvasItem> &item);
	void removeChild(const std::shared_ptr<FilamentCanvasItem> &item);

	virtual bool isEffectivelyVisible() const = 0;

protected:
	inline const std::vector<std::weak_ptr<FilamentCanvasItem>> &children() const {
		return m_children;
	}

private:
	std::vector<std::weak_ptr<FilamentCanvasItem>> m_children;
};

#endif
