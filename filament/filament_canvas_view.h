#ifndef FILAMENT_CANVAS_VIEW_H
#define FILAMENT_CANVAS_VIEW_H

#include <memory>

#include <filament/filament_engine_object.h>

class FilamentCanvas;
class FilamentCamera;

namespace filament {
	class View;
	class Renderer;
	class Viewport;
	class RenderTarget;
}

class FilamentCanvasView {
public:
	explicit FilamentCanvasView(const std::shared_ptr<FilamentCanvas> &canvas);
	~FilamentCanvasView();

	FilamentCanvasView(const FilamentCanvasView &other) = delete;
	FilamentCanvasView &operator =(const FilamentCanvasView &other) = delete;

	inline const std::shared_ptr<FilamentCanvas> &canvas() const {
		return m_canvas;
	}

	void render(filament::Renderer *renderer, filament::RenderTarget *renderTarget, const filament::Viewport &viewport, bool standalone, bool yFlip);

private:
	std::shared_ptr<FilamentCanvas> m_canvas;
	std::shared_ptr<FilamentCamera> m_camera;
	FilamentEngineObject<filament::View> m_view;
};

#endif

