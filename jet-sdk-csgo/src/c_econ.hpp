#pragma once

#include "signatures.hpp"
#include "sdk.hpp"

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

struct item_list_entry_t
{
	short definition_index; 	// 0x0 		0
	int paint_kit;				// 0x4 		4
	int rarity;					// 0x8		8 - USED IN ANOTHER SCOPE
	float paint_kit_wear;		// 0xC 		12 - UNUSED
	int sticker_id;				// 0x10 	16
	int music_kit;				// 0x14 	20
	bool nested_list;			// 0x18 	24
	bool unusual_list;			// 0x1C 	28 - UNUSED
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
		const auto token = *reinterpret_cast<const char**>(this + 0x4C); // item_name
		return sdk::localize->find_as_utf(token);
	}

	const char* get_item_name() // name
	{
		return *reinterpret_cast<const char**>(this + 0x1DC);
	}

	const char* get_inventory_image() // image_inventory
	{
		return *reinterpret_cast<const char**>(this + 0x6C);
	}

	int get_rarity()
	{
		return *reinterpret_cast<char*>(this + 0x2A);
	}

	int get_equipped_position() // get_loadout_slot
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
		return sdk::localize->find_as_utf(get_item_name());
	}

	const char* get_name()
	{
		return *reinterpret_cast<const char**>(this + 0x8);
	}

	const char* get_description_string()
	{
		return *reinterpret_cast<const char**>(this + 0x18);
	}

	const char* get_item_name()
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

class c_econ_music_definition
{
public:
	short music_id;
	char* name;
	char* localized_name;
	char* localized_description;
	char* pedestal_display_model;
	char* image_inventory;

	const char* get_music_kit_localize_name()
	{
		return sdk::localize->find_as_utf(localized_name);
	}
};

class c_econ_loot_list_definition
{
public:
	const char* get_name()
	{
		return utilities::call_virtual<const char*, 0>(this);
	}

	c_utl_vector<item_list_entry_t> get_loot_list_contents()
	{
		return utilities::call_virtual<const c_utl_vector<item_list_entry_t>&, 1>(this);
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

	c_econ_loot_list_definition* get_loot_list_interface_by_name(const char* name) // "55 8B EC 83 E4 F8 8B 55 08 81 EC ? ? ? ? 56 57"
	{
		return utilities::call_virtual< c_econ_loot_list_definition*, 31, const char*, int >(this, name, 0);
	}

	c_econ_loot_list_definition* get_loot_list_interface_by_index(int index)
	{
		return utilities::call_virtual< c_econ_loot_list_definition*, 32, int >(this, index);
	}

	c_econ_item_definition* get_item_by_name(const char* name)
	{
		return utilities::call_virtual< c_econ_item_definition*, 42, const char* >(this, name);
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

	void set_stattrak(int value)
	{
		if (value < 0 || get_definition_index() > 4500) return;

		int music_kit = get_definition_index() == 1314;
		set_attribute_value(80, &value);
		set_attribute_value(81, &music_kit);
	}

	void set_paint_wear(float wear)
	{
		set_attribute_value(8, &wear);
	}

	void set_sticker(int index, int kit, float wear, float scale, float rotation)
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

	void set_origin(const int origin)
	{
		const auto data = get_econ_item_data();
		get_econ_item_data() = data ^ (static_cast<std::uint8_t>(data) ^ static_cast<std::uint8_t>(origin)) & 0x1F;
	}

	void set_level(const int level)
	{
		const auto data = get_econ_item_data();
		get_econ_item_data() = data ^ (data ^ (level << 9)) & 0x600;
	}

	void set_custom_name(const char* name)
	{
		signatures::fn_set_custom_name(this, name);
	}
};

namespace crate_lootlist
{
	void initialize() noexcept;
	void reserve_crates(const std::size_t size) noexcept;
	std::vector< item_list_entry_t > get_weapons(const char* lootlist_name) noexcept;
	bool has_more_than_one_drop(const char* lootlist_name) noexcept;
}