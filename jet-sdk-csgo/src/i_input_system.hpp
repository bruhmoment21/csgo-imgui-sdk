#pragma once

#include "utilities.hpp"

class i_input_system
{
public:
	void enable_input(const bool enable)
	{
		return utilities::call_virtual<void, 11, bool>(this, enable);
	}
};