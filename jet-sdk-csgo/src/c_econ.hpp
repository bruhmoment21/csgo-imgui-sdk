#pragma once

#include "sdk.hpp"
#include "signatures.hpp"

#include "c_utl_vector.hpp"

enum class item_rarity_t : int;
enum class item_quality_t : int;

class item_schema_t;
class c_econ_item;

struct attribute_info_t
{
	short id;
	int value;

	bool operator ==(const short other) const noexcept
	{
		return id == other;
	}
};

class c_econ_item_definition
{
public:
	const char* get_item_type()
	{
		return *reinterpret_cast<const char**>(this + 0x54);
	}

	const char* get_model_name()
	{
		return *reinterpret_cast<const char**>(this + 0x94);
	}

	const char* get_world_model_name()
	{
		return *reinterpret_cast<const char**>(this + 0x9C);
	}

	const char* get_localize_name()
	{
		const auto token = *reinterpret_cast<const char**>(this + 0x4C);
		return sdk::localize->find_as_utf(token);
	}

	const char* get_item_name()
	{
		return *reinterpret_cast<const char**>(this + 0x1DC);
	}

	const char* get_inventory_image()
	{
		return reinterpret_cast<const char*>((*(int(__thiscall**)(void*))(*reinterpret_cast<unsigned long*>(this) + 0x14))(this));
	}

	int get_rarity()
	{
		return *reinterpret_cast<char*>(this + 0x2A);
	}

	int get_equipped_position()
	{
		return *reinterpret_cast<int*>(this + 0x268);
	}

	int get_supported_sticker_slots()
	{
		return *reinterpret_cast<int*>(this + 0x108);
	}

	short get_definition_index()
	{
		return *reinterpret_cast<short*>(this + 0x8);
	}

	auto get_attributes()
	{
		auto attributes = std::vector< attribute_info_t >{ };

		const auto size = *reinterpret_cast<int*>(this + 0x3C);
		const auto data = *reinterpret_cast<std::uintptr_t*>(this + 0x30);

		if (data)
		{
			for (int i = 0; i < size; ++i)
			{
				const auto id = *reinterpret_cast<std::int16_t*>(data + (i * 0xC));
				const auto value = *reinterpret_cast<std::int32_t*>(data + (i * 0xC) + 0x4);
				attributes.emplace_back(id, value);
			}
		}

		return attributes;
	}

	auto get_associated_items()
	{
		auto items = std::vector< std::uint16_t >{ };

		const auto size = *reinterpret_cast<int*>(this + 0x18);
		const auto data = *reinterpret_cast<uintptr_t*>(this + 0xC);

		if (data)
		{
			for (int i = 0; i < size; ++i)
			{
				items.emplace_back(*reinterpret_cast<std::uint16_t*>(data + (i * sizeof std::uint16_t)));
			}
		}

		return items;
	}
};

class c_econ_paint_kit_definition
{
public:
	const char* get_skin_localize_name()
	{
		const auto token = *reinterpret_cast<const char**>(this + 0x24);
		return sdk::localize->find_as_utf(token);
	}

	int get_paint_kit()
	{
		return *reinterpret_cast<int*>(this);
	}

	int get_rarity()
	{
		return *reinterpret_cast<int*>(this + 0x78);
	}

	const char* get_skin_name()
	{
		return *reinterpret_cast<const char**>(this + 0x4);
	}
};

class c_econ_sticker_definition // aka CStickerKit
{
public:
	const char* get_sticker_localize_name()
	{
		const auto token = *reinterpret_cast<const char**>(this + 0x28);
		return sdk::localize->find_as_utf(token);
	}

	const char* get_name()
	{
		return *reinterpret_cast<const char**>(this + 0x28);
	}

	const char* get_sticker_material()
	{
		return *reinterpret_cast<const char**>(this + 0x38);
	}

	int get_sticker_id()
	{
		return *reinterpret_cast<int*>(this);
	}

	int get_rarity()
	{
		return *reinterpret_cast<int*>(this + 0x4);
	}

	int get_team_id()
	{
		return *reinterpret_cast<int*>(this + 0x6C);
	}

	int get_event_id()
	{
		return *reinterpret_cast<int*>(this + 0x68);
	}

	int get_player_id()
	{
		return *reinterpret_cast<int*>(this + 0x70);
	}
};

class item_schema_t
{
public:
	const char* get_crate_series_by_id(int index)
	{		
		const auto id = signatures::fn_get_crate_series_by_id(reinterpret_cast<std::uintptr_t>(this) + 0x17C, &index);
		if (id == -1) return nullptr;

		const auto v11 = *reinterpret_cast<unsigned long*>(this + 0x180) + 24 * id;
		return *reinterpret_cast<const char**>(v11 + 0x14);
	}

	std::size_t get_item_definition_count()
	{
		return *reinterpret_cast<std::size_t*>(this + 0xE8);
	}

	c_econ_item_definition* get_item_definition(std::size_t index)
	{
		return *reinterpret_cast<c_econ_item_definition**>(*reinterpret_cast<std::uintptr_t*>(this + 0xD4) + 12 * index + 0x4);
	}

	std::size_t get_paint_kit_definition_count()
	{
		return *reinterpret_cast<std::size_t*>(this + 0x2A8);
	}

	c_econ_paint_kit_definition* get_paint_kit_definition(std::size_t index)
	{
		return *reinterpret_cast<c_econ_paint_kit_definition**>(*reinterpret_cast<std::uintptr_t*>(this + 0x290) + 24 * index + 0x14);
	}

	std::size_t get_sticker_definition_count()
	{
		return *reinterpret_cast<std::size_t*>(this + 0x2CC);
	}

	c_econ_sticker_definition* get_sticker_definition(std::size_t index)
	{
		return *reinterpret_cast<c_econ_sticker_definition**>(*reinterpret_cast<std::uintptr_t*>(this + 0x2B4) + 24 * index + 0x14);
	}

	c_econ_sticker_definition* get_sticker_by_skin_index(std::int32_t index)
	{
		for (std::size_t i = 0; i <= get_sticker_definition_count(); ++i)
		{
			auto item = get_sticker_definition(i);
			if (item->get_sticker_id() == index)
			{
				return item;
			}
		}
		return nullptr;
	}

	c_econ_paint_kit_definition* get_paint_kit_by_skin_index(std::int32_t index)
	{
		for (std::size_t i = 0; i <= get_paint_kit_definition_count(); ++i)
		{
			auto item = get_paint_kit_definition(i);
			if (item->get_paint_kit() == index)
			{
				return item;
			}
		}
		return nullptr;
	}

	c_econ_item_definition* get_item_by_definition_index(std::int32_t index)
	{
		for (std::size_t i = 0; i < get_item_definition_count(); ++i)
		{
			auto item = get_item_definition(i);
			if (item->get_definition_index() == index)
			{
				return item;
			}
		}
		return nullptr;
	}

	std::uintptr_t get_attribute_by_index(std::size_t index)
	{
		return *reinterpret_cast<std::uintptr_t*>(*reinterpret_cast<std::uintptr_t*>(this + 0x120) + 4 * index);
	}

	std::uintptr_t get_loot_list_interface_by_name(const char* name, int unk) // "55 8B EC 83 E4 F8 8B 55 08 81 EC ? ? ? ? 56 57"
	{
		return utilities::call_virtual< std::uintptr_t, 31, const char*, int >(this, name, unk);
	}
	
	std::uintptr_t get_loot_list_interface_by_index(int index) // "55 8B EC 83 E4 F8 8B 55 08 81 EC ? ? ? ? 56 57"
	{
		return utilities::call_virtual< std::uintptr_t, 32, int >(this, index);
	}
};

class c_econ_item_view
{
public:
	void clear_inventory_image_rgba()
	{
		signatures::fn_clear_inventory_image_rgba(this);
	}

	c_econ_item* get_soc_data()
	{
		return signatures::fn_get_soc_data(this);
	}

	std::uint32_t& get_inventory()
	{
		return *reinterpret_cast<std::uint32_t*>(this + signatures::inventory_offset);
	}

	c_econ_item_definition* get_static_data()
	{
		return signatures::fn_get_static_data(this);
	}

	const char* get_crate_series()
	{
		const auto& attributes_crate = this->get_static_data()->get_attributes();

		const auto supply_create_series = std::find(attributes_crate.cbegin(), attributes_crate.cend(), 68);
		if (supply_create_series != attributes_crate.cend())
		{
			return signatures::fn_get_item_schema()->get_crate_series_by_id(supply_create_series->value);
		}

		return nullptr;
	}

	bool tool_can_apply_to(c_econ_item_view* item)
	{
		bool ret_val;

		__asm {
			mov eax, this
			add eax, 0xC
			mov ecx, eax
			mov eax, item
			add eax, 0xC
			mov edx, eax
			push 0x4
			call signatures::fn_tool_can_apply_to
			mov ret_val, al
			add esp, 4
		};

		return ret_val;
	}
};

class c_econ_item
{
public:
	static c_econ_item* create_econ_item()
	{
		return signatures::fn_create_econ_item();
	}

	void destroy()
	{
		return utilities::call_virtual< void, 0, bool >(this, false);
	}

	std::uint64_t& get_item_id()
	{
		return *reinterpret_cast<std::uint64_t*>(this + 0x8);
	}

	std::uint32_t& get_inventory()
	{
		return *reinterpret_cast<std::uint32_t*>(this + 0x20);
	}

	std::uint32_t& get_account_id()
	{
		return *reinterpret_cast<std::uint32_t*>(this + 0x1C);
	}

	std::uint64_t& get_original_id()
	{
		return *reinterpret_cast<std::uint64_t*>(this + 0x10);
	}

	std::uint8_t& get_flags()
	{
		return *reinterpret_cast<std::uint8_t*>(this + 0x30);
	}

	std::uint16_t& get_definition_index()
	{
		return *reinterpret_cast<std::uint16_t*>(this + 0x24);
	}

	void set_attribute_value(int index, void* val)
	{
		const auto attribute = signatures::fn_get_item_schema()->get_attribute_by_index(index);
		signatures::fn_set_attribute_value(this, attribute, val);
	}

	void set_paint_kit(float kit)
	{
		set_attribute_value(6, &kit);
	}

	void set_paint_seed(float seed)
	{
		set_attribute_value(7, &seed);
	}

	void set_stattrak(int val)
	{
		int zero = 0;
		set_attribute_value(80, &val);
		set_attribute_value(81, &zero);
	}

	void set_paint_wear(float wear)
	{
		set_attribute_value(8, &wear);
	}

	void add_sticker(int index, int kit, float wear, float scale, float rotation)
	{
		set_attribute_value(113 + 4 * index, &kit);
		set_attribute_value(114 + 4 * index, &wear);
		set_attribute_value(115 + 4 * index, &scale);
		set_attribute_value(116 + 4 * index, &rotation);
	}

	std::uint16_t& get_econ_item_data()
	{
		return *reinterpret_cast<std::uint16_t*>(this + 0x26);
	}

	void set_quality(item_quality_t quality)
	{
		const auto data = get_econ_item_data();
		get_econ_item_data() = data ^ (data ^ 32 * int(quality)) & 0x1E0;
	}

	void set_rarity(item_rarity_t rarity)
	{
		const auto data = get_econ_item_data();
		get_econ_item_data() = (data ^ (int(rarity) << 11)) & 0x7800 ^ data;
	}

	void set_custom_name(const char* name)
	{
		signatures::fn_set_custom_name(this, name);
	}
};

struct weapon_drop_info_t
{
	short definition_index;
	int paint_kit;
	int rarity;
	int sticker_id;

	static void fn_recursive_add_loot_to_loot_list(item_schema_t* const item_schema, std::uintptr_t a1, std::vector< weapon_drop_info_t >& drop) noexcept
	{
		auto size = *(unsigned long*) ((*(int(__thiscall**)(int))(*(unsigned long*) a1 + 4))(a1) + 12);
		
		auto v9 = 0;
		auto v8 = 0u;
		do
		{
			auto v4 = (unsigned long*) (*(int(__thiscall**)(int))(*(unsigned long*) a1 + 4))(a1);
			auto v5 = v9 + *v4;

			if (*reinterpret_cast<std::uint8_t*>(v5 + 24))
			{
				auto v7 = item_schema->get_loot_list_interface_by_index(*reinterpret_cast<unsigned long*>(v5));
				fn_recursive_add_loot_to_loot_list(item_schema, v7, drop);
			} else
			{
				const auto item_definition = *reinterpret_cast<int*>(v5);
				const auto paint_kit = *reinterpret_cast<int*>(v5 + 0x4);
				const auto sticker_kit = *reinterpret_cast<int*>(v5 + 0x10);

				if (item_definition)
				{
					auto rarity = 0;

					if (sticker_kit)
					{
						rarity = signatures::fn_get_item_schema()->get_sticker_by_skin_index(sticker_kit)->get_rarity();
					} else
					{
						const auto item_def = signatures::fn_get_item_schema()->get_item_by_definition_index(item_definition);
						const auto skin_def = !paint_kit ? nullptr : signatures::fn_get_item_schema()->get_paint_kit_by_skin_index(paint_kit);
						const auto item_rarity = item_def ? item_def->get_rarity() : 0;
						const auto skin_rarity = skin_def ? skin_def->get_rarity() : 0;

						const auto skin_rarity_fixed = (skin_rarity == 7) + 6;
						rarity = item_rarity + skin_rarity - 1;

						if (rarity >= 0)
						{
							if (rarity > skin_rarity_fixed)
							{
								rarity = skin_rarity_fixed;
							}
						} else
						{
							rarity = 0;
						}
					}

					drop.emplace_back(item_definition, paint_kit, rarity, sticker_kit);
				}
			}

			v9 += 28;
			++v8;
		} while (v8 < size);
	}

	static auto get_weapons_for_crate(const char* name)
	{
		auto drop = std::vector< weapon_drop_info_t >{ };
		auto* const item_schema = signatures::fn_get_item_schema() + 4;

		fn_recursive_add_loot_to_loot_list(item_schema, item_schema->get_loot_list_interface_by_name(name, 0), drop);

		return drop;
	}
};