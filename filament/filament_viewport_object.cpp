#include "filament/filament_viewport_object.h"
#include "filament/filament_engine_object.h"
#include "filament/filament_rendering_server_backend.h"
#include "filament/filament_window.h"
#include "filament/filament_scenario_object.h"
#include "filament/filament_camera.h"
#include "filament/filament_canvas_view.h"

#include <filament/Engine.h>
#include <filament/Renderer.h>
#include <filament/View.h>
#include <filament/Viewport.h>
#include <filament/RenderTarget.h>
#include <filament/Texture.h>

FilamentViewportObject::FilamentViewportObject() :
	m_view(FilamentRenderingServerBackend::filamentEngine()->createView()),
	m_registeredWithWindow(nullptr), m_active(false), m_disable2D(false), m_disable3D(false) {

	if(!m_view) {
		throw std::bad_alloc();
	}

	m_view->setPostProcessingEnabled(false);

	FilamentRenderingServerBackend::get()->addTopLevelViewport(this);
}

FilamentViewportObject::~FilamentViewportObject() {
	setWindow(nullptr, Rect2());
}

void FilamentViewportObject::setWindow(FilamentWindow *window, const Rect2 & p_rect) {
	if(m_registeredWithWindow) {
		m_registeredWithWindow->unregisterViewport(this);
		m_registeredWithWindow = nullptr;
	}

	if(window) {
		window->registerViewport(this);
		m_registeredWithWindow = window;
	}

	m_windowRect = p_rect;

	markDirty();
}

void FilamentViewportObject::setSize(int width, int height) {
	m_size = Size2i(width, height);

	markDirty();
}

void FilamentViewportObject::doClean() {
	objectAboutToInvalidate();

	auto engine = FilamentRenderingServerBackend::filamentEngine();

	filament::Viewport viewport;

	if(m_registeredWithWindow) {
		m_renderTarget = FilamentEngineObject<filament::RenderTarget>();
		m_framebufferTexture = FilamentEngineObject<filament::Texture>();
		m_framebufferDepthTexture = FilamentEngineObject<filament::Texture>();

		viewport = filament::Viewport(
			// TODO: this is left, bottom; top left, top; verify the coordinate system
			m_windowRect.position.x, m_windowRect.position.y, m_windowRect.size.width, m_windowRect.size.height
		);

	} else {
		filament::Texture::Builder textureBuilder;

		auto actualWidth = std::max<int>(1, m_size.width);
		auto actualHeight = std::max<int>(1, m_size.height);

		textureBuilder.width(actualWidth);
		textureBuilder.height(actualHeight);
		textureBuilder.levels(1);
		textureBuilder.usage(
			filament::Texture::Usage::COLOR_ATTACHMENT |
			filament::Texture::Usage::SAMPLEABLE);

		if(!m_disable3D) {
			filament::Texture::Builder depthTextureBuilder;

			depthTextureBuilder.width(actualWidth);
			depthTextureBuilder.height(actualHeight);
			depthTextureBuilder.levels(1);
			depthTextureBuilder.usage(filament::Texture::Usage::DEPTH_ATTACHMENT);
			depthTextureBuilder.format(filament::Texture::InternalFormat::DEPTH24);
			m_framebufferDepthTexture = FilamentEngineObject<filament::Texture>(
				depthTextureBuilder.build(*engine)
			);
		} else {
			m_framebufferDepthTexture = FilamentEngineObject<filament::Texture>();
		}
/*
 * TODO: when implementing full sRGB
 *		textureBuilder.format(filament::Texture::InternalFormat::SRGB8_A8);
 */
		m_framebufferTexture = FilamentEngineObject<filament::Texture>(
			textureBuilder.build(*engine)
		);

		if(!m_framebufferTexture) {
			throw std::runtime_error("failed to create the texture");
		}


		filament::RenderTarget::Builder builder;
		builder.texture(filament::RenderTarget::AttachmentPoint::COLOR, m_framebufferTexture.get());

		if(m_framebufferDepthTexture) {
			builder.texture(filament::RenderTarget::AttachmentPoint::DEPTH, m_framebufferDepthTexture.get());
		}

		m_renderTarget = FilamentEngineObject<filament::RenderTarget>(builder.build(*engine));
		if(!m_renderTarget) {
			throw std::runtime_error("failed to create the render target");
		}

		viewport = filament::Viewport(
			0, 0, actualWidth, actualHeight
		);
	}

	m_view->setRenderTarget(m_renderTarget.get());
	m_view->setViewport(viewport);
}

void FilamentViewportObject::dissociateFromDestroyedWindow(FilamentWindow *window) noexcept {
	if(m_registeredWithWindow == window) {
		m_registeredWithWindow = nullptr;
	}

	markDirty();
}

void FilamentViewportObject::render(filament::Renderer *renderer, bool standalone) {
	/*
	 * This is called for both non-window viewports (through renderViewport())
	 * and for window viewports.
	 */

	if(!m_active) {
		return;
	}

	clean();

	auto cameraObject = m_camera.lock();

	filament::Camera *camera = nullptr;

	auto viewport = m_view->getViewport();
	auto renderTarget = m_view->getRenderTarget();

	bool yFlip = renderTarget != nullptr && renderer->getEngine()->getBackend() == filament::backend::Backend::OPENGL;

	if(standalone) {
		renderer->setClearOptions(filament::Renderer::ClearOptions{
			.clearColor = filament::math::float4(0, 0, 0, 1),
			.clear = true,
			.discard = false
		});
	}

	if(!m_disable3D) {

		m_view->setFrontFaceWindingInverted(!yFlip);

		if(cameraObject) {
			cameraObject->configureForViewport(viewport, yFlip);
			camera = cameraObject->camera();
		}

		m_view->setCamera(camera);

		if(camera) {

			if(standalone) {
				renderer->renderStandaloneView(m_view.get());

				renderer->setClearOptions(filament::Renderer::ClearOptions{
					.clear = false,
					.discard = false
				});
			} else {
				renderer->render(m_view.get());
			}
		}
	}


	if(!m_disable2D) {
		/*
		* TODO: canvas sorting.
		*/
		for(const auto &viewPtr: m_attachedCanvases) {
			viewPtr->render(renderer, renderTarget, viewport, standalone, yFlip);
		}
	}
}

void FilamentViewportObject::setScenario(const std::shared_ptr<FilamentScenarioObject> &scenario) {
	filament::Scene *scene = nullptr;
	if(scenario) {
		scene = scenario->scene();
	}

	m_view->setScene(scene);
}

void FilamentViewportObject::setCamera(const std::shared_ptr<FilamentCamera> &cameraObject) {
	m_camera = cameraObject;
}

void FilamentViewportObject::attachCanvas(const std::shared_ptr<FilamentCanvas> &canvas) {
	if(!canvas) {
		return;
	}

	auto existing = getViewOfCanvas(canvas);
	if(existing) {
		return;
	}

	m_attachedCanvases.emplace_back(std::make_unique<FilamentCanvasView>(canvas));
}

void FilamentViewportObject::removeCanvas(const std::shared_ptr<FilamentCanvas> &canvas) {
	if(!canvas) {
		return;
	}

	auto end = std::remove_if(m_attachedCanvases.begin(), m_attachedCanvases.end(), [&canvas](const auto &view) { return view->canvas() == canvas; });
	m_attachedCanvases.erase(end, m_attachedCanvases.end());
}

FilamentCanvasView *FilamentViewportObject::getViewOfCanvas(const std::shared_ptr<FilamentCanvas> &canvas) {
	if(!canvas) {
		return nullptr;
	}

	for(const auto &viewPtr:m_attachedCanvases) {
		if(viewPtr->canvas() == canvas) {
			return viewPtr.get();
		}
	}

	return nullptr;
}

void FilamentViewportObject::setParentViewport(const std::shared_ptr<FilamentViewportObject> &parent) {
	if(m_parentViewport != parent) {
		auto sharedThis = std::static_pointer_cast<FilamentViewportObject>(shared_from_this());

		FilamentViewportListItem::remove();

		m_parentViewport = parent;

		if(m_parentViewport) {
			m_parentViewport->addChild(this);
		} else {
			FilamentRenderingServerBackend::get()->addTopLevelViewport(this);
		}
	}
}

filament::Texture *FilamentViewportObject::texture() {
	clean();

	return m_framebufferTexture.get();
}

void FilamentViewportObject::renderViewport() {
	for(auto item = m_childViewports.next(); item != &m_childViewports; item = item->next()) {
		static_cast<FilamentViewportObject *>(static_cast<FilamentViewportListItem *>(item))->renderViewport();
	}

	/*
	 * Viewports that are registered with a window are drawn by the window.
	 * Inactive viewports don't get drawn.
	 */
	if(m_registeredWithWindow || !m_active) {
		return;
	}

	render(FilamentRenderingServerBackend::get()->offscreenRenderer(), true);
}

void FilamentViewportObject::setDisable3D(bool disable3D) {
	if(disable3D != m_disable3D) {
		m_disable3D = disable3D;
		markDirty();
	}
}
