#ifndef FILAMENT_VIEWPORT_OBJECT_H
#define FILAMENT_VIEWPORT_OBJECT_H

#include "filament/filament_object.h"
#include "filament/filament_engine_object.h"

namespace filament {
	class View;
	class Renderer;
}

class FilamentWindow;
class FilamentScenarioObject;
class FilamentCamera;

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

private:
	FilamentEngineObject<filament::View> m_view;
	FilamentWindow *m_registeredWithWindow;
	std::weak_ptr<FilamentCamera> m_camera;
};

#endif
