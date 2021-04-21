#pragma once

using fn_create_interface_t = void* (const char*, int*);

struct app_system_info_t
{
	const char* module_name;
	const char* interface_name;
};

enum class init_return_val_t : int
{
	INIT_FAILED = 0,
	INIT_OK,

	INIT_LAST_VAL,
};

enum class app_system_tier_t : int
{
	APP_SYSTEM_TIER0 = 0,
	APP_SYSTEM_TIER1,
	APP_SYSTEM_TIER2,
	APP_SYSTEM_TIER3,

	APP_SYSTEM_TIER_OTHER,
};

class i_app_system
{
public:
	virtual bool connect(fn_create_interface_t factory) = 0;
	virtual void disconnect() = 0;
	virtual void* query_interface(const char* interface_name) = 0;
	virtual init_return_val_t init() = 0;
	virtual void shutdown() = 0;
	virtual const app_system_info_t* get_dependencies() = 0;
	virtual app_system_tier_t get_tir() = 0;
	virtual void reconnect(fn_create_interface_t factory, const char* interface_name) = 0;
	virtual bool is_singleton() = 0;
};