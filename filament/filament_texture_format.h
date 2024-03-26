#ifndef FILAMENT_TEXTURE_FORMAT_H
#define FILAMENT_TEXTURE_FORMAT_H

#include <filament/Texture.h>

#include <core/io/image.h>

namespace filament {
	class Engine;
}

class FilamentTextureFormat {
public:
	explicit FilamentTextureFormat(filament::Texture::InternalFormat format = filament::Texture::InternalFormat::RGBA8);
	explicit FilamentTextureFormat(filament::Texture::InternalFormat format,
								   filament::Texture::Swizzle swizzleR, filament::Texture::Swizzle swizzleG, filament::Texture::Swizzle swizzleB,
								   filament::Texture::Swizzle swizzleA);

	explicit FilamentTextureFormat(Image::Format format);
	~FilamentTextureFormat();

	FilamentTextureFormat(const FilamentTextureFormat &other);
	FilamentTextureFormat &operator =(const FilamentTextureFormat &other);

	FilamentTextureFormat(FilamentTextureFormat &&other) noexcept;
	FilamentTextureFormat &operator =(FilamentTextureFormat &&other) noexcept;

	inline filament::Texture::InternalFormat format() const {
		return m_format;
	}

	inline filament::Texture::Swizzle swizzleR() const {
		return m_swizzleR;
	}

	inline filament::Texture::Swizzle swizzleG() const {
		return m_swizzleG;
	}

	inline filament::Texture::Swizzle swizzleB() const {
		return m_swizzleB;
	}

	inline filament::Texture::Swizzle swizzleA() const {
		return m_swizzleA;
	}

	bool usesSwizzle() const;

	bool isSupported(filament::Engine &engine) const;

	bool operator ==(const FilamentTextureFormat &other) const;
	bool operator !=(const FilamentTextureFormat &other) const;

private:
	static FilamentTextureFormat mapTextureFormat(Image::Format format);

	filament::Texture::InternalFormat m_format;
	filament::Texture::Swizzle m_swizzleR;
	filament::Texture::Swizzle m_swizzleG;
	filament::Texture::Swizzle m_swizzleB;
	filament::Texture::Swizzle m_swizzleA;
};



#endif
