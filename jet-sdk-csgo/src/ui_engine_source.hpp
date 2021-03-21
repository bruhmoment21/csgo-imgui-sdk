#pragma once

#include "utilities.hpp"
#include "signatures.hpp"

class c_ui_engine
{
public:
	void dispatch_event(const std::uintptr_t event)
	{
		return utilities::call_virtual<void, 52, std::uintptr_t>(this, event);
	}

	bool broadcast_event(const std::uintptr_t event)
	{
		return utilities::call_virtual<bool, 54, std::uintptr_t>(this, event);
	}
};

class c_panorama_ui_engine
{
public:
	void update_inventory()
	{
		static const auto& address = signatures::update_inventory_address;
		static const auto fn_send = reinterpret_cast<int(__stdcall*)()>(*reinterpret_cast<std::uintptr_t*>(address + 1) + address + 5);

		fn_send();
	}

	c_ui_engine* access_ui_engine()
	{
		return utilities::call_virtual<c_ui_engine*, 11>(this);
	}
};