#include "filament/filament_skeleton_object.h"
#include "filament/filament_rendering_server_backend.h"
#include "filament/filament_math_adapters.h"

#include <filament/SkinningBuffer.h>
#include <filament/Engine.h>

FilamentSkeletonObject::FilamentSkeletonObject() = default;

FilamentSkeletonObject::~FilamentSkeletonObject() = default;

void FilamentSkeletonObject::allocate(size_t bones) {
	if(bones != m_bones.size()) {
		if(m_buffer) {
			objectAboutToInvalidate();
			m_buffer = FilamentEngineObject<filament::SkinningBuffer>();
		}

		m_bones.resize(bones);

		if(!m_bones.empty()) {

			m_buffer = FilamentEngineObject<filament::SkinningBuffer>(
				filament::SkinningBuffer::Builder()
					.boneCount(m_bones.size())
					.initialize(false)
					.build(*FilamentRenderingServerBackend::filamentEngine()));

			if(!m_buffer) {
				throw std::runtime_error("failed to create the skinning buffer");
			}

			markDirty();
		}
	}
}

void FilamentSkeletonObject::setTransform(size_t boneIndex, const Transform3D &transform) {
	if(boneIndex >= m_bones.size()) {
		throw std::runtime_error("bad bone index");
	}

	m_bones[boneIndex] = filamentTransformFromGodotTransform<float>(transform);

	markDirty();
}

Transform3D FilamentSkeletonObject::getTransform(size_t boneIndex) const {

	if(boneIndex >= m_bones.size()) {
		throw std::runtime_error("bad bone index");
	}

	return godotTransformFromFilamentTransform(m_bones[boneIndex]);

}

filament::SkinningBuffer *FilamentSkeletonObject::buffer() {
	clean();

	return m_buffer.get();
}

void FilamentSkeletonObject::doClean() {
	if(m_buffer) {
		if(m_buffer->getBoneCount() != m_bones.size()) {
			throw std::runtime_error("Unexpected mismatch between the CPU-side bone count and SkinningBuffer bone count");
		}

		m_buffer->setBones(*FilamentRenderingServerBackend::filamentEngine(), m_bones.data(), m_bones.size());
	}
}
