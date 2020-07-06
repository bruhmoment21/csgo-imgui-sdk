#pragma once

class i_surface { // don't bother using surface. try to use imgui as much as you can where you can(better performance).
public:
	void unlock_cursor( ) {
		using original_fn = void( __thiscall* )( i_surface* );
		return ( *reinterpret_cast< original_fn** >( this ) )[ 66 ]( this );
	}
};
