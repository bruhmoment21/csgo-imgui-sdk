#pragma once
#include "helpers/vec3_t.hpp"

struct ImVec2;

namespace math {
	bool world_to_screen(const vec3_t& origin, ImVec2& screen);
}