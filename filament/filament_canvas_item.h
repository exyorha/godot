#ifndef FILAMENT_CANVAS_ITEM_H
#define FILAMENT_CANVAS_ITEM_H

#include "filament/filament_canvas_item_container.h"
#include "filament/filament_entity_object.h"

#include <core/templates/vector.h>
#include <core/math/vector2.h>
#include <core/templates/rid.h>
#include <core/math/transform_2d.h>

#include <filament/RenderableManager.h>

class FilamentScenarioObject;
class FilamentTextureReferenceObject;
class FilamentCanvas;
class FilamentCanvasElement;
struct Color;
class FilamentCanvasElementTextureRect;

class FilamentCanvasItem final : public FilamentEntityObject, public FilamentCanvasItemContainer {
public:
	FilamentCanvasItem();
	~FilamentCanvasItem() override;

	std::shared_ptr<FilamentCanvasItemContainer> parent() const;
	void setParent(const std::shared_ptr<FilamentCanvasItemContainer> &parent);

	void clear();

	void setTransform(const Transform2D &transform);

	void setVisible(bool visible);

	bool isEffectivelyVisible() const override;

	inline int32_t zIndex() const {
		return m_zIndex;
	}

	void setZIndex(int zIndex);

	inline bool zRelativeToParent() const {
		return m_zRelativeToParent;
	}

	void setZRelativeToParent(bool zRelativeToParent);

	inline bool drawBehindParent() const {
		return m_drawBehindParent;
	}

	void setDrawBehindParent(bool drawBehindParent);

	inline float blendOrder() const {
		return m_blendOrder;
	}

	void setBlendOrder(float blendOrder);

	void addTextureRect(
		RID texture,
		const Rect2 & p_rect,
		bool p_tile,
		const Color & p_modulate,
		bool p_transpose);

	void addTextureRectRegion(
		RID texture,
		const Rect2 & p_rect,
		const Rect2 & p_src_rect,
		const Color & p_modulate,
		bool p_transpose,
		bool p_clip_uv);

	void addTriangleArray(
		RID texture,
		const Vector<int> & p_indices,
		const Vector<Point2> & p_points,
		const Vector<Color> & p_colors,
		const Vector<Point2> & p_uvs,
		const Vector<int> & p_bones,
		const Vector<float> & p_weights);

protected:
	int32_t calculateZOrder(int32_t parentZOrder) const override;
	std::optional<size_t> collectSelf(FilamentCanvasRenderOrderCollector &collector, int32_t calculatedZOrder) override;

private:

	void updateOwningScene(const std::shared_ptr<FilamentScenarioObject> &scene);

	void controlledObjectAboutToInvalidate(FilamentControlledObjectReferenceBase *linkedViaReference) override;

	void doClean() override;

	void beforeGeometryChange();

	void updateTransform();

	FilamentCanvasElementTextureRect *createOrReuseTextureRect(RID texture);

	std::weak_ptr<FilamentCanvasItemContainer> m_parent;
	std::weak_ptr<FilamentCanvas> m_owningCanvas;
	std::shared_ptr<FilamentScenarioObject> m_owningScene;
	std::vector<std::unique_ptr<FilamentCanvasElement>> m_elements;
	bool m_visible;
	int32_t m_zIndex;
	bool m_zRelativeToParent;
	bool m_drawBehindParent;
	float m_blendOrder;
	Transform2D m_transform;
};

#endif

