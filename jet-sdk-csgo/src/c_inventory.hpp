#pragma once

#include "utilities.hpp"
#include "signatures.hpp"

#include "c_econ.hpp"
#include "c_utl_vector.hpp"

class c_shared_object_type_cache
{
public:
	void add_object(void* obj)
	{
		return utilities::call_virtual< void, 1, void* >(this, obj);
	}
	void remove_object(void* obj)
	{
		return utilities::call_virtual< void, 3, void* >(this, obj);
	}

	auto get_econ_items()
	{
		auto ret = std::vector< c_econ_item* >{ };
		const auto size = *reinterpret_cast<std::size_t*>(this + 0x18);
		auto* const data = *reinterpret_cast<std::uintptr_t**>(this + 0x4);

		for (std::size_t i = 0; i < size; ++i)
		{
			ret.emplace_back(reinterpret_cast<c_econ_item*>(data[i]));
		}

		return ret;
	}
};

class c_player_inventory
{
public:
	c_econ_item_view* get_item_in_loadout(int team, int slot)
	{
		return utilities::call_virtual<c_econ_item_view*, 8, int, int>(this, team, slot);
	}

	bool add_econ_item(c_econ_item* item, int a3, int a4, char a5)
	{
		get_base_type_cache()->add_object(item);

		if (!signatures::fn_add_econ_item(this, item, a3, a4, a5))
			return false;

		auto* const i = get_inventory_item_by_item_id(item->get_item_id());
		if (!i) return false;

		i->clear_inventory_image_rgba();

		return true;
	}

	c_shared_object_type_cache* get_base_type_cache()
	{
		const auto so_cache = signatures::fn_find_so_cache(signatures::client_system + 0x70, *reinterpret_cast<std::uint64_t*>(this + 8), *reinterpret_cast<std::uint64_t*>(this + 0x10), 1);
		return signatures::fn_create_base_type_cache(so_cache, 1);
	}

	c_econ_item_view* get_inventory_item_by_item_id(std::int64_t id)
	{
		return signatures::fn_get_inventory_item_by_item_id(this, id);
	}

	std::uint32_t get_steam_id()
	{
		return *reinterpret_cast<std::uint32_t*>(this + 0x8);
	}

	c_utl_vector<c_econ_item_view*>* get_inventory_items()
	{
		return reinterpret_cast<c_utl_vector<c_econ_item_view*>*>(this + 0x2C);
	}

	c_econ_item_view* find_key_to_open(c_econ_item_view* crate)
	{
		for (int i = 0; i < this->get_inventory_items()->size; ++i)
		{
			auto* const prop_key = *(this->get_inventory_items()->memory + i);
			if (prop_key)
			{
				if (prop_key->tool_can_apply_to(crate))
				{
					return prop_key;
				}
			}
		}

		return nullptr;
	}

	void remove_item_by_id(std::uint64_t id)
	{
		return utilities::call_virtual< void, 15, std::uint64_t >(this, id);
	}

	void remove_item(c_econ_item_view* item)
	{
		const auto econ_item = item->get_soc_data();

		remove_item_by_id(econ_item->get_item_id());
		get_base_type_cache()->remove_object(econ_item);

		econ_item->destroy(); // "55 8B EC 56 8B F1 8B 4E 18 C7 06 ? ? ? ? C7 46 ? ? ? ? ? 85 C9 74 05"
	}

	auto get_last_values_ids()
	{
		std::uint64_t max_id = 1;
		std::uint32_t max_inv_id = 1;

		for (const auto& i : get_base_type_cache()->get_econ_items())
		{
			const bool is_default_item = static_cast<std::uint32_t>(i->get_item_id() >> 32) == 0xF0000000;
			if (!is_default_item)
			{
				if (i->get_item_id() > max_id)
				{
					max_id = i->get_item_id();
				}

				if (i->get_inventory() > max_inv_id)
				{
					max_inv_id = i->get_inventory();
				}
			}
		}

		return std::make_pair(max_id, max_inv_id);
	}
};

class cs_inventory_manager
{
public:
	c_player_inventory* get_local_player_inventory()
	{
		return *reinterpret_cast<c_player_inventory**>(this + signatures::local_inventory_offset);
	}

	// "55 8B EC 83 E4 F8 83 EC 24 83 3D ? ? ? ? ? 53 56 57 8B F9"
	bool equip_item_in_loadout(int team, int slot, std::uint64_t item_id) // T = 2; CT = 3
	{
		return utilities::call_virtual< bool, 20, int, int, std::uint64_t, bool >(this, team, slot, item_id, true);
	}
};