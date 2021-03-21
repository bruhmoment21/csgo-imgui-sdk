#pragma once

#include "c_convar.hpp"

class i_console
{
public:
	convar_t* get_convar(const char* name)
	{
		return utilities::call_virtual< convar_t*, 15, const char* >(this, name);
	}
};