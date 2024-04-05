#ifndef FILAMENT_UBERSHADER_VARIANT_H
#define FILAMENT_UBERSHADER_VARIANT_H

#include <string>
#include <vector>

#include <glsp/definition.hpp>

class FilamentUbershaderVariant {
public:
	FilamentUbershaderVariant(const std::string &name, std::vector<glshader::process::definition> &&definitions);
	~FilamentUbershaderVariant();

	FilamentUbershaderVariant(const FilamentUbershaderVariant &other) = delete;
	FilamentUbershaderVariant &operator =(const FilamentUbershaderVariant &other) = delete;

	FilamentUbershaderVariant(FilamentUbershaderVariant &&other) noexcept;
	FilamentUbershaderVariant &operator =(FilamentUbershaderVariant &&other) noexcept;

	inline const std::string &name() const {
		return m_name;
	}

	inline const std::vector<glshader::process::definition> &definitions() const {
		return m_definitions;
	}

private:
	std::string m_name;
	std::vector<glshader::process::definition> m_definitions;
};

#endif
