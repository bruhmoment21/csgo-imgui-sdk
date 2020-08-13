#pragma once

#include "../../other files/math/helpers/vec3_t.hpp"

class unknown; // angle_t or QAngle

class i_effects {
public:
	virtual ~i_effects( ) { };

	virtual void beam( const vec3_t& start, const vec3_t& end, int model_index,
					   int index, unsigned char frame_start, unsigned char frame_rate,
					   float life, unsigned char width, unsigned char end_width, unsigned char fade_length,
					   unsigned char noise, unsigned char red, unsigned char green,
					   unsigned char blue, unsigned char brightness, unsigned char speed ) = 0;

	//-----------------------------------------------------------------------------
	// Purpose: Emits smoke sprites.
	// Input  : origin - Where to emit the sprites.
	//			scale - Sprite scale * 10.
	//			framerate - Framerate at which to animate the smoke sprites.
	//-----------------------------------------------------------------------------
	virtual void smoke( const vec3_t& origin, int model_index, float scale, float framerate ) = 0;

	virtual void sparks( const vec3_t& position, int magnitude = 1, int trail_length = 1, const vec3_t* direction = NULL ) = 0;

	virtual void dust( const vec3_t& pos, const vec3_t& dir, float size, float speed ) = 0;

	virtual void muzzle_flash( const vec3_t& origin, const unknown& angles, float scale, int type ) = 0;

	// like ricochet, but no sound
	virtual void metal_sparks( const vec3_t& position, const vec3_t& direction ) = 0;

	virtual void energy_splash( const vec3_t& position, const vec3_t& direction, bool explosive = false ) = 0;

	virtual void ricochet( const vec3_t& position, const vec3_t& direction ) = 0;

	// FIXME: Should these methods remain in this interface? Or go in some 
	// other client-server neutral interface?
	virtual float time( ) = 0;
	virtual bool is_server( ) = 0;

	// Used by the playback system to suppress sounds
	virtual void suppress_effects_sounds( bool suppress ) = 0;
};