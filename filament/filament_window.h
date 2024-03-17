#ifndef FILAMENT_WINDOW_H
#define FILAMENT_WINDOW_H

#include "filament_engine_object.h"

namespace filament {
	class SwapChain;
	class Renderer;
}

class FilamentWindow {
public:
	explicit FilamentWindow(void *nativeWindow);
	~FilamentWindow();

	FilamentWindow(const FilamentWindow &other) = delete;
	FilamentWindow &operator =(const FilamentWindow &other) = delete;

	void renderWindow();

private:
	FilamentEngineObject<filament::SwapChain> m_swapchain;
	FilamentEngineObject<filament::Renderer> m_renderer;
};

#endif
