 #ifndef FILAMENT_FILAMENT_TEXTURE_OBJECT_H
#define FILAMENT_FILAMENT_TEXTURE_OBJECT_H

#include "filament/filament_texture_reference_object.h"
#include "filament/filament_engine_object.h"

#include <filament/Texture.h>

#include <core/io/image.h>

#include <variant>

class FilamentTextureObject;
class FilamentTextureFormat;

class FilamentTextureObject final : public FilamentTextureReferenceObject {
public:
	FilamentTextureObject(
		filament::Texture::Sampler type,
		const FilamentTextureFormat &format,
		uint32_t width,
		uint32_t height,
		uint32_t depth,
		bool useMipMaps
	);
	~FilamentTextureObject() override;

	filament::Texture *texture() const override;

	static uint32_t calculateNumberOfMipMaps(uint32_t width, uint32_t height, uint32_t depth);

	void uploadMipChain(filament::Engine &engine, uint32_t index, const Ref<Image> &image);

	void uploadDepthSlice(filament::Engine &engine, uint32_t mipLevel, uint32_t depthSlice, const Ref<Image> &image);

private:
	struct UncompressedPixelFormat {
		filament::Texture::PixelBufferDescriptor::PixelDataFormat dataFormat;
		filament::Texture::PixelBufferDescriptor::PixelDataType dataType;
	};

	struct CompressedPixelFormat {
		filament::Texture::CompressedType dataType;
	};

	using PixelFormat = std::variant<UncompressedPixelFormat, CompressedPixelFormat>;

	static filament::Texture::PixelBufferDescriptor pixelBufferFromImage(const Ref<Image> &image, int32_t mipLevel);
	static filament::Texture::PixelBufferDescriptor pixelBufferFromImage(const UncompressedPixelFormat &format, const Ref<Image> &image, int32_t mipLevel);
	static filament::Texture::PixelBufferDescriptor pixelBufferFromImage(const CompressedPixelFormat &format, const Ref<Image> &image, int32_t mipLevel);

	static PixelFormat mapImageFormat(Image::Format format);

	static void releaseImage(void* buffer, size_t size, void *imagePtr);

	FilamentEngineObject<filament::Texture> m_texture;
};

#endif
