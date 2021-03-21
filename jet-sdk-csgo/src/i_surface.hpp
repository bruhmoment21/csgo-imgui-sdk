#pragma once

#include "utilities.hpp"

class i_surface
{
public:
	void unlock_cursor()
	{
		return utilities::call_virtual<void, 66>(this);
	}
};