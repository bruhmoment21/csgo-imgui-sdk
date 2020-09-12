#pragma once

struct constants {
public:
	static constexpr double		   pi						{ 3.141592 };
	static constexpr double		   radian					{ 57.28578 };
	static constexpr float		   pi_float					{ static_cast< float >( pi ) };
	
	static constexpr float radians_to_degrees( const float& x ) {
		return x * 180.f / pi_float;
	}

	static constexpr float degrees_to_radians( const float& x ) {
		return x * pi_float / 180.f;
	}
};
