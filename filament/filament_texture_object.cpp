#include "filament/filament_texture_object.h"
#include "filament/filament_rendering_server_backend.h"
#include "filament/filament_texture_format.h"

FilamentTextureObject::FilamentTextureObject(
	filament::Texture::Sampler type,
	const FilamentTextureFormat &format,
	uint32_t width,
	uint32_t height,
	uint32_t depth,
	bool useMipMaps
) {

	uint32_t numberOfMipmaps;
	if(!useMipMaps) {
		numberOfMipmaps = 1;
	} else if(type == filament::Texture::Sampler::SAMPLER_3D) {
		numberOfMipmaps = calculateNumberOfMipMaps(width, height, depth);
	} else {
		numberOfMipmaps = calculateNumberOfMipMaps(width, height, 1);
	}

	filament::Texture::Builder builder;
	builder.sampler(type).width(width).height(height).depth(depth).levels(numberOfMipmaps).format(format.format());

	if(format.usesSwizzle()) {
		builder.swizzle(format.swizzleR(), format.swizzleG(), format.swizzleB(), format.swizzleA());
	}

	m_texture = FilamentEngineObject<filament::Texture>(builder.build(*FilamentRenderingServerBackend::filamentEngine()));
	if(!m_texture) {
		throw std::bad_alloc();
	}
}

FilamentTextureObject::~FilamentTextureObject() = default;

filament::Texture *FilamentTextureObject::texture() {
	return m_texture.get();
}

uint32_t FilamentTextureObject::calculateNumberOfMipMaps(uint32_t width, uint32_t height, uint32_t depth) {
	uint32_t mipWidth = width;
	uint32_t mipHeight = height;
	uint32_t mipDepth = depth;

	uint32_t numberOfMipmaps = 1;

	while(mipWidth > 1 || mipHeight > 1 || mipDepth > 1) {
		mipWidth = std::max<uint32_t>(mipWidth >> 1, 1);
		mipHeight = std::max<uint32_t>(mipHeight >> 1, 1);
		mipDepth = std::max<uint32_t>(mipDepth >> 1, 1);
		numberOfMipmaps++;
	}

	return numberOfMipmaps;
}

void FilamentTextureObject::uploadMipChain(filament::Engine &engine, uint32_t index, const Ref<Image> &image) {
	auto mipmapsToUpload = std::min<uint32_t>(m_texture->getLevels(), image->get_mipmap_count() + 1);

	if(image.is_null()) {
		fprintf(stderr, "FilamentTextureObject::uploadMipChain: the passed image is null\n");
		return;
	}

	for(uint32_t mipLevel = 0; mipLevel < mipmapsToUpload; mipLevel++) {
		auto mipWidth = m_texture->getWidth(mipLevel);
		auto mipHeight = m_texture->getHeight(mipLevel);

		int32_t dataOffset;
		int32_t dataLength;
		int32_t imageWidth;
		int32_t imageHeight;
		image->get_mipmap_offset_size_and_dimensions(mipLevel, dataOffset, dataLength, imageWidth, imageHeight);

		if(mipWidth != imageWidth || mipHeight != imageHeight) {
			fprintf(stderr, "FilamentTextureObject::uploadMipChain: the dimensions of the image don't match the expectation: at the mip level %u, expected %zux%zu, got %dx%d\n",
				mipLevel, mipWidth, mipHeight, imageWidth, imageHeight);
			return;
		}

		m_texture->setImage(engine, mipLevel, 0, 0, index, mipWidth, mipHeight, 1, pixelBufferFromImage(image, mipLevel));
	}
}

void FilamentTextureObject::uploadDepthSlice(filament::Engine &engine, uint32_t mipLevel, uint32_t depthSlice, const Ref<Image> &image) {

	if(image.is_null()) {
		fprintf(stderr, "FilamentTextureObject::uploadDepthSlice: the passed image is null\n");
		return;
	}

	if(mipLevel >= m_texture->getLevels()) {
		fprintf(stderr, "FilamentTextureObject::uploadDepthSlice: incorrect mip level %u. The image only has %zu levels\n",
				mipLevel, m_texture->getLevels());
		return;
	}

	auto mipWidth = m_texture->getWidth(mipLevel);
	auto mipHeight = m_texture->getHeight(mipLevel);
	auto mipDepth = m_texture->getDepth(mipLevel);

	if(mipWidth != image->get_width() || mipHeight != image->get_height() || depthSlice >= mipDepth) {
		fprintf(stderr, "FilamentTextureObject::uploadDepthSlice: incorrect image dimensions or incorrect slice number. At the mip level %u, the texture has %zu slices and is of size %zux%zu. Passed the slice number %u and an image of size %dx%d\n",
				mipLevel, mipDepth, mipWidth, mipHeight, depthSlice, image->get_width(), image->get_height());
		return;
	}

	m_texture->setImage(engine, mipLevel, 0, 0, depthSlice, mipWidth, mipHeight, 1, pixelBufferFromImage(image, 0));
}

filament::Texture::PixelBufferDescriptor FilamentTextureObject::pixelBufferFromImage(const Ref<Image> &image, int32_t mipLevel) {

	const auto &format = mapImageFormat(image->get_format());

	return std::visit([&image, mipLevel](const auto &formatSpec) -> filament::Texture::PixelBufferDescriptor {
		return pixelBufferFromImage(formatSpec, image, mipLevel);
	}, format);
}

filament::Texture::PixelBufferDescriptor FilamentTextureObject::pixelBufferFromImage(const UncompressedPixelFormat &format, const Ref<Image> &image, int32_t mipLevel) {

	int32_t dataOffset;
	int32_t dataLength;
	int32_t imageWidth;
	int32_t imageHeight;
	image->get_mipmap_offset_size_and_dimensions(mipLevel, dataOffset, dataLength, imageWidth, imageHeight);

	filament::Texture::PixelBufferDescriptor descriptor(
		image->get_data().ptr() + dataOffset, dataLength, format.dataFormat, format.dataType, nullptr, releaseImage, image.ptr()
	);

	image->reference();

	return descriptor;
}

filament::Texture::PixelBufferDescriptor FilamentTextureObject::pixelBufferFromImage(const CompressedPixelFormat &format, const Ref<Image> &image, int32_t mipLevel) {

	int32_t dataOffset;
	int32_t dataLength;
	int32_t imageWidth;
	int32_t imageHeight;
	image->get_mipmap_offset_size_and_dimensions(mipLevel, dataOffset, dataLength, imageWidth, imageHeight);

	filament::Texture::PixelBufferDescriptor descriptor(
		image->get_data().ptr() + dataOffset, dataLength, format.dataType, dataLength, nullptr, releaseImage, image.ptr()
	);

	image->reference();

	return descriptor;
}


void FilamentTextureObject::releaseImage(void* buffer, size_t size, void *imagePtr) {
	(void)buffer;
	(void)size;

	static_cast<Image *>(imagePtr)->unreference();
}

auto FilamentTextureObject::mapImageFormat(Image::Format format) -> PixelFormat {


	switch(format) {
		case Image::FORMAT_L8:
		case Image::FORMAT_R8:
			return UncompressedPixelFormat{filament::Texture::PixelBufferDescriptor::PixelDataFormat::R, filament::Texture::PixelBufferDescriptor::PixelDataType::UBYTE };

		case Image::FORMAT_LA8:
		case Image::FORMAT_RG8:
			return UncompressedPixelFormat{filament::Texture::PixelBufferDescriptor::PixelDataFormat::RG, filament::Texture::PixelBufferDescriptor::PixelDataType::UBYTE };

		case Image::FORMAT_RGB8:
			return UncompressedPixelFormat{filament::Texture::PixelBufferDescriptor::PixelDataFormat::RGB, filament::Texture::PixelBufferDescriptor::PixelDataType::UBYTE };

		case Image::FORMAT_RGBA8:
			return UncompressedPixelFormat{filament::Texture::PixelBufferDescriptor::PixelDataFormat::RGBA, filament::Texture::PixelBufferDescriptor::PixelDataType::UBYTE };

		// Image::FORMAT_RGBA4444 - not representable

		case Image::FORMAT_RGB565:
			return UncompressedPixelFormat{filament::Texture::PixelBufferDescriptor::PixelDataFormat::RGB, filament::Texture::PixelBufferDescriptor::PixelDataType::USHORT_565 };

		case Image::FORMAT_RF:
			return UncompressedPixelFormat{filament::Texture::PixelBufferDescriptor::PixelDataFormat::R, filament::Texture::PixelBufferDescriptor::PixelDataType::FLOAT };

		case Image::FORMAT_RGF:
			return UncompressedPixelFormat{filament::Texture::PixelBufferDescriptor::PixelDataFormat::RG, filament::Texture::PixelBufferDescriptor::PixelDataType::FLOAT };

		case Image::FORMAT_RGBF:
			return UncompressedPixelFormat{filament::Texture::PixelBufferDescriptor::PixelDataFormat::RGB, filament::Texture::PixelBufferDescriptor::PixelDataType::FLOAT };

		case Image::FORMAT_RGBAF:
			return UncompressedPixelFormat{filament::Texture::PixelBufferDescriptor::PixelDataFormat::RGBA, filament::Texture::PixelBufferDescriptor::PixelDataType::FLOAT };

		case Image::FORMAT_RH:
			return UncompressedPixelFormat{filament::Texture::PixelBufferDescriptor::PixelDataFormat::R, filament::Texture::PixelBufferDescriptor::PixelDataType::HALF };

		case Image::FORMAT_RGH:
			return UncompressedPixelFormat{filament::Texture::PixelBufferDescriptor::PixelDataFormat::RG, filament::Texture::PixelBufferDescriptor::PixelDataType::HALF };

		case Image::FORMAT_RGBH:
			return UncompressedPixelFormat{filament::Texture::PixelBufferDescriptor::PixelDataFormat::RGB, filament::Texture::PixelBufferDescriptor::PixelDataType::HALF };

		case Image::FORMAT_RGBAH:
			return UncompressedPixelFormat{filament::Texture::PixelBufferDescriptor::PixelDataFormat::RGBA, filament::Texture::PixelBufferDescriptor::PixelDataType::HALF };

		// FORMAT_RGBE9995 - not representable

		case Image::FORMAT_DXT1:
			return CompressedPixelFormat{ filament::Texture::CompressedType::DXT1_RGBA };

		case Image::FORMAT_DXT3:
			return CompressedPixelFormat{ filament::Texture::CompressedType::DXT3_RGBA };

		case Image::FORMAT_DXT5:
		case Image::FORMAT_DXT5_RA_AS_RG:
			return CompressedPixelFormat{ filament::Texture::CompressedType::DXT5_RGBA };

		case Image::FORMAT_RGTC_R:
			return CompressedPixelFormat{ filament::Texture::CompressedType::RED_RGTC1 };

		case Image::FORMAT_RGTC_RG:
			return CompressedPixelFormat{ filament::Texture::CompressedType::RED_GREEN_RGTC2 };

		case Image::FORMAT_BPTC_RGBA:
			return CompressedPixelFormat{ filament::Texture::CompressedType::RGBA_BPTC_UNORM };

		case Image::FORMAT_BPTC_RGBF:
			return CompressedPixelFormat{ filament::Texture::CompressedType::RGB_BPTC_SIGNED_FLOAT };

		case Image::FORMAT_BPTC_RGBFU:
			return CompressedPixelFormat{ filament::Texture::CompressedType::RGB_BPTC_UNSIGNED_FLOAT };

		case Image::FORMAT_ETC:
		case Image::FORMAT_ETC2_RGB8:
			/*
			 * Filament has no separate enumeration for ETC1, but ETC2
			 * decoders can read ETC1.
			 */
			return CompressedPixelFormat{ filament::Texture::CompressedType::ETC2_RGB8 };

		case Image::FORMAT_ETC2_R11:
			return CompressedPixelFormat{ filament::Texture::CompressedType::EAC_R11 };

		case Image::FORMAT_ETC2_R11S:
			return CompressedPixelFormat{ filament::Texture::CompressedType::EAC_R11_SIGNED };

		case Image::FORMAT_ETC2_RG11:
			return CompressedPixelFormat{ filament::Texture::CompressedType::EAC_RG11 };

		case Image::FORMAT_ETC2_RG11S:
			return CompressedPixelFormat{ filament::Texture::CompressedType::EAC_RG11_SIGNED };

		case Image::FORMAT_ETC2_RGBA8:
		case Image::FORMAT_ETC2_RA_AS_RG:
			return CompressedPixelFormat{ filament::Texture::CompressedType::ETC2_EAC_RGBA8 };

		case Image::FORMAT_ETC2_RGB8A1:
			return CompressedPixelFormat{ filament::Texture::CompressedType::ETC2_RGB8_A1 };

		case Image::FORMAT_ASTC_4x4:
			return CompressedPixelFormat{ filament::Texture::CompressedType::RGBA_ASTC_4x4 };

		case Image::FORMAT_ASTC_8x8:
			return CompressedPixelFormat{ filament::Texture::CompressedType::RGBA_ASTC_8x8 };

		default:
			throw std::runtime_error("FilamentTextureObject::mapImageFormat: unsupported image format");
	}
}
