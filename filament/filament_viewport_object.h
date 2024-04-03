#ifndef FILAMENT_VIEWPORT_OBJECT_H
#define FILAMENT_VIEWPORT_OBJECT_H

#include "filament/filament_linked_list_entry.h"
#include "filament/filament_texture_reference_object.h"
#include "filament/filament_engine_object.h"

#include "core/math/rect2.h"
#include "core/math/vector2i.h"

#include <vector>

namespace filament {
	class View;
	class Renderer;
	class RenderTarget;
	class Texture;
}

class FilamentWindow;
class FilamentScenarioObject;
class FilamentCamera;
class FilamentCanvasView;
class FilamentCanvas;

struct FilamentViewportListItem : public FilamentLinkedListEntry {

};

class FilamentViewportObject final : public FilamentTextureReferenceObject, public FilamentViewportListItem {
public:
	FilamentViewportObject();
	~FilamentViewportObject() override;

	void setWindow(FilamentWindow *window, const Rect2 & p_rect);

	void dissociateFromDestroyedWindow(FilamentWindow *window) noexcept;

	void render(filament::Renderer *renderer, bool standalone = false);

	void setScenario(const std::shared_ptr<FilamentScenarioObject> &scenario);

	void setCamera(const std::shared_ptr<FilamentCamera> &camera);

	void attachCanvas(const std::shared_ptr<FilamentCanvas> &canvas);
	void removeCanvas(const std::shared_ptr<FilamentCanvas> &canvas);

	void setParentViewport(const std::shared_ptr<FilamentViewportObject> &parent);

	void setSize(int width, int height);

	filament::Texture *texture() override;

	void renderViewport();

	inline bool isActive() const {
		return m_active;
	}

	inline void setActive(bool active) {
		m_active = active;
	}

	inline bool disable2D() const {
		return m_disable2D;
	}

	inline void setDisable2D(bool disable2D) {
		m_disable2D = disable2D;
	}

	inline bool disable3D() const {
		return m_disable3D;
	}

	void setDisable3D(bool disable3D);

protected:
	void doClean() override;

private:
	FilamentCanvasView *getViewOfCanvas(const std::shared_ptr<FilamentCanvas> &canvas);

	inline void addChild(FilamentViewportListItem *item) noexcept {
		m_childViewports.insertTail(item);
	}

	void addChild(const std::shared_ptr<FilamentViewportObject> &object);
	void removeChild(const std::shared_ptr<FilamentViewportObject> &object);

	FilamentEngineObject<filament::View> m_view;
	FilamentEngineObject<filament::Texture> m_framebufferTexture;
	FilamentEngineObject<filament::Texture> m_framebufferDepthTexture;
	FilamentEngineObject<filament::RenderTarget> m_renderTarget;
	FilamentWindow *m_registeredWithWindow;
	std::weak_ptr<FilamentCamera> m_camera;
	std::vector<std::unique_ptr<FilamentCanvasView>> m_attachedCanvases;
	std::shared_ptr<FilamentViewportObject> m_parentViewport;
	FilamentLinkedListEntry m_childViewports;
	Rect2 m_windowRect;
	Size2i m_size;
	bool m_active;
	bool m_disable2D;
	bool m_disable3D;
};

#endif
