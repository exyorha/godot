#ifndef FILAMENT_SKELETON_OBJECT_H
#define FILAMENT_SKELETON_OBJECT_H

#include <vector>

#include "filament_object.h"

#include "filament/filament_engine_object.h"

#include <math/mat4.h>

namespace filament {
	class SkinningBuffer;
}

struct Transform3D;

class FilamentSkeletonObject final : public FilamentObject {
public:
	FilamentSkeletonObject();
	~FilamentSkeletonObject() override;

	void allocate(size_t bones);

	inline size_t boneCount() const {
		return m_bones.size();
	}

	void setTransform(size_t boneIndex, const Transform3D &transform);
	Transform3D getTransform(size_t boneIndex) const;

	filament::SkinningBuffer *buffer();

protected:
	void doClean() override;

private:
	std::vector<filament::math::mat4f> m_bones;
	FilamentEngineObject<filament::SkinningBuffer> m_buffer;
};

#endif


