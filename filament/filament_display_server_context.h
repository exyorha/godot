#ifndef FILAMENT_DISPLAY_SERVER_CONTEXT_H
#define FILAMENT_DISPLAY_SERVER_CONTEXT_H

#include "core/typedefs.h"

namespace filament {
	class Engine;
}

class FilamentDisplayServerContext {
public:
	FilamentDisplayServerContext();
	~FilamentDisplayServerContext();

	FilamentDisplayServerContext(const FilamentDisplayServerContext &other) = delete;
	FilamentDisplayServerContext &operator =(const FilamentDisplayServerContext &other) = delete;

	Error initialize();

private:
	filament::Engine *m_engine;
};

#endif
