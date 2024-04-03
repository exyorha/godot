#ifndef FILAMENT_CANVAS_H
#define FILAMENT_CANVAS_H

#include "filament/filament_canvas_item_container.h"
#include "filament/filament_object.h"

#include "core/math/vector2i.h"

#include <filament/Viewport.h>

class FilamentScenarioObject;

namespace filament {
	class Viewport;
}

class FilamentCanvas final : public FilamentObject, public FilamentCanvasItemContainer {
public:
	FilamentCanvas();
	~FilamentCanvas() override;

	static std::shared_ptr<FilamentCanvas> getCanvas(const std::shared_ptr<FilamentCanvasItemContainer> &parent);

	inline const std::shared_ptr<FilamentScenarioObject> &getBackingScene() const {
		return m_backingScene;
	}

	bool isEffectivelyVisible() const override;

	inline void renderingOrderChanged() {
		markDirty();
	}

	std::optional<Rect2i> calculateClipRectangle(const std::optional<Rect2i> &childRect) const override;

	void setDrawingViewport(const Size2i &windowSize, const filament::Viewport &viewport, bool yFlip);

	Rect2i clipRectangleToScissorRectangle(const Rect2i &clip) const;

protected:
	void doClean() override;

private:
	std::shared_ptr<FilamentScenarioObject> m_backingScene;
	std::optional<Size2i> m_windowSize;
	std::optional<filament::Viewport> m_viewport;
	bool m_yFlip;
};

#endif

