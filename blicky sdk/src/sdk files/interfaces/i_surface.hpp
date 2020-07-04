#pragma once

class i_surface {
public:
	void unlock_cursor( ) {
		using original_fn = void( __thiscall* )( i_surface* );
		return ( *reinterpret_cast< original_fn** >( this ) )[ 66 ]( this );
	}
};