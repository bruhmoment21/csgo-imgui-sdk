#pragma once
#include "utilities.hpp"

class iv_model_info
{
public:
	void* get_model(int index)
	{
		return utilities::call_virtual< void*, 1, int >(this, index);
	}

	int get_model_index(const char* file_name)
	{
		return utilities::call_virtual< int, 2, const char* >(this, file_name);
	}

	const char* get_model_name(const void* model)
	{
		return utilities::call_virtual< const char*, 3, const void* >(this, model);
	}

	void* get_studio_model(const void* model)
	{
		return utilities::call_virtual< void*, 32, const void* >(this, model);
	}
};