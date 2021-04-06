#pragma once

#include "c_utl_vector.hpp"

class game_event_descriptor_t
{
public:
	int	event_id;
	int element_index;
	void* keys;
	c_utl_vector<void*> listeners;
};

class i_game_event
{
public:
	virtual ~i_game_event() = 0;
	virtual const char* get_name() const = 0;

	virtual bool is_reliable() const = 0;
	virtual bool is_local() const = 0;
	virtual bool is_empty(const char* key_name = nullptr) = 0;

	virtual bool get_bool(const char* key_name = nullptr, bool default_value = false) = 0;
	virtual int get_int(const char* key_name = nullptr, int default_value = 0) = 0;
	virtual std::uint64_t get_uint_64(const char* key_name = nullptr, unsigned long default_value = 0) = 0;
	virtual float get_float(const char* key_name = nullptr, float default_value = 0.f) = 0;
	virtual const char* get_string(const char* key_name = nullptr, const char* default_value = "") = 0;
	virtual const wchar_t* get_wstring(const char* key_name, const wchar_t* default_value = L"") = 0;

	virtual void pad_function_0() = 0;
	virtual void set_bool(const char* key_name, bool value) = 0;
	virtual void set_int(const char* key_name, int value) = 0;
	virtual void set_uint_64(const char* key_name, unsigned long value) = 0;
	virtual void set_float(const char* key_name, float value) = 0;
	virtual void set_string(const char* key_name, const char* value) = 0;
	virtual void set_wstring(const char* key_name, const wchar_t* value) = 0;
};

class i_game_event_listener
{
public:
	virtual ~i_game_event_listener()
	{}
	virtual void fire_game_event(i_game_event* event) = 0;
	virtual int get_event_debug_id()
	{
		return 42;
	};
};

class i_game_event_manager
{
public:
	virtual ~i_game_event_manager() = 0;
	virtual int load_events_from_file(const char* filename) = 0;
	virtual void reset() = 0;
	virtual bool add_listener(i_game_event_listener* listener, const char* name, bool server_side = false) = 0;
	virtual bool find_listener(i_game_event_listener* listener, const char* name) = 0;
	virtual int remove_listener(i_game_event_listener* listener) = 0;
	virtual i_game_event* create_event(const char* name, bool force, unsigned int unknown) = 0;
	virtual bool fire_event(i_game_event* event, bool dont_broadcast = false) = 0;
	virtual bool fire_event_client_side(i_game_event* event) = 0;
	virtual i_game_event* duplicate_event(i_game_event* event) = 0;
	virtual void free_event(i_game_event* event) = 0;
};