#include "filament/filament_builtin_materials.h"

namespace FilamentBuiltinMaterials {


	const unsigned char default_canvas_item[] = {
#include "materials/default_canvas_item.mat.gen.h"
	};

	const size_t default_canvas_item_size = sizeof(default_canvas_item);

	const unsigned char fallback_lit[] = {
#include "materials/fallback_lit.mat.gen.h"
	};

	const size_t fallback_lit_size = sizeof(fallback_lit);

}

