#pragma once

#include "../classes/convar.hpp"

class i_console {
public:
	convar* get_convar( const char* name ) {
		return virtual_method::call_virtual< convar*, 15, const char* >( this, name );
	}
};