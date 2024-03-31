#ifndef FILAMENT_VIEWPORT_OBJECT_H
#define FILAMENT_VIEWPORT_OBJECT_H

#include "filament/filament_object.h"
#include "filament/filament_engine_object.h"

#include <vector>

namespace filament {
	class View;
	class Renderer;
}

class FilamentWindow;
class FilamentScenarioObject;
class FilamentCamera;
class FilamentCanvasView;
class FilamentCanvas;

struct Rect2;

class FilamentViewportObject final : public FilamentObject {
public:
	FilamentViewportObject();
	~FilamentViewportObject() override;

	void setWindow(FilamentWindow *window, const Rect2 & p_rect);

	void dissociateFromDestroyedWindow(FilamentWindow *window) noexcept;

	void render(filament::Renderer *renderer);

	void setScenario(const std::shared_ptr<FilamentScenarioObject> &scenario);

	void setCamera(const std::shared_ptr<FilamentCamera> &camera);

	void attachCanvas(const std::shared_ptr<FilamentCanvas> &canvas);
	void removeCanvas(const std::shared_ptr<FilamentCanvas> &canvas);

	void setParentViewport(const std::shared_ptr<FilamentViewportObject> &parent);

private:
	FilamentCanvasView *getViewOfCanvas(const std::shared_ptr<FilamentCanvas> &canvas);

	void addChild(const std::shared_ptr<FilamentViewportObject> &object);
	void removeChild(const std::shared_ptr<FilamentViewportObject> &object);

	FilamentEngineObject<filament::View> m_view;
	FilamentWindow *m_registeredWithWindow;
	std::weak_ptr<FilamentCamera> m_camera;
	std::vector<std::unique_ptr<FilamentCanvasView>> m_attachedCanvases;
	std::shared_ptr<FilamentViewportObject> m_parentViewport;
	std::vector<std::weak_ptr<FilamentViewportObject>> m_childViewports;
};

#endif
