#include "filament_engine_object.h"
#include "filament_rendering_server.h"

#include "filament/Engine.h"

template<typename T>
void FilamentEngineObjectReleaser<T>::release(T *ptr) {
	FilamentRenderingServer::filamentEngine()->destroy(ptr);
}

template class FilamentEngineObjectReleaser<filament::SwapChain>;
template class FilamentEngineObjectReleaser<filament::Renderer>;
template class FilamentEngineObjectReleaser<filament::Texture>;

