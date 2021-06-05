#include "pch.hpp"
#include "c_econ.hpp"

#include "netvars.hpp"

static item_schema_t* item_schema;
static std::unordered_map<std::uint32_t, std::vector<item_list_entry_t>> lootlist_cache;

static void fn_recursive_add_loot_to_loot_list(c_econ_loot_list_definition* const loot_list_definition, std::vector<item_list_entry_t>& drop) noexcept
{
	if (!loot_list_definition) return;

	const auto& loot_list_items = loot_list_definition->get_loot_list_contents();
	for (int i = 0; i < loot_list_items.size; ++i)
	{
		const auto& item_list_entry = loot_list_items[i];
		if (item_list_entry.nested_list)
		{
			auto* const next_loot_list = item_schema->get_loot_list_interface_by_index(item_list_entry.definition_index);
			fn_recursive_add_loot_to_loot_list(next_loot_list, drop);
		} else
		{
			const auto& item_definition = item_list_entry.definition_index;

			if (item_definition)
			{
				const auto& paint_kit = item_list_entry.paint_kit;
				const auto& sticker_kit = item_list_entry.sticker_id;

				const auto rarity = [item_definition, paint_kit, sticker_kit]()
				{
					if (sticker_kit)
					{
						return signatures::fn_get_item_schema()->get_sticker_by_skin_index(sticker_kit)->get_rarity();
					} else
					{
						const auto item_def = signatures::fn_get_item_schema()->get_item_by_definition_index(item_definition);
						const auto skin_def = !paint_kit ? nullptr : signatures::fn_get_item_schema()->get_paint_kit_by_skin_index(paint_kit);
						const auto item_rarity = item_def ? item_def->get_rarity() : 0;
						const auto skin_rarity = skin_def ? skin_def->get_rarity() : 0;

						// Implement your rarity algorithm here. (instead of 6)
						return 6;
					}
				}();

				drop.emplace_back(item_definition, paint_kit, rarity, item_list_entry.paint_kit_wear, sticker_kit, item_list_entry.music_kit, false, false);
			}
		}
	}
}

static bool check_for_items(c_econ_loot_list_definition* const loot_list_definition) noexcept
{
	if (!loot_list_definition) return false;

	const auto& loot_list_items = loot_list_definition->get_loot_list_contents();
	for (int i = 0; i < loot_list_items.size; ++i)
	{
		const auto& item_list_entry = loot_list_items[i];
		if (item_list_entry.nested_list)
		{
			auto* const next_loot_list = item_schema->get_loot_list_interface_by_index(item_list_entry.definition_index);
			return check_for_items(next_loot_list);
		} else
		{
			if (item_list_entry.definition_index)
			{
				return true;
			}
		}
	}

	return false;
}

namespace crate_lootlist
{
	void initialize() noexcept
	{
		item_schema = signatures::fn_get_item_schema() + 4;
	}

	void reserve_crates(const std::size_t size) noexcept
	{
		lootlist_cache.reserve(size);
	}

	std::vector<item_list_entry_t> get_weapons(const char* lootlist_name) noexcept
	{
		const auto key = fnv::hash(lootlist_name);
		if (lootlist_cache.count(key))
		{
			return lootlist_cache.at(key);
		}

		fn_recursive_add_loot_to_loot_list(item_schema->get_loot_list_interface_by_name(lootlist_name), lootlist_cache[key]);

		return lootlist_cache[key];
	}

	bool has_more_than_one_drop(const char* lootlist_name) noexcept
	{
		const auto result = check_for_items(item_schema->get_loot_list_interface_by_name(lootlist_name));

		if (!result)
		{
			char fixed_name[64];
			sprintf_s(fixed_name, "%s_lootlist", lootlist_name);

			return check_for_items(item_schema->get_loot_list_interface_by_name(fixed_name));
		}

		return true;
	}
}