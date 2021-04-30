#include "pch.hpp"
#include "math.hpp"

#include "imgui/imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui_internal.h"

static void sin_cos(const float r, float& s, float& c) noexcept
{
	s = sin(r);
	c = cos(r);
}

namespace math
{
	angle_t calculate_angle(const vec3_t& src, const vec3_t& dest) noexcept
	{
		const vec3_t delta{src.x - dest.x, src.y - dest.y, src.z - dest.z};

		const auto hyp = sqrtf(delta.x * delta.x + delta.y * delta.y);
		
		angle_t angles{atanf(delta.z / hyp), atanf(delta.y / delta.x), 0.f};
		angles *= constants_t::radian;

		if (delta.x >= 0.f)
		{
			angles.yaw += 180.0f;
		}

		return angles;
	}

	vec3_t angle_vectors(const angle_t& angles) noexcept
	{
		float sp, sy, cp, cy;

		sin_cos(constants_t::degree_to_radians(angles.pitch), sp, cp);
		sin_cos(constants_t::degree_to_radians(angles.yaw), sy, cy);

		return {cp * cy, cp * sy, -sp};
	}

	bool world_to_screen(const vec3_t& origin, ImVec2& screen) noexcept
	{
		const auto w = matrix.m[3][0] * origin.x + matrix.m[3][1] * origin.y + matrix.m[3][2] * origin.z + matrix.m[3][3];
		if (w < 0.001f) return false;

		screen = ImGui::GetIO().DisplaySize / 2.0f;
		screen.x *= 1.0f + (matrix.m[0][0] * origin.x + matrix.m[0][1] * origin.y + matrix.m[0][2] * origin.z + matrix.m[0][3]) / w;
		screen.y *= 1.0f - (matrix.m[1][0] * origin.x + matrix.m[1][1] * origin.y + matrix.m[1][2] * origin.z + matrix.m[1][3]) / w;

		screen = ImFloor(screen);

		return true;
	}
}