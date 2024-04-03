#include "filament/filament_canvas.h"

#include "filament/filament_canvas_item_container.h"
#include "filament/filament_scenario_object.h"
#include "filament/filament_canvas_item.h"
#include "filament/filament_canvas_render_order_collector.h"

#include "core/math/rect2i.h"

FilamentCanvas::FilamentCanvas() : m_yFlip(false) {
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

void FilamentCanvas::doClean() {
	FilamentCanvasRenderOrderCollector renderOrderCollector;

	int32_t parentZOrder = 0;
	collectItems(renderOrderCollector, parentZOrder);

	renderOrderCollector.finalize();
}

std::optional<Rect2i> FilamentCanvas::calculateClipRectangle(const std::optional<Rect2i> &childRect) const {
	return childRect;
}

void FilamentCanvas::setDrawingViewport(const Size2i &windowSize, const filament::Viewport &viewport, bool yFlip) {
	if(!m_windowSize.has_value() || windowSize != *m_windowSize || !m_viewport.has_value() || viewport != *m_viewport || yFlip != m_yFlip) {
		m_windowSize.emplace(windowSize);
		m_viewport.emplace(viewport);
		m_yFlip = yFlip;

		updateClipping();
	}
}

Rect2i FilamentCanvas::clipRectangleToScissorRectangle(const Rect2i &clip) const {
	if(m_yFlip) {
		return clip;
	}

	if(!m_windowSize.has_value() || !m_viewport.has_value()) {
		return Rect2i();
	}

	return Rect2i(clip.position.x + m_viewport->left, m_windowSize->height - (clip.position.y + m_viewport->bottom + clip.size.height),
				  clip.size.width, clip.size.height);
}

