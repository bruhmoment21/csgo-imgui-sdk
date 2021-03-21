#pragma once

struct recv_prop_t;

struct recv_proxy_data_t
{
	const recv_prop_t* recv_prop;
	union
	{
		float _float;
		long _int;
		char* _string;
		void* data;
		void* vector;
		std::int64_t int64;
	} value;
};

using recv_proxy_t = std::add_pointer_t< void(recv_proxy_data_t&, void*, void*) >;

struct recv_prop_t
{
	char* name;
	int type;
	int flags;
	int string_buffer_size;
	int inside_array;
	const void* extra_data;
	recv_prop_t* array_prop;
	void* array_length_prop;
	recv_proxy_t proxy;
	void* data_table_proxy;
	struct recv_table_t* data_table;
	int offset;
	int element_stride;
	int element_count;
	const char* parent_array_prop_name;
};

struct recv_table_t
{
	recv_prop_t* props;
	int count;
	void* decoder;
	char* table_name;
	bool initialized;
	bool in_main_list;
};

class recv_prop_hook_t
{
public:
	recv_prop_hook_t(recv_prop_t* prop, const recv_proxy_t proxy_fn) : property(prop), original_proxy_fn(prop->proxy)
	{
		set_proxy_function(proxy_fn);
	}

	~recv_prop_hook_t()
	{
		this->property->proxy = original_proxy_fn;
	}

	auto get_original_function() const -> recv_proxy_t
	{
		return original_proxy_fn;
	}

	auto set_proxy_function(const recv_proxy_t proxy_fn) const -> void
	{
		this->property->proxy = proxy_fn;
	}

private:
	recv_prop_t* property;
	recv_proxy_t original_proxy_fn;
};