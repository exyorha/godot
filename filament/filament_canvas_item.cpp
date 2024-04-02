#include "filament/filament_canvas_item.h"
#include "filament/filament_canvas.h"
#include "filament/filament_controlled_object_reference.h"
#include "filament/filament_entity_object.h"
#include "filament/filament_scenario_object.h"
#include "filament/filament_rendering_server_backend.h"
#include "filament/filament_canvas_render_order_collector.h"
#include "filament/filament_canvas_element_texture_rect.h"
#include "filament/filament_canvas_element_triangle_array.h"

#include <filament/Scene.h>
#include <filament/Engine.h>

FilamentCanvasItem::FilamentCanvasItem() : m_visible(true), m_zIndex(0), m_zRelativeToParent(true), m_drawBehindParent(false),
	m_blendOrder(0) {

}

FilamentCanvasItem::~FilamentCanvasItem() = default;

std::shared_ptr<FilamentCanvasItemContainer> FilamentCanvasItem::parent() const {
	return m_parent.lock();
}

void FilamentCanvasItem::setParent(const std::shared_ptr<FilamentCanvasItemContainer> &parent) {
	auto currentParent = this->parent();
	auto self = std::static_pointer_cast<FilamentCanvasItem>(shared_from_this());

	if(currentParent != parent) {
		beforeGeometryChange();

		if(currentParent) {
			currentParent->removeChild(self);
		}

		m_parent = parent;

		if(parent) {
			parent->addChild(self);
		}

		auto parentItem = std::dynamic_pointer_cast<FilamentCanvasItem>(parent);
		FilamentEntityObject::setParent(parentItem);

		auto canvas = FilamentCanvas::getCanvas(parent);
		m_owningCanvas = canvas;

		std::shared_ptr<FilamentScenarioObject> scene;
		if(canvas) {
			scene = canvas->getBackingScene();
		}

		updateOwningScene(scene);

		if(canvas) {
			canvas->renderingOrderChanged();
		}
	}
}

void FilamentCanvasItem::updateOwningScene(const std::shared_ptr<FilamentScenarioObject> &scene) {
	if(m_owningScene != scene) {
		if(m_owningScene) {
			m_owningScene->scene()->remove(entity());
		}

		m_owningScene = scene;

		if(m_owningScene) {
			m_owningScene->scene()->addEntity(entity());
		}

		for(const auto &weakChild: children()) {
			auto child = weakChild.lock();
			if(child) {
				child->updateOwningScene(scene);
			}
		}
	}
}

void FilamentCanvasItem::beforeGeometryChange() {
	auto engine = FilamentRenderingServerBackend::filamentEngine();

	auto &rm = engine->getRenderableManager();

	if(rm.hasComponent(entity())) {
		rm.destroy(entity());
	}

	markDirty();
}

void FilamentCanvasItem::clear() {
	beforeGeometryChange();

	m_elements.clear();
}

FilamentCanvasElementTextureRect *FilamentCanvasItem::createOrReuseTextureRect(RID texture) {
	if(!m_elements.empty() &&
		m_elements.back()->type() == FilamentCanvasElement::Type::TextureRect &&
		m_elements.back()->texture() == texture) {

		return static_cast<FilamentCanvasElementTextureRect *>(m_elements.back().get());
	} else {
		return static_cast<FilamentCanvasElementTextureRect *>(
			m_elements.emplace_back(std::make_unique<FilamentCanvasElementTextureRect>(
				static_cast<FilamentControlledObjectReferenceOwner *>(this), texture)).get()
		);
	}
}


void FilamentCanvasItem::addTextureRect(
	RID texture,
	const Rect2 & p_rect,
	bool p_tile,
	const Color & p_modulate,
	bool p_transpose) {

	beforeGeometryChange();

	createOrReuseTextureRect(texture)->addTextureRect(p_rect,  p_tile, p_modulate, p_transpose);
}

void FilamentCanvasItem::addTextureRectRegion(
	RID texture,
	const Rect2 & p_rect,
	const Rect2 & p_src_rect,
	const Color & p_modulate,
	bool p_transpose,
	bool p_clip_uv) {

	beforeGeometryChange();

	createOrReuseTextureRect(texture)->addTextureRectRegion(p_rect, p_src_rect, p_modulate, p_transpose, p_clip_uv);
}

void FilamentCanvasItem::addTriangleArray(
	RID texture,
	const Vector<int> & p_indices,
	const Vector<Point2> & p_points,
	const Vector<Color> & p_colors,
	const Vector<Point2> & p_uvs,
	const Vector<int> & p_bones,
	const Vector<float> & p_weights) {

	beforeGeometryChange();

	m_elements.emplace_back(std::make_unique<FilamentCanvasElementTriangleArray>(
		static_cast<FilamentControlledObjectReferenceOwner *>(this),
		texture,
		p_indices,
		p_points,
		p_colors,
		p_uvs,
		p_bones,
		p_weights));
}

void FilamentCanvasItem::controlledObjectAboutToInvalidate(FilamentControlledObjectReferenceBase *linkedViaReference) {
	FilamentEntityObject::controlledObjectAboutToInvalidate(linkedViaReference);

	beforeGeometryChange();
}

void FilamentCanvasItem::doClean() {
	if(!isEffectivelyVisible()) {
		return;
	}

	auto engine = FilamentRenderingServerBackend::filamentEngine();

	size_t primitiveCount = m_elements.size();

	if(primitiveCount == 0) {
		printf("FilamentCanvasItem %p: empty, will not draw\n", this);
		return;
	}

	filament::RenderableManager::Builder builder(primitiveCount);
	auto firstPrimitive = 0;

	for(const auto &elementPtr: m_elements) {
		elementPtr->build(builder, firstPrimitive);
		builder.blendOrder(firstPrimitive, std::min<size_t>(firstPrimitive, 32767));
		firstPrimitive++;
	}

	builder.culling(false);
	filament::Box box;
	box.set(filament::math::float3(-100000, -100000, -100000), filament::math::float3(100000, 100000, 100000));
	builder.boundingBox(box);

	auto result = builder.build(*engine, entity());
	if(result != decltype(result)::Success) {
		throw std::runtime_error("failed to build the canvas item renderable");
	}
}

void FilamentCanvasItem::setTransform(const Transform2D &transform) {
	m_transform = transform;
	updateTransform();
}

void FilamentCanvasItem::updateTransform() {
	Transform3D transform3D(
		m_transform.columns[0].x, m_transform.columns[0].y, 0,
		m_transform.columns[1].x, m_transform.columns[1].y, 0,
		0, 0, 1,
		m_transform.columns[2].x, m_transform.columns[2].y, m_blendOrder);

	FilamentEntityObject::setTransform(transform3D);

}

void FilamentCanvasItem::setVisible(bool visible) {
	if(visible != m_visible) {
		beforeGeometryChange();

		m_visible = visible;
	}
}

bool FilamentCanvasItem::isEffectivelyVisible() const {
	if(!m_visible) {
		return false;
	}

	auto parent = this->parent();
	if(!parent) {
		return true;
	}

	return parent->isEffectivelyVisible();
}


void FilamentCanvasItem::setZIndex(int zIndex) {
	if(m_zIndex != zIndex) {
		m_zIndex = zIndex;

		auto canvas = m_owningCanvas.lock();
		if(canvas) {
			canvas->renderingOrderChanged();
		}
	}
}

void FilamentCanvasItem::setZRelativeToParent(bool zRelativeToParent) {
	if(m_zRelativeToParent != zRelativeToParent) {
		m_zRelativeToParent = zRelativeToParent;

		auto canvas = m_owningCanvas.lock();
		if(canvas) {
			canvas->renderingOrderChanged();
		}
	}
}

int32_t FilamentCanvasItem::calculateZOrder(int32_t parentZOrder) const {
	if(m_zRelativeToParent) {
		return parentZOrder + m_zIndex;
	} else {
		return m_zIndex;
	}
}

void FilamentCanvasItem::setDrawBehindParent(bool drawBehindParent) {
	if(m_drawBehindParent != drawBehindParent) {
		m_drawBehindParent = drawBehindParent;

		auto canvas = m_owningCanvas.lock();
		if(canvas) {
			canvas->renderingOrderChanged();
		}
	}
}

void FilamentCanvasItem::setBlendOrder(float blendOrder) {
	m_blendOrder = blendOrder;

	updateTransform();
}

std::optional<size_t> FilamentCanvasItem::collectSelf(FilamentCanvasRenderOrderCollector &collector, int32_t calculatedZOrder) {
	return collector.collectItem(std::static_pointer_cast<FilamentCanvasItem>(shared_from_this()), calculatedZOrder);
}
