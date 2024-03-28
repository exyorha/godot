#ifndef FILAMENT_CAMERA_H
#define FILAMENT_CAMERA_H

#include "filament_entity_object.h"

#include "core/math/vector2.h"

#include <utils/Entity.h>

#include <filament/Viewport.h>

#include <variant>

namespace filament {
	class Camera;
}

class FilamentCamera final : public FilamentEntityObject {
public:
	FilamentCamera();
	~FilamentCamera() override;

	inline filament::Camera *camera() const {
		return m_camera;
	}

	void setPerspective(float fov, double near, double far);

	void setOrthogonal(double size, double near, double far);

	void setFrustum(double size, const Vector2 &offset, double near, double far);

	inline void setUseVerticalAspect(bool enable) {
		m_verticalAspect = enable;
	}

	void configureForViewport(const filament::Viewport &viewport);

private:
	struct PerspectiveProjection {
		float fov;
		double near;
		double far;
	};

	struct OrthogonalProjection {
		double size;
		double near;
		double far;
	};

	struct FrustumProjection {
		double size;
		Vector2 offset;
		double near;
		double far;
	};

	using ConfiguredProjection = std::variant<std::monostate, PerspectiveProjection, OrthogonalProjection, FrustumProjection>;

	void configureForViewport(const filament::Viewport &viewport, const std::monostate &config);
	void configureForViewport(const filament::Viewport &viewport, const PerspectiveProjection &config);
	void configureForViewport(const filament::Viewport &viewport, const OrthogonalProjection &config);
	void configureForViewport(const filament::Viewport &viewport, const FrustumProjection &config);

	filament::Camera *m_camera;
	ConfiguredProjection m_projection;
	bool m_verticalAspect;
};

#endif
