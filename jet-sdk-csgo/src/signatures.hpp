#pragma once

class game_rules_t;
class player_resource_t;

class c_econ_item;
class c_econ_item_view;
class c_econ_item_definition;

class item_schema_t;

class c_shared_object_type_cache;
class cs_inventory_manager;

class game_event_descriptor_t;
class i_game_event_manager;

class c_key_values;

namespace signatures
{
	void initialize();

	inline std::uintptr_t* panorama_marshall_helper; // "68 ? ? ? ? 8B C8 E8 ? ? ? ? 8D 4D F4 FF 15 ? ? ? ? 8B CF FF 15 ? ? ? ? 5F 5E 8B E5 5D C3" + 1
	inline IDirect3DDevice9* directx; // "A1 ? ? ? ? 50 8B 08 FF 51 0C" + 1

	using setup_event_fn = std::add_pointer_t<std::uintptr_t(int(***)())>; // use nullptr as parameter. add +3 as offset and then dereference
	inline setup_event_fn match_assisted_accept; // xref "MatchAssistedAccept"

	inline std::uint8_t* delayed_lobby; // https://www.unknowncheats.me/forum/counterstrike-global-offensive/443148-invite-cooldown-partly-clientsided.html
	inline std::uint8_t* fake_prime; // "17 F6 40 14 10" - 1
	inline std::uintptr_t client_system; // "8B 0D ? ? ? ? 6A 00 83 EC 10" + 2	
	inline std::uintptr_t local_inventory_offset; // "8B 8F ? ? ? ? 0F B7 C0 50" + 2
	inline std::uintptr_t inventory_offset; // "8D 9E ? ? ? ? 8B 0B" + 2
	inline cs_inventory_manager* inventory_manager; // "B9 ? ? ? ? 8D 44 24 10 89 54 24 14" + 1
	inline std::uintptr_t failed_to_open_crate; // "68 ? ? ? ? 8B 01 8B 80 ? ? ? ? FF D0 84 C0 74 1A 8B 35 ? ? ? ? 8B D3 33 C9 8B 3E E8 ? ? ? ? 50 8B CE FF 97 ? ? ? ? 5F 5E B0 01 5B 8B E5 5D C2 04 ?" + 1
	inline std::uintptr_t item_customization_notification; // "68 ? ? ? ? 8B 80 ? ? ? ? FF D0 84 C0 74 28" + 1
	inline std::byte* loot_list_items_count_return_address; // "85 C0 0F 84 ? ? ? ? 8B C8 E8 ? ? ? ? 52 50 E8 ? ? ? ? 83 C4 08 89 44 24 14 85 C0 0F 84 ? ? ? ? 8B 0D"
	inline std::uint8_t* is_loadout_allowed; // "8B 40 6C FF D0 ? ? ? ? ? ? 5F" + 5
	inline std::byte* accumulate_layers_return_address; // "84 C0 75 0D F6 87"
	inline std::byte* setup_velocity_return_address; // "84 C0 75 38 8B 0D ? ? ? ? 8B 01 8B 80 ? ? ? ? FF D0"
	inline std::uintptr_t hud; // "B9 ? ? ? ? E8 ? ? ? ? 8B 5D 08" + 1
	inline game_rules_t** game_rules; // "8B 0D ? ? ? ? 85 C9 74 1A 8A 41 7D" + 2
	inline player_resource_t** player_resource; // "8B 3D ? ? ? ? 85 FF 0F 84 ? ? ? ? 81 C7" + 2

	// Functions
	using fn_add_econ_item_t = bool(__thiscall*)(void*, c_econ_item*, int, int, char);
	inline fn_add_econ_item_t fn_add_econ_item; // "55 8B EC 83 E4 F8 A1 ? ? ? ? 83 EC 14 53 56 57 8B F9 8B 08"

	using fn_get_item_schema_t = std::add_pointer_t<item_schema_t* __stdcall()>;
	inline fn_get_item_schema_t fn_get_item_schema; // "A1 ? ? ? ? 85 C0 75 53"

	using fn_find_so_cache_t = std::uintptr_t(__thiscall*)(std::uintptr_t, std::uint64_t, std::uint64_t, bool);
	inline fn_find_so_cache_t fn_find_so_cache; // "55 8B EC 83 E4 F8 83 EC 1C 0F 10 45 08"

	using fn_create_base_type_cache_t = c_shared_object_type_cache * (__thiscall*)(std::uintptr_t, int);
	inline fn_create_base_type_cache_t fn_create_base_type_cache; // "55 8B EC 51 53 56 8B D9 8D 45 08"

	using fn_get_inventory_item_by_item_id_t = c_econ_item_view * (__thiscall*)(void*, std::int64_t);
	inline fn_get_inventory_item_by_item_id_t fn_get_inventory_item_by_item_id; // "55 8B EC 8B 55 08 83 EC 10 8B C2"

	using fn_get_crate_series_by_id_t = int(__thiscall*)(std::uintptr_t, int*);
	inline fn_get_crate_series_by_id_t fn_get_crate_series_by_id; // "55 8B EC 8B 45 08 56 57 8B 30 8B 41 10 83 F8 FF 74 1E 8B 79 04 8D 0C 40 8B 54 CF 10 3B D6 7E 05"

	using fn_update_inventory_t = int(__stdcall*)();
	inline fn_update_inventory_t fn_update_inventory; // "E8 ? ? ? ? 83 C4 04 80 BF ? ? ? ? ? 74 0C" + 1 [relative]

	using fn_clear_inventory_image_rgba_t = int* (__thiscall*)(void*);
	inline fn_clear_inventory_image_rgba_t fn_clear_inventory_image_rgba; // "55 8B EC 81 EC ? ? ? ? 57 8B F9 C7 47"

	using fn_get_soc_data_t = c_econ_item * (__thiscall*)(c_econ_item_view*);
	inline fn_get_soc_data_t fn_get_soc_data; // "55 8B EC 83 E4 F0 83 EC 18 56 8B F1 57 8B 86"

	using fn_get_static_data_t = c_econ_item_definition * (__thiscall*)(void*);
	inline fn_get_static_data_t fn_get_static_data; // "55 8B EC 51 53 8B D9 8B 0D ? ? ? ? 56 57 8B B9"

	// Doesn't need a typedef because we are calling it with assembly. c_econ_item_definition * (__thiscall*)(void*);
	inline void* fn_tool_can_apply_to; // "55 8B EC 83 EC 18 53 56 8B F1 57 8B FA"

	using fn_create_econ_item_t = c_econ_item * (__stdcall*)();
	inline fn_create_econ_item_t fn_create_econ_item; // "A1 ? ? ? ? 6A 38 8B 08 8B 01 FF 50 04 85 C0 74 07 8B C8 E9 ? ? ? ? 33 C0 C3 ? ? ? ? A1 ? ? ? ?"

	using fn_set_attribute_value_t = int(__thiscall*)(c_econ_item*, std::uintptr_t, void*);
	inline fn_set_attribute_value_t fn_set_attribute_value; // "55 8B EC 83 E4 F8 83 EC 3C 53 8B 5D 08 56 57 6A ?" 

	using fn_equip_wearable_t = int(__thiscall*)(void*, void*); // same for unequip
	inline fn_equip_wearable_t fn_equip_wearable; // "55 8B EC 83 EC 10 53 8B 5D 08 57 8B F9"

	// Doesn't need a typedef because we are calling it with assembly. std::uintptr_t(__fastcall*)(int, int, const char*, const char*);
	inline void* fn_init_item_customization_notification_event; // "55 8B EC A1 ? ? ? ? 53 56 8B F1 8B DA 8B 08 57 6A 1C 8B 01 FF 50 04 8B F8 85 FF 74 48"

	using fn_initialize_attributes_t = int(__thiscall*)(void*);
	inline fn_initialize_attributes_t fn_initialize_attributes; // "55 8B EC 83 E4 F8 83 EC 0C 53 56 8B F1 8B 86"

	using fn_get_econ_item_view_t = c_econ_item_view * (__thiscall*)(void*);
	inline fn_get_econ_item_view_t fn_get_econ_item_view; // "E8 ? ? ? ? 89 44 24 54" + 1 [relative]

	using fn_set_custom_name_t = c_econ_item * (__thiscall*)(void*, const char*);
	inline fn_set_custom_name_t fn_set_custom_name; // "E8 ? ? ? ? 8B 46 78 C1 E8 0A A8 01 74 13 8B 46 34" + 1 [relative]

	using fn_get_event_descriptor_t = game_event_descriptor_t * (__thiscall*)(i_game_event_manager*, const char*, int*);
	inline fn_get_event_descriptor_t fn_get_event_descriptor; // "E8 ? ? ? ? 8B D8 85 DB 75 27" + 1 [relative]

	inline void* fn_perform_screen_overlay; // "55 8b ec 51 a1 ? ? ? ? 53 56 8b d9"

	inline void* fn_render_view; // "55 8B EC 83 E4 C0 81 EC ? ? ? ? 53 56 57 8B F9 89 7C 24 44"

	inline void* fn_warning; // "Warning" exported in tier0.dll

	using fn_find_hud_element_t = int* (__thiscall*)(std::uintptr_t, const char*);
	inline fn_find_hud_element_t fn_find_hud_element; // temp + 5 [relative]

	using fn_clear_hud_weapon_t = int(__thiscall*)(int*, int);
	inline fn_clear_hud_weapon_t fn_clear_hud_weapon; // "55 8B EC 51 53 56 8B 75 08 8B D9 57 6B FE 2C"

	namespace key_values
	{
		using fn_init_t = void(__thiscall*)(void*, const char*);
		inline fn_init_t fn_init; // "55 8B EC 51 33 C0 C7 45"

		using fn_load_from_buffer_t = void(__thiscall*)(void*, const char*, const char*, void*, const char*, void*, void*);
		inline fn_load_from_buffer_t fn_load_from_buffer; // "55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89"

		using fn_find_key_t = c_key_values * (__thiscall*)(void*, const char*, bool);
		inline fn_find_key_t fn_find_key; // "55 8B EC 83 EC 1C 53 8B D9 85 DB"

		using fn_set_string_t = void(__thiscall*)(void*, const char*);
		inline fn_set_string_t fn_set_string; // "55 8B EC A1 ? ? ? ? 53 56 57 8B F9 8B 08 8B 01"

		using fn_get_int_t = int(__thiscall*)(void*, const char*, int);
		inline fn_get_int_t fn_get_int; // "55 8B EC 6A ? FF 75 08 E8 ? ? ? ? 85 C0 74 45"

		using fn_get_string_t = const char* (__thiscall*)(void*, const char*, const char*);
		inline fn_get_string_t fn_get_string; // "55 8B EC 83 E4 C0 81 EC ? ? ? ? 53 8B 5D 08"

		using fn_save_to_file_t = bool(__thiscall*)(void*, void*, const char*, const char*, bool);
		inline fn_save_to_file_t fn_save_to_file; // "E8 ? ? ? ? 8B 8E ? ? ? ? 8D 45 BC" + 1 [relative]
	}

	using fn_get_sequence_activity_t = int(__fastcall*)(void*, void*, int);
	inline fn_get_sequence_activity_t fn_get_sequence_activity; // "55 8B EC 53 8B 5D 08 56 8B F1 83"

	inline void* fn_static_prop_init; // E8 ? ? ? ? 8B 0F 03 CE + 1 [relative]

	inline void* fn_get_color_modulation; // "55 8B EC 83 EC ? 56 8B F1 8A 46"

	inline void* fn_pre_render_3d_skybox_world; // "83 F9 01 8B 0D ? ? ? ?"

	using fn_set_skybox_t = void(__fastcall*)(const char*);
	inline fn_set_skybox_t fn_set_skybox; // "55 8B EC 81 EC ? ? ? ? 56 57 8B F9 C7 45"
}