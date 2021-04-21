#pragma once

#include "utilities.hpp"

class i_base_file_system
{
public:
	int read(void* output, int size, void* handle)
	{
		return utilities::call_virtual<int, 0, void*, int, void*>(this, output, size, handle);
	}

	void* open(const char* file_name, const char* options, const char* path_id)
	{
		return utilities::call_virtual<void*, 2, const char*, const char*, const char*>(this, file_name, options, path_id);
	}

	void close(void* handle)
	{
		return utilities::call_virtual<void, 3, void*>(this, handle);
	}

	std::uint32_t size(void* handle)
	{
		return utilities::call_virtual<std::uint32_t, 7, void*>(this, handle);
	}

	bool file_exists(const char* file_name, const char* path_id)
	{
		return utilities::call_virtual<bool, 11, const char*, const char*>(this, file_name, path_id);
	}
};