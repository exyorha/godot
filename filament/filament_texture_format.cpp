#include "filament/filament_texture_format.h"

FilamentTextureFormat::FilamentTextureFormat(filament::Texture::InternalFormat format) :
	m_format(format),
	m_swizzleR(filament::Texture::Swizzle::CHANNEL_0),
	m_swizzleG(filament::Texture::Swizzle::CHANNEL_1),
	m_swizzleB(filament::Texture::Swizzle::CHANNEL_2),
	m_swizzleA(filament::Texture::Swizzle::CHANNEL_3) {

}

FilamentTextureFormat::FilamentTextureFormat(filament::Texture::InternalFormat format,
											 filament::Texture::Swizzle swizzleR,
											 filament::Texture::Swizzle swizzleG,
											 filament::Texture::Swizzle swizzleB,
											 filament::Texture::Swizzle swizzleA) :
	m_format(format),
	m_swizzleR(swizzleR),
	m_swizzleG(swizzleG),
	m_swizzleB(swizzleB),
	m_swizzleA(swizzleA) {

}

FilamentTextureFormat::FilamentTextureFormat(Image::Format format) : FilamentTextureFormat(mapTextureFormat(format)) {

}

FilamentTextureFormat::~FilamentTextureFormat() = default;


FilamentTextureFormat::FilamentTextureFormat(const FilamentTextureFormat &other) = default;

FilamentTextureFormat &FilamentTextureFormat::operator =(const FilamentTextureFormat &other) = default;

FilamentTextureFormat::FilamentTextureFormat(FilamentTextureFormat &&other) noexcept = default;

FilamentTextureFormat &FilamentTextureFormat::operator =(FilamentTextureFormat &&other) noexcept = default;

bool FilamentTextureFormat::operator ==(const FilamentTextureFormat &other) const {
	return m_format == other.m_format && m_swizzleR == other.m_swizzleR && m_swizzleG == other.m_swizzleG &&
		m_swizzleB == other.m_swizzleB && m_swizzleA == other.m_swizzleA;
}

bool FilamentTextureFormat::operator !=(const FilamentTextureFormat &other) const {
	return m_format != other.m_format || m_swizzleR != other.m_swizzleR || m_swizzleG != other.m_swizzleG ||
		m_swizzleB != other.m_swizzleB || m_swizzleA != other.m_swizzleA;
}

FilamentTextureFormat FilamentTextureFormat::mapTextureFormat(Image::Format format) {
	switch(format) {
		case Image::FORMAT_L8:
			return FilamentTextureFormat(filament::Texture::InternalFormat::R8,
										 filament::Texture::Swizzle::CHANNEL_0,
										 filament::Texture::Swizzle::CHANNEL_0,
										 filament::Texture::Swizzle::CHANNEL_0,
										 filament::Texture::Swizzle::SUBSTITUTE_ONE);

		case Image::FORMAT_LA8:
			return FilamentTextureFormat(filament::Texture::InternalFormat::RG8,
										 filament::Texture::Swizzle::CHANNEL_0,
										 filament::Texture::Swizzle::CHANNEL_0,
										 filament::Texture::Swizzle::CHANNEL_0,
										 filament::Texture::Swizzle::CHANNEL_1);

		case Image::FORMAT_R8:
			return FilamentTextureFormat(filament::Texture::InternalFormat::R8);

		case Image::FORMAT_RG8:
			return FilamentTextureFormat(filament::Texture::InternalFormat::RG8);

		case Image::FORMAT_RGB8:
			return FilamentTextureFormat(filament::Texture::InternalFormat::RGB8);

		case Image::FORMAT_RGBA8:
			return FilamentTextureFormat(filament::Texture::InternalFormat::RGBA8);

		case Image::FORMAT_RGBA4444:
			return FilamentTextureFormat(filament::Texture::InternalFormat::RGBA4);

		case Image::FORMAT_RGB565:
			return FilamentTextureFormat(filament::Texture::InternalFormat::RGB565);

		case Image::FORMAT_RF:
			return FilamentTextureFormat(filament::Texture::InternalFormat::R32F);

		case Image::FORMAT_RGF:
			return FilamentTextureFormat(filament::Texture::InternalFormat::RG32F);

		case Image::FORMAT_RGBF:
			return FilamentTextureFormat(filament::Texture::InternalFormat::RGB32F);

		case Image::FORMAT_RGBAF:
			return FilamentTextureFormat(filament::Texture::InternalFormat::RGBA32F);

		case Image::FORMAT_RH:
			return FilamentTextureFormat(filament::Texture::InternalFormat::R16F);

		case Image::FORMAT_RGH:
			return FilamentTextureFormat(filament::Texture::InternalFormat::RG16F);

		case Image::FORMAT_RGBH:
			return FilamentTextureFormat(filament::Texture::InternalFormat::RGB16F);

		case Image::FORMAT_RGBAH:
			return FilamentTextureFormat(filament::Texture::InternalFormat::RGBA16F);

		case Image::FORMAT_RGBE9995:
			return FilamentTextureFormat(filament::Texture::InternalFormat::RGB9_E5);

		case Image::FORMAT_DXT1:
			return FilamentTextureFormat(filament::Texture::InternalFormat::DXT1_RGBA);

		case Image::FORMAT_DXT3:
			return FilamentTextureFormat(filament::Texture::InternalFormat::DXT3_RGBA);

		case Image::FORMAT_DXT5:
			return FilamentTextureFormat(filament::Texture::InternalFormat::DXT5_RGBA);

		case Image::FORMAT_RGTC_R:
			return FilamentTextureFormat(filament::Texture::InternalFormat::RED_RGTC1);

		case Image::FORMAT_RGTC_RG:
			return FilamentTextureFormat(filament::Texture::InternalFormat::RED_GREEN_RGTC2);

		case Image::FORMAT_BPTC_RGBA:
			return FilamentTextureFormat(filament::Texture::InternalFormat::RGBA_BPTC_UNORM);

		case Image::FORMAT_BPTC_RGBF:
			return FilamentTextureFormat(filament::Texture::InternalFormat::RGB_BPTC_SIGNED_FLOAT);

		case Image::FORMAT_BPTC_RGBFU:
			return FilamentTextureFormat(filament::Texture::InternalFormat::RGB_BPTC_UNSIGNED_FLOAT);

		case Image::FORMAT_ETC:
			/*
			 * Filament doesn't have an internal format for ETC1, however,
			 * ETC1 can be read by an ETC2 decoder.
			 */
			return FilamentTextureFormat(filament::Texture::InternalFormat::ETC2_RGB8);

		case Image::FORMAT_ETC2_R11:
			return FilamentTextureFormat(filament::Texture::InternalFormat::EAC_R11);

		case Image::FORMAT_ETC2_R11S:
			return FilamentTextureFormat(filament::Texture::InternalFormat::EAC_R11_SIGNED);

		case Image::FORMAT_ETC2_RG11:
			return FilamentTextureFormat(filament::Texture::InternalFormat::EAC_RG11);

		case Image::FORMAT_ETC2_RG11S:
			return FilamentTextureFormat(filament::Texture::InternalFormat::EAC_RG11_SIGNED);

		case Image::FORMAT_ETC2_RGB8:
			return FilamentTextureFormat(filament::Texture::InternalFormat::ETC2_RGB8);

		case Image::FORMAT_ETC2_RGBA8:
			return FilamentTextureFormat(filament::Texture::InternalFormat::ETC2_EAC_RGBA8);

		case Image::FORMAT_ETC2_RGB8A1:
			return FilamentTextureFormat(filament::Texture::InternalFormat::ETC2_RGB8_A1);

		case Image::FORMAT_ETC2_RA_AS_RG:
			return FilamentTextureFormat(filament::Texture::InternalFormat::ETC2_EAC_RGBA8,
										 filament::Texture::Swizzle::CHANNEL_0,
										 filament::Texture::Swizzle::CHANNEL_3,
										 filament::Texture::Swizzle::SUBSTITUTE_ZERO,
										 filament::Texture::Swizzle::SUBSTITUTE_ONE);

		case Image::FORMAT_DXT5_RA_AS_RG:
			return FilamentTextureFormat(filament::Texture::InternalFormat::DXT5_RGBA,
										 filament::Texture::Swizzle::CHANNEL_0,
										 filament::Texture::Swizzle::CHANNEL_3,
										 filament::Texture::Swizzle::SUBSTITUTE_ZERO,
										 filament::Texture::Swizzle::SUBSTITUTE_ONE);

		case Image::FORMAT_ASTC_4x4:
			return FilamentTextureFormat(filament::Texture::InternalFormat::RGBA_ASTC_4x4);

		case Image::FORMAT_ASTC_8x8:
			return FilamentTextureFormat(filament::Texture::InternalFormat::RGBA_ASTC_8x8);

		case Image::FORMAT_ASTC_4x4_HDR:
		case Image::FORMAT_ASTC_8x8_HDR:
			throw std::runtime_error("HDR ASTC formats are not supported by Filament");

		default:
			throw std::runtime_error("Unsupported texture format");
	}
}

bool FilamentTextureFormat::usesSwizzle() const {
	return m_swizzleR != filament::Texture::Swizzle::CHANNEL_0 ||
		m_swizzleG != filament::Texture::Swizzle::CHANNEL_1 ||
		m_swizzleB != filament::Texture::Swizzle::CHANNEL_2 ||
		m_swizzleA != filament::Texture::Swizzle::CHANNEL_3;
}

bool FilamentTextureFormat::isSupported(filament::Engine &engine) const {
	return filament::Texture::isTextureFormatSupported(engine, format()) && (!usesSwizzle() || filament::Texture::isTextureSwizzleSupported(engine));
}
