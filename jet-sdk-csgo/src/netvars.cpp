#include "pch.hpp"
#include "netvars.hpp"

#include "utilities.hpp"
#include "hooks.hpp"
#include "sdk.hpp"

using netvar_key_value_map = std::unordered_map< std::uint32_t, std::uintptr_t >;
using netvar_table_map = std::unordered_map< std::uint32_t, netvar_key_value_map >;

static void add_props_for_table(netvar_table_map& table_map, const std::uint32_t table_name_hash, const recv_table_t* const table, const std::size_t child_offset = 0) noexcept
{
	for (int i = 0; i < table->count; ++i)
	{
		const auto& prop = &table->props[i];
		if (fnv::equal_hashes(prop->name, "baseclass") || isdigit(prop->name[0])) continue;

		if (prop->type == 6 && prop->data_table && prop->data_table->table_name[0] == 'D')
		{
			add_props_for_table(table_map, table_name_hash, prop->data_table, prop->offset + child_offset);
		}

		if (fnv::equal_hashes(prop->name, "m_nSequence") && table_name_hash == fnv::hash("DT_BaseViewModel"))
		{
			hooks::original::sequence = std::make_unique< recv_prop_hook_t >(prop, hooks::sequence);
		}

		const auto& offset = static_cast<std::uintptr_t>(prop->offset) + child_offset;
		table_map[table_name_hash].emplace(fnv::hash(prop->name), offset);
	}
}

static void initialize_props(netvar_table_map& table_map) noexcept
{
	for (auto* client_class = sdk::client->get_all_classes(); client_class; client_class = client_class->next)
	{
		const auto& table = client_class->recv_table;
		if (!table) continue;

		add_props_for_table(table_map, fnv::hash(table->table_name), table);
	}
}

namespace netvar_manager
{
	std::uintptr_t get_offset(const std::uint32_t table, const std::uint32_t prop) noexcept
	{
		static netvar_table_map map;
		if (map.empty()) initialize_props(map);

		if (map.find(table) == map.cend()) return 0;

		const netvar_key_value_map& table_map = map.at(table);
		if (table_map.find(prop) == table_map.cend()) return 0;

		return table_map.at(prop);
	}
}