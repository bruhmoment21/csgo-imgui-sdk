#pragma once

struct matrix_3x4;
struct ImVec2;

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

struct vec4_t
{
	float x, y, z, w;
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

	inline float dot_product(const vec3_t& rhs) const noexcept
	{
		return this->x * rhs.x + this->y * rhs.y + this->z * rhs.z;
	}
};

struct vec2_t
{
	float x, y;
};

struct matrix_3x4
{
	explicit matrix_3x4(const vec3_t& x_axis, const vec3_t& y_axis, const vec3_t& z_axis, const vec3_t& origin_vector) noexcept
	{
		m[0][0] = x_axis.x; m[0][1] = y_axis.x; m[0][2] = z_axis.x; m[0][3] = origin_vector.x;
		m[1][0] = x_axis.y; m[1][1] = y_axis.y; m[1][2] = z_axis.y; m[1][3] = origin_vector.y;
		m[2][0] = x_axis.z; m[2][1] = y_axis.z; m[2][2] = z_axis.z; m[2][3] = origin_vector.z;
	}

	float m[3][4];
};

struct matrix_4x4 // view_matrix
{
	matrix_4x4() = default;

	explicit matrix_4x4(const matrix_3x4& matrix) noexcept
	{
		m[0][0] = matrix.m[0][0]; m[0][1] = matrix.m[0][1]; m[0][2] = matrix.m[0][2]; m[0][3] = matrix.m[0][3];
		m[1][0] = matrix.m[1][0]; m[1][1] = matrix.m[1][1]; m[1][2] = matrix.m[1][2]; m[1][3] = matrix.m[1][3];
		m[2][0] = matrix.m[2][0]; m[2][1] = matrix.m[2][1]; m[2][2] = matrix.m[2][2]; m[2][3] = matrix.m[2][3];

		m[3][0] = 0.f; m[3][1] = 0.f; m[3][2] = 0.f; m[3][3] = 1.f;
	}

	float m[4][4];
};

namespace math
{
	angle_t calculate_angle(const vec3_t& src, const vec3_t& dest) noexcept;
	vec3_t angle_vectors(const angle_t& angles) noexcept;
	bool world_to_screen(const vec3_t& origin, ImVec2& screen) noexcept;

	inline matrix_4x4 matrix; // updated in frame stage notify
}