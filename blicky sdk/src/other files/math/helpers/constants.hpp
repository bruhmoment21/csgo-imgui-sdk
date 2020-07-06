#pragma once
#include <cstdint>

class constants {
public:
	static constexpr double		   PI						 = 3.141592;
	static constexpr double		   RADIAN					 = 57.28578;
	static constexpr float		   PI_FLOAT					 = static_cast< float >( PI );
	static constexpr std::uint32_t FLOAT32_NAN_BITS			 = 0x7FC00000;
	
	static constexpr float RAD2DEG( const float x ) {
		return static_cast< float >( x ) * static_cast< float >( 180.f / PI_FLOAT );
	}

	static constexpr float DEG2RAD( const float x ) {
		return static_cast< float >( x ) * static_cast< float >( PI_FLOAT / 180.f );
	}
};