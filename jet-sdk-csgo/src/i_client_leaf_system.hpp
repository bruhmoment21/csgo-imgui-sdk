#pragma once

#include "utilities.hpp"

class i_client_leaf_system
{
public:
	void create_renderable_handle(void* obj)
	{
		return utilities::call_virtual< void, 0, std::uintptr_t, bool, int, int, std::uint32_t >(this, reinterpret_cast<std::uintptr_t>(obj) + 4, false, 0, -1, 0xFFFFFFFF);
	}
};