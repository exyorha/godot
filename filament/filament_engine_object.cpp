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
template class FilamentEngineObjectReleaser<filament::View>;
template class FilamentEngineObjectReleaser<filament::Scene>;
template class FilamentEngineObjectReleaser<filament::VertexBuffer>;
template class FilamentEngineObjectReleaser<filament::IndexBuffer>;
template class FilamentEngineObjectReleaser<filament::Material>;
template class FilamentEngineObjectReleaser<filament::MaterialInstance>;
template class FilamentEngineObjectReleaser<filament::SkinningBuffer>;
template class FilamentEngineObjectReleaser<filament::RenderTarget>;

