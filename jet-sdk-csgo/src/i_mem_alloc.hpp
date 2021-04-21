#pragma once

#include "utilities.hpp"

class i_mem_alloc
{
public:
	void* alloc(std::uint32_t size)
	{
		return utilities::call_virtual<void*, 1, std::uint32_t>(this, size);
	}

	void* realloc(void* memory, std::uint32_t size)
	{
		return utilities::call_virtual<void*, 3, void*, std::uint32_t>(this, memory, size);
	}

	void free(void* memory)
	{
		utilities::call_virtual<void, 5, void*>(this, memory);
	}
};