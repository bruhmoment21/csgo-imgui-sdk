#pragma once
#include "../virtual_method.hpp"

class i_surface { // don't bother using surface. try to use imgui as much as you can where you can(better performance).
public:
	void unlock_cursor( ) {
		return virtual_method::call_virtual< void, 66 >( this );
	}
};
