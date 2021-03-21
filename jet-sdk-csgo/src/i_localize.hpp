#pragma once

#include "utilities.hpp"

class i_localize
{
public:
	const char* find_as_utf(const char* token)
	{
		return utilities::call_virtual<const char*, 47>(this, token);
	}
};