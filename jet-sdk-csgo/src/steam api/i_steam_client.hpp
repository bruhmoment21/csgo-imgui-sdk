#pragma once

#include "utilities.hpp"

class i_steam_client
{
public:
	void* get_steam_generic_interface(int steam_user, int steam_pipe, const char* version)
	{
		return utilities::call_virtual<void*, 12, int, int, const char*>(this, steam_user, steam_pipe, version);
	}
};