#pragma once

#include "utilities.hpp"
#include "c_entity.hpp"

class i_entity_list
{
public:
	entity_t* get_client_entity(int index)
	{
		return utilities::call_virtual< entity_t*, 3, int >(this, index);
	}

	entity_t* get_client_entity_handle(int handle)
	{
		return utilities::call_virtual< entity_t*, 4, int >(this, handle);
	}

	int get_highest_index()
	{
		return utilities::call_virtual< int, 6 >(this);
	}
};