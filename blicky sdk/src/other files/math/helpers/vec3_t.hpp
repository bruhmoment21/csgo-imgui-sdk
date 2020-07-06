#pragma once
#include <cmath>

class vec3_t {
public:
	float x, y, z;

	explicit vec3_t( const float x = 0.f, const float y = 0.f, const float z = 0.f ) : x( x ), y( y ), z( z ) {

	}

	void set_value( const vec3_t& value ) {
		this->x = value.x;
		this->y = value.y;
		this->z = value.z;
	}

	// operations
	vec3_t operator+( const vec3_t& vector ) const {
		return vec3_t{ x + vector.x, y + vector.y, z + vector.z };
	}
	vec3_t operator-( const vec3_t& vector ) const {
		return vec3_t{ x - vector.x, y - vector.y, z - vector.z };
	}

	// formulas
	[[nodiscard]] float distance_2d( ) const {
		return std::sqrt( x * x + y * y );
	}
	[[nodiscard]] float distance( ) const {
		return std::sqrt( x * x + y * y + z * z );
	}
};