#include "filament_ubershader_variant.h"

FilamentUbershaderVariant::FilamentUbershaderVariant(const std::string &name, std::vector<glshader::process::definition> &&definitions) :
	m_name(name), m_definitions(std::move(definitions)) {

}

FilamentUbershaderVariant::~FilamentUbershaderVariant() = default;

FilamentUbershaderVariant::FilamentUbershaderVariant(FilamentUbershaderVariant &&other) noexcept = default;

FilamentUbershaderVariant &FilamentUbershaderVariant::operator =(FilamentUbershaderVariant &&other) noexcept = default;
