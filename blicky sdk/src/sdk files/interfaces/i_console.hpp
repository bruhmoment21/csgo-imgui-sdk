#pragma once

#include "../classes/convar.hpp"

struct console_color {
private:
	unsigned char rgba[ 4 ];
	typedef const unsigned char color;
public:
	console_color( color& r, color& g, color& b, color& a = 255 ) {
		rgba[ 0 ] = r;
		rgba[ 1 ] = g;
		rgba[ 2 ] = b;
		rgba[ 3 ] = a;
	}

	static console_color blue( color& a = 255 ) {
		return { 0, 160, 255, a };
	}

	static console_color green( color& a = 255 ) {
		return { 0, 255, 0, a };
	}

	static console_color white( color& a = 255 ) {
		return { 255, 255, 255, a };
	}

	static console_color yellow( color& a = 255 ) {
		return { 255, 255, 0, a };
	}

	static console_color red( color& a = 255 ) {
		return { 255, 0, 0, a };
	}
};

class i_console {
public:
	convar* get_convar( const char* name ) {
		return virtual_method::call_virtual< convar*, 15, const char* >( this, name );
	}

	template <typename ...values>
	void printf_color( const console_color& color, const char* message, values... parameters ) {
		return ( ( *reinterpret_cast< void( *** )( void*, const console_color&, const char*, ... ) >( this ) )[ 25 ] )( this, color, message, parameters... );
	}
};