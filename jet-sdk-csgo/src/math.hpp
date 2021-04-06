#pragma once

class constants_t
{
public:
	static constexpr float pi = 3.14159265f;
	static constexpr float radian = pi / 180.f;

	inline static constexpr float degree_to_radians(const float angle) noexcept
	{
		return angle * radian;
	}
};

struct angle_t
{
	float pitch;	// x
	float yaw;		// y
	float roll;		// z

	inline angle_t& operator*= (const float scalar) noexcept
	{
		this->pitch *= scalar;
		this->yaw *= scalar;
		this->roll *= scalar;

		return *this;
	}
};

struct vec3_t
{
	float x, y, z;

	inline vec3_t operator+ (const vec3_t& rhs) const noexcept
	{
		return vec3_t{this->x + rhs.x, this->y + rhs.y, this->z + rhs.z};
	}

	inline vec3_t operator* (const vec3_t& rhs) const noexcept
	{
		return vec3_t{this->x * rhs.x, this->y * rhs.y, this->z * rhs.z};
	}
};

struct vec2_t
{
	float x, y;
};

namespace math
{
	angle_t calculate_angle(const vec3_t& src, const vec3_t& dest) noexcept;
	vec3_t angle_vectors(const angle_t& angles) noexcept;
}