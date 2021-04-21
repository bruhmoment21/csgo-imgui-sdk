#pragma once

#include "signatures.hpp"

class c_key_values
{
public:
	~c_key_values();

	void init(const char* key_name)
	{
		signatures::key_values::fn_init(this, key_name);
	}

	void load_from_buffer(const char* resource_name, const char* buf, void* file_sys = nullptr, const char* path_id = nullptr, void* eval_sym_proc = nullptr, void* unk = nullptr)
	{
		signatures::key_values::fn_load_from_buffer(this, resource_name, buf, file_sys, path_id, eval_sym_proc, unk);
	}

	c_key_values* find_key(const char* name, bool create = false)
	{
		return signatures::key_values::fn_find_key(this, name, create);
	}

	void set_int(const char* name, int value)
	{
		const auto key = find_key(name, true);
		if (!key) return;

		*reinterpret_cast<int*>(key + 12) = value;
		*reinterpret_cast<char*>(key + 16) = 2; // 2 == TYPE_INT
	}

	void set_string(const char* name, const char* a2)
	{
		const auto key = find_key(name, true);
		if (!key) return;

		signatures::key_values::fn_set_string(key, a2);
	}

	int get_int(const char* name, int default_value)
	{
		return signatures::key_values::fn_get_int(this, name, default_value);
	}
	
	const char* get_string(const char* name, const char* default_value)
	{
		return signatures::key_values::fn_get_string(this, name, default_value);
	}
	
	// Pass sdk::file_system as first argument and "MOD" for path_id. (used for dumping)
	bool save_to_file(void* file_system, const char* file_name, const char* path_id, bool write_empty_sub_keys = false)
	{
		return signatures::key_values::fn_save_to_file(this, file_system, file_name, path_id, write_empty_sub_keys);
	}
};