#ifndef FILAMENT_WINDOW_H
#define FILAMENT_WINDOW_H

#include "filament_engine_object.h"

#include <vector>

namespace filament {
	class SwapChain;
	class Renderer;
}

class FilamentViewportObject;

class FilamentWindow {
public:
	explicit FilamentWindow(void *nativeWindow);
	~FilamentWindow();

	FilamentWindow(const FilamentWindow &other) = delete;
	FilamentWindow &operator =(const FilamentWindow &other) = delete;

	void renderWindow();

	void registerViewport(FilamentViewportObject *viewport);
	void unregisterViewport(FilamentViewportObject *viewport) noexcept;

private:
	FilamentEngineObject<filament::SwapChain> m_swapchain;
	FilamentEngineObject<filament::Renderer> m_renderer;
	std::vector<FilamentViewportObject *> m_viewports;
};

#endif
