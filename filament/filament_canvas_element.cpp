#include "filament/filament_canvas_element.h"
#include "filament/filament_rendering_server_backend.h"
#include "filament/filament_material_object.h"
#include "filament/filament_shader_object.h"

FilamentCanvasElement::FilamentCanvasElement(FilamentControlledObjectReferenceOwner *owner, RID texture) : m_texture(texture),
	m_material(owner) {

	m_material = std::make_shared<FilamentMaterialObject>();
	m_material->setFallbackParent(FilamentRenderingServerBackend::get()->defaultCanvasItemShader());

	if(texture.is_valid()) {
		m_material->setParam("defaultCanvasTexture", texture);
	}
}

FilamentCanvasElement::~FilamentCanvasElement() = default;

filament::MaterialInstance *FilamentCanvasElement::material() const {
	return m_material->materialInstance();
}

void FilamentCanvasElement::setScissor(const std::optional<Rect2i> &scissor) {
	m_material->setScissor(scissor);
}

