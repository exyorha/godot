#include "filament/filament_camera.h"
#include "filament/filament_rendering_server_backend.h"

#include <filament/Engine.h>
#include <filament/Camera.h>

FilamentCamera::FilamentCamera() : m_camera(FilamentRenderingServerBackend::filamentEngine()->createCamera(entity())), m_verticalAspect(false) {
	if(!m_camera) {
		throw std::bad_alloc();
	}

	m_camera->setExposure(1.0f);
}

FilamentCamera::~FilamentCamera() = default;

void FilamentCamera::setPerspective(float fov, double near, double far) {
	m_projection.emplace<PerspectiveProjection>(PerspectiveProjection{ fov, near, far });
}

void FilamentCamera::setOrthogonal(double size, double near, double far) {
	m_projection.emplace<OrthogonalProjection>(OrthogonalProjection{ size, near, far });
}

void FilamentCamera::setFrustum(double size, const Vector2 &offset, double near, double far) {
	m_projection.emplace<FrustumProjection>(FrustumProjection{ size, offset, near, far });
}

void FilamentCamera::configureForViewport(const filament::Viewport &viewport, bool yFlip) {
	std::visit([this, &viewport, yFlip](const auto &value) { configureForViewport(viewport, value, yFlip); }, m_projection);
}

void FilamentCamera::configureForViewport(const filament::Viewport &viewport, const std::monostate &config, bool yFlip) {
	(void)viewport;
	(void)config;
}

void FilamentCamera::configureForViewport(const filament::Viewport &viewport, const PerspectiveProjection &config, bool yFlip) {
	filament::Camera::Fov direction;

	/*
	 * Godot 'vertical aspect' flag means 'keep the aspect ratio for the display oriented vertically', that is:
	 *   vertical aspect = false:
	 * 		- the fov specified is the fov for the Y (vertically oriented) axis, and the X axis is free.
	 * 		- this corresponds to the Camera3D setting of KeepAspect::KEEP_HEIGHT, and is the default.
	 *      - in Filament speak, this corresponds to Camera::Fov::VERTICAL.
	 *   vertical aspect = true:
	 * 		- the fov specified is the fov for the X (horizontally oriented) axis, and the Y axis is free
	 * 		- this corresponds to the Camera3D setting of KeepAspect::KEEP_WIDTH.
	 *      - in Filament speak, this corresponds to Camera::Fov::HORIZONTAL.
	 */

	if(m_verticalAspect) {
		direction = filament::Camera::Fov::HORIZONTAL;
	} else {
		direction = filament::Camera::Fov::VERTICAL;
	}

	double aspect = viewport.width * 1.0 / viewport.height;

	if(yFlip) {
		auto matrix = filament::Camera::projection(direction, config.fov, aspect, config.near, config.far);

		matrix[1][1] = -matrix[1][1];

		m_camera->setCustomProjection(matrix, config.near, config.far);
	} else {
		m_camera->setProjection(config.fov, aspect, config.near, config.far, direction);
	}
}

void FilamentCamera::configureForViewport(const filament::Viewport &viewport, const OrthogonalProjection &config, bool yFlip) {
	double left;
	double right;
	double top;
	double bottom;

	double aspect = viewport.width * 1.0 / viewport.height;

	/*
	 * m_verticalAspect = false: the size specified is the Y axis size of the frustum,
	 * m_verticalAspect = true: the size specified is the X axis size of the frustum.
	 */
	if(m_verticalAspect) {
		left = -config.size * 0.5;
		right = config.size * 0.5;

		double sizeY = config.size / aspect;

		top = sizeY * 0.5;
		bottom = -sizeY * 0.5;
	} else {
		top = config.size * 0.5f;
		bottom = -config.size * 0.5f;

		double sizeX = config.size * aspect;

		left = -sizeX * 0.5;
		right = sizeX * 0.5;
	}

	if(yFlip) {
		std::swap(bottom, top);
	}

	m_camera->setProjection(filament::Camera::Projection::ORTHO, left, right, bottom, top, config.near, config.far);
}

void FilamentCamera::configureForViewport(const filament::Viewport &viewport, const FrustumProjection &config, bool yFlip) {
	double left;
	double right;
	double top;
	double bottom;


	double aspect = viewport.width * 1.0 / viewport.height;

	/*
	 * m_verticalAspect = false: the size specified is the Y axis size of the frustum,
	 * m_verticalAspect = true: the size specified is the X axis size of the frustum.
	 */
	if(m_verticalAspect) {
		left = -config.size * 0.5 + config.offset.x;
		right = config.size * 0.5 + config.offset.x;

		double sizeY = config.size / aspect;

		top = sizeY * 0.5 + config.offset.y;
		bottom = -sizeY * 0.5 + config.offset.y;
	} else {
		top = config.size * 0.5f + config.offset.y;
		bottom = -config.size * 0.5f + config.offset.y;

		double sizeX = config.size * aspect;

		left = -sizeX * 0.5 + config.offset.x;
		right = sizeX * 0.5 + config.offset.x;
	}

	if(yFlip) {
		std::swap(bottom, top);
	}

	m_camera->setProjection(filament::Camera::Projection::ORTHO, left, right, bottom, top, config.near, config.far);
}

