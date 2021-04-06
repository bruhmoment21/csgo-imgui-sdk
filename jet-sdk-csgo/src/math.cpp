#include "pch.hpp"
#include "math.hpp"

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
}