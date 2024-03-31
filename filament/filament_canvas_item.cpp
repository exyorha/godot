#include "filament/filament_canvas_item.h"
#include "filament/filament_canvas.h"
#include "filament/filament_entity_object.h"
#include "filament/filament_scenario_object.h"
#include "filament/filament_rendering_server_backend.h"
#include "filament/filament_canvas_item_material_group.h"

#include <filament/Scene.h>
#include <filament/Engine.h>

FilamentCanvasItem::FilamentCanvasItem() : m_visible(true) {

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

		std::shared_ptr<FilamentScenarioObject> scene;
		if(canvas) {
			scene = canvas->getBackingScene();
		}

		updateOwningScene(scene);
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
			printf("FilamentCanvasItem %p: placed to scene %p\n", this, m_owningScene->scene());
		} else {
			printf("FilamentCanvasItem %p: removed from scene\n", this);
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

	m_materialGroups.clear();
}

FilamentCanvasItemMaterialGroup *FilamentCanvasItem::getMaterialGroup(const std::shared_ptr<FilamentTextureReferenceObject> &texture) {

	/*
	 * TODO: if texture is null, subsitute the default solid white texture
	 */

	beforeGeometryChange();

	for(const auto &group: m_materialGroups) {
		if(group->texture() == texture) {
			return group.get();
		}
	}

	return m_materialGroups.emplace_back(std::make_unique<FilamentCanvasItemMaterialGroup>(texture)).get();
}

void FilamentCanvasItem::doClean() {
	if(!isEffectivelyVisible()) {
		return;
	}

	auto engine = FilamentRenderingServerBackend::filamentEngine();

	size_t primitiveCount = 0;
	for(const auto &elementPtr: m_materialGroups) {
		primitiveCount += elementPtr->numberOfPrimitives();
	}

	if(primitiveCount == 0) {
		printf("FilamentCanvasItem %p: empty, will not draw\n", this);
		return;
	}

	filament::RenderableManager::Builder builder(primitiveCount);
	auto firstPrimitive = 0;

	for(const auto &elementPtr: m_materialGroups) {
		firstPrimitive += elementPtr->build(builder, firstPrimitive);
	}

	builder.culling(false);
	filament::Box box;
	box.set(filament::math::float3(-100000, -100000, -100000), filament::math::float3(100000, 100000, 100000));
	builder.boundingBox(box);

	auto result = builder.build(*engine, entity());
	if(result != decltype(result)::Success) {
		throw std::runtime_error("failed to build the canvas item renderable");
	}


	printf("FilamentCanvasItem %p: built with %zu primitves\n", this, primitiveCount);
}

void FilamentCanvasItem::setTransform(const Transform2D &transform) {
	Transform3D transform3D(
		transform.columns[0].x, transform.columns[0].y, 0,
		transform.columns[1].x, transform.columns[1].y, 0,
		0, 0, 1,
		transform.columns[2].x, transform.columns[2].y, 0);

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
