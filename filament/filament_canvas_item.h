#ifndef FILAMENT_CANVAS_ITEM_H
#define FILAMENT_CANVAS_ITEM_H

#include "filament/filament_canvas_item_container.h"
#include "filament/filament_entity_object.h"

#include <core/templates/vector.h>
#include <core/math/vector2.h>

#include <filament/RenderableManager.h>

class FilamentScenarioObject;
class FilamentTextureReferenceObject;
class FilamentCanvasItemMaterialGroup;
struct Transform2D;

class FilamentCanvasItem final : public FilamentEntityObject, public FilamentCanvasItemContainer {
public:
	FilamentCanvasItem();
	~FilamentCanvasItem() override;

	std::shared_ptr<FilamentCanvasItemContainer> parent() const;
	void setParent(const std::shared_ptr<FilamentCanvasItemContainer> &parent);

	void clear();

	FilamentCanvasItemMaterialGroup *getMaterialGroup(const std::shared_ptr<FilamentTextureReferenceObject> &texture);

	void setTransform(const Transform2D &transform);

	void setVisible(bool visible);

	bool isEffectivelyVisible() const override;

private:

	void updateOwningScene(const std::shared_ptr<FilamentScenarioObject> &scene);

	void doClean() override;

	void beforeGeometryChange();

	std::weak_ptr<FilamentCanvasItemContainer> m_parent;
	std::shared_ptr<FilamentScenarioObject> m_owningScene;
	std::vector<std::unique_ptr<FilamentCanvasItemMaterialGroup>> m_materialGroups;
	bool m_visible;
};

#endif

