#include "filament/filament_canvas_item_material_group.h"
#include "filament/filament_canvas_element_triangle_array.h"
#include "filament/filament_material_object.h"
#include "filament/filament_rendering_server_backend.h"
#include "filament/filament_shader_object.h"

FilamentCanvasItemMaterialGroup::FilamentCanvasItemMaterialGroup(FilamentControlledObjectReferenceOwner *owner,
																 RID texture)
	: m_texture(texture), m_material(owner) {

	m_material = std::make_shared<FilamentMaterialObject>();
	m_material->setFallbackParent(FilamentRenderingServerBackend::get()->defaultCanvasItemShader());

	if(texture.is_valid()) {
		m_material->setParam("defaultCanvasTexture", texture);
	}
}

FilamentCanvasItemMaterialGroup::~FilamentCanvasItemMaterialGroup() = default;


size_t FilamentCanvasItemMaterialGroup::numberOfPrimitives() const {
	size_t primitives = m_triangleArrays.size();

	if(m_textureRect.has_value()) {
		primitives++;
	}

	return primitives;
}

size_t FilamentCanvasItemMaterialGroup::build(filament::RenderableManager::Builder &builder, size_t index) {
	size_t startIndex = index;

	auto material = m_material->materialInstance();

	for(auto &triangleArray: m_triangleArrays) {
		triangleArray.build(builder, index);
		builder.material(index, material);
		index++;
	}

	if(m_textureRect.has_value()) {
		m_textureRect->build(builder, index);
		builder.material(index, material);
		index++;
	}

	return index - startIndex;
}

void FilamentCanvasItemMaterialGroup::addTriangleArray(
	const Vector<int> & p_indices,
	const Vector<Point2> & p_points,
	const Vector<Color> & p_colors,
	const Vector<Point2> & p_uvs,
	const Vector<int> & p_bones,
	const Vector<float> & p_weights
) {

	m_triangleArrays.emplace_back(
		p_indices, p_points, p_colors, p_uvs, p_bones, p_weights
	);
}

void FilamentCanvasItemMaterialGroup::addTextureRect(
	const Rect2 & p_rect,
	bool p_tile,
	const Color & p_modulate,
	bool p_transpose) {

	if(!m_textureRect.has_value()) {
		m_textureRect.emplace();
	}

	m_textureRect->addTextureRect(p_rect, p_tile, p_modulate, p_transpose);
}

void FilamentCanvasItemMaterialGroup::addTextureRectRegion(
	const Rect2 & p_rect,
	const Rect2 & p_src_rect,
	const Color & p_modulate,
	bool p_transpose,
	bool p_clip_uv) {


	if(!m_textureRect.has_value()) {
		m_textureRect.emplace();
	}

	m_textureRect->addTextureRectRegion(p_rect, p_src_rect, p_modulate, p_transpose, p_clip_uv);
}

