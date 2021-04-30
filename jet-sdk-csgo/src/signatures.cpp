#include "pch.hpp"
#include "signatures.hpp"

#include "netvars.hpp"

struct module_t
{
	module_t() = default;

	explicit module_t(const char* module_name)
	{
		if (!module_name) throw std::runtime_error{"module_name was nullptr"};

		const HMODULE handle = GetModuleHandleA(module_name);
		if (!handle) throw std::runtime_error{"[module_t] Couldn't get " + std::string{module_name} + " handle"};

		this->handle = handle;
		this->dos_headers = reinterpret_cast<PIMAGE_DOS_HEADER>(handle);
		this->nt_headers = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<std::byte*>(handle) + dos_headers->e_lfanew);
		this->size_of_image = nt_headers->OptionalHeader.SizeOfImage;
		this->scan_bytes = reinterpret_cast<std::uint8_t*>(handle);
	}

	inline bool operator!() const noexcept
	{
		return !handle;
	}

	HMODULE handle;
	PIMAGE_DOS_HEADER dos_headers;
	PIMAGE_NT_HEADERS nt_headers;
	DWORD size_of_image;
	std::uint8_t* scan_bytes;
};

static std::unordered_map<std::uint32_t, module_t> modules;

template <std::size_t size> // Thanks cristeigabriel!
static std::byte* pattern_scan(const char* module_name, const int(&pattern)[size])
{
	/*
	* Faster pattern scanning function. (Avoids heap allocations).
	*
	* Use 0 instead of '?' and '0xCC'
	* Add 0x before bytes
	*
	* Normal pattern scanning parameter : "80 BF ? ? ? ? ? 0F 84 ? ? ? ? 32 DB"
	* Faster pattern scanning parameter : {0x80, 0xBF, 0, 0, 0, 0, 0, 0x0F, 0x84, 0, 0, 0, 0, 0x32, 0xDB}
	*/

	const auto& current_module = [module_name]()
	{
		const auto& cached_module = modules[fnv::hash(module_name)];

		return !cached_module ? module_t{module_name} : cached_module;
	}();

	for (auto i = 0ul; i < current_module.size_of_image - size; ++i)
	{
		bool found = true;
		for (auto j = 0ul; j < size; ++j)
		{
			if (current_module.scan_bytes[i + j] != pattern[j] && pattern[j])
			{
				found = false;
				break;
			}
		}

		if (found)
		{
			const auto& result_address = reinterpret_cast<std::byte*>(&current_module.scan_bytes[i]);
#ifdef _DEBUG
			std::cout << '[' << module_name << ']';

			for (auto i = 0u; i < size; ++i)
			{
				if (pattern[i])
				{
					std::cout << ' ' << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << pattern[i];
				} else
				{
					std::cout << " ?";
				}
			}

			std::cout << " -> 0x" << result_address << '\n' << std::dec;
#endif
			return result_address;
		}
	}

	throw std::runtime_error{"Pattern with " + std::to_string(size) + " bytes is wrong!"};
}

static auto relative_to_absolute(const std::byte* const address) noexcept
{
	const auto new_address = reinterpret_cast<std::uintptr_t>(address);
	return new_address + 4 + *reinterpret_cast<int*>(new_address);
}

static auto relative_to_absolute(const std::uintptr_t address) noexcept
{
	return address + 4 + *reinterpret_cast<int*>(address);
}

namespace signatures
{
	void initialize()
	{
		panorama_marshall_helper = *reinterpret_cast<decltype(panorama_marshall_helper)*>(pattern_scan("client.dll",
			{0x68, 0, 0, 0, 0, 0x8B, 0xC8, 0xE8, 0, 0, 0, 0, 0x8D, 0x4D, 0xF4, 0xFF, 0x15, 0, 0, 0, 0, 0x8B, 0xCF, 0xFF, 0x15, 0, 0, 0, 0, 0x5F, 0x5E, 0x8B, 0xE5, 0x5D, 0xC3}) + 1);

		directx = **reinterpret_cast<decltype(directx)**>(pattern_scan("shaderapidx9.dll",
			{0xA1, 0, 0, 0, 0, 0x50, 0x8B, 0x08, 0xFF, 0x51, 0x0C}) + 1);

		match_assisted_accept = *reinterpret_cast<decltype(match_assisted_accept)*>(pattern_scan("client.dll",
			{0xC7, 0x45, 0, 0, 0, 0, 0, 0xC7, 0x45, 0, 0, 0, 0, 0, 0xC7, 0x45, 0, 0, 0, 0, 0, 0xC7, 0x45, 0, 0, 0, 0, 0, 0xC7, 0x45, 0, 0, 0, 0, 0, 0xC7, 0x45, 0, 0, 0, 0, 0, 0xC6, 0x45, 0xF4, 0, 0xC7, 0x45, 0, 0, 0, 0, 0, 0xE8, 0, 0, 0, 0, 0xA1, 0, 0, 0, 0, 0xA8, 0x01, 0x75, 0x39, 0x0F, 0x28, 0x05, 0, 0, 0, 0, 0x83, 0xC8, 0x01, 0xA3, 0, 0, 0, 0, 0x8A, 0x45, 0xFF, 0x0F, 0x11, 0x05, 0, 0, 0, 0, 0x68, 0, 0, 0, 0, 0x0F, 0x28, 0x05, 0, 0, 0, 0, 0xA2, 0, 0, 0, 0, 0x0F, 0x11, 0x05, 0, 0, 0, 0, 0xE8, 0, 0, 0, 0, 0x83, 0xC4, 0x04, 0x8D, 0x45, 0xD4, 0xB9, 0, 0, 0, 0, 0x50, 0x8D, 0x45, 0xFE, 0x50, 0xE8, 0, 0, 0, 0, 0xB8, 0, 0, 0, 0, 0x8B, 0xE5, 0x5D, 0xC2, 0x10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xA1, 0, 0, 0, 0, 0x85, 0xC0, 0x75, 0x2E}) + 3);

		delayed_lobby = reinterpret_cast<decltype(delayed_lobby)>(pattern_scan("client.dll",
			{0x63, 0x8B, 0x76, 0x24}) - 1);

		fake_prime = reinterpret_cast<decltype(fake_prime)>(pattern_scan("client.dll",
			{0x17, 0xF6, 0x40, 0x14, 0x10}) - 1);

		client_system = **reinterpret_cast<decltype(client_system)**>(pattern_scan("client.dll",
			{0x8B, 0x0D, 0, 0, 0, 0, 0x6A, 0, 0x83, 0xEC, 0x10}) + 2);

		local_inventory_offset = *reinterpret_cast<decltype(local_inventory_offset)*>(pattern_scan("client.dll",
			{0x8B, 0x8F, 0, 0, 0, 0, 0x0F, 0xB7, 0xC0, 0x50}) + 2);

		inventory_offset = *reinterpret_cast<decltype(inventory_offset)*>(pattern_scan("client.dll",
			{0x8D, 0x9E, 0, 0, 0, 0, 0x8B, 0x0B}) + 2);

		inventory_manager = *reinterpret_cast<decltype(inventory_manager)*>(pattern_scan("client.dll",
			{0xB9, 0, 0, 0, 0, 0x8D, 0x44, 0x24, 0x10, 0x89, 0x54, 0x24, 0x14}) + 1);

		failed_to_open_crate = *reinterpret_cast<decltype(failed_to_open_crate)*>(pattern_scan("client.dll",
			{0x68, 0, 0, 0, 0, 0x8B, 0x01, 0x8B, 0x80, 0, 0, 0, 0, 0xFF, 0xD0, 0x84, 0xC0, 0x74, 0x1A, 0x8B, 0x35, 0, 0, 0, 0, 0x8B, 0xD3, 0x33, 0xC9, 0x8B, 0x3E, 0xE8, 0, 0, 0, 0, 0x50, 0x8B, 0xCE, 0xFF, 0x97, 0, 0, 0, 0, 0x5F, 0x5E, 0xB0, 0x01, 0x5B, 0x8B, 0xE5, 0x5D, 0xC2, 0x04, 0}) + 1);

		item_customization_notification = *reinterpret_cast<decltype(item_customization_notification)*>(pattern_scan("client.dll",
			{0x68, 0, 0, 0, 0, 0x8B, 0x80, 0, 0, 0, 0, 0xFF, 0xD0, 0x84, 0xC0, 0x74, 0x28}) + 1);

		loot_list_items_count_return_address = reinterpret_cast<decltype(loot_list_items_count_return_address)>(pattern_scan("client.dll",
			{0x85, 0xC0, 0x0F, 0x84, 0, 0, 0, 0, 0x8B, 0xC8, 0xE8, 0, 0, 0, 0, 0x52, 0x50, 0xE8, 0, 0, 0, 0, 0x83, 0xC4, 0x08, 0x89, 0x44, 0x24, 0x14, 0x85, 0xC0, 0x0F, 0x84, 0, 0, 0, 0, 0x8B, 0x0D}));

		is_loadout_allowed = reinterpret_cast<decltype(is_loadout_allowed)>(pattern_scan("client.dll",
			{0x8B, 0x40, 0x6C, 0xFF, 0xD0, 0, 0, 0, 0, 0, 0, 0x5F}) + 5);

		accumulate_layers_return_address = reinterpret_cast<decltype(accumulate_layers_return_address)>(pattern_scan("client.dll",
			{0x84, 0xC0, 0x75, 0x0D, 0xF6, 0x87}));

		setup_velocity_return_address = reinterpret_cast<decltype(setup_velocity_return_address)>(pattern_scan("client.dll",
			{0x84, 0xC0, 0x75, 0x38, 0x8B, 0x0D, 0, 0, 0, 0, 0x8B, 0x01, 0x8B, 0x80, 0, 0, 0, 0, 0xFF, 0xD0}));

		auto* const temp = reinterpret_cast<std::uintptr_t*>(pattern_scan("client.dll", 
			{0xB9, 0, 0, 0, 0, 0xE8, 0, 0, 0, 0, 0x8B, 0x5D, 0x08}) + 1);

		hud = *temp;

		game_rules = *reinterpret_cast<decltype(game_rules)*>(pattern_scan("client.dll",
			{0x8B, 0x0D, 0, 0, 0, 0, 0x85, 0xC9, 0x74, 0x1A, 0x8A, 0x41, 0x7D}) + 2);

		player_resource = *reinterpret_cast<decltype(player_resource)*>(pattern_scan("client.dll",
			{0x8B, 0x3D, 0, 0, 0, 0, 0x85, 0xFF, 0x0F, 0x84, 0, 0, 0, 0, 0x81, 0xC7}) + 2);

		// Functions
		fn_add_econ_item = reinterpret_cast<decltype(fn_add_econ_item)>(pattern_scan("client.dll",
			{0x55, 0x8B, 0xEC, 0x83, 0xE4, 0xF8, 0xA1, 0, 0, 0, 0, 0x83, 0xEC, 0x14, 0x53, 0x56, 0x57, 0x8B, 0xF9, 0x8B, 0x08}));

		fn_get_item_schema = reinterpret_cast<decltype(fn_get_item_schema)>(pattern_scan("client.dll",
			{0xA1, 0, 0, 0, 0, 0x85, 0xC0, 0x75, 0x53}));

		fn_find_so_cache = reinterpret_cast<decltype(fn_find_so_cache)>(pattern_scan("client.dll",
			{0x55, 0x8B, 0xEC, 0x83, 0xE4, 0xF8, 0x83, 0xEC, 0x1C, 0x0F, 0x10, 0x45, 0x08}));

		fn_create_base_type_cache = reinterpret_cast<decltype(fn_create_base_type_cache)>(pattern_scan("client.dll",
			{0x55, 0x8B, 0xEC, 0x51, 0x53, 0x56, 0x8B, 0xD9, 0x8D, 0x45, 0x08}));

		fn_get_inventory_item_by_item_id = reinterpret_cast<decltype(fn_get_inventory_item_by_item_id)>(pattern_scan("client.dll",
			{0x55, 0x8B, 0xEC, 0x8B, 0x55, 0x08, 0x83, 0xEC, 0x10, 0x8B, 0xC2}));

		fn_get_crate_series_by_id = reinterpret_cast<decltype(fn_get_crate_series_by_id)>(pattern_scan("client.dll",
			{0x55, 0x8B, 0xEC, 0x8B, 0x45, 0x08, 0x56, 0x57, 0x8B, 0x30, 0x8B, 0x41, 0x10, 0x83, 0xF8, 0xFF, 0x74, 0x1E, 0x8B, 0x79, 0x04, 0x8D, 0x0C, 0x40, 0x8B, 0x54, 0xCF, 0x10, 0x3B, 0xD6, 0x7E, 0x05}));

		fn_update_inventory = reinterpret_cast<decltype(fn_update_inventory)>(relative_to_absolute(pattern_scan("client.dll",
			{0xE8, 0, 0, 0, 0, 0x83, 0xC4, 0x04, 0x80, 0xBF, 0, 0, 0, 0, 0, 0x74, 0x0C}) + 1));

		fn_clear_inventory_image_rgba = reinterpret_cast<decltype(fn_clear_inventory_image_rgba)>(pattern_scan("client.dll",
			{0x55, 0x8B, 0xEC, 0x81, 0xEC, 0, 0, 0, 0, 0x57, 0x8B, 0xF9, 0xC7, 0x47}));

		fn_get_soc_data = reinterpret_cast<decltype(fn_get_soc_data)>(pattern_scan("client.dll",
			{0x55, 0x8B, 0xEC, 0x83, 0xE4, 0xF0, 0x83, 0xEC, 0x18, 0x56, 0x8B, 0xF1, 0x57, 0x8B, 0x86}));

		fn_get_static_data = reinterpret_cast<decltype(fn_get_static_data)>(pattern_scan("client.dll",
			{0x55, 0x8B, 0xEC, 0x51, 0x53, 0x8B, 0xD9, 0x8B, 0x0D, 0, 0, 0, 0, 0x56, 0x57, 0x8B, 0xB9}));

		fn_tool_can_apply_to = reinterpret_cast<decltype(fn_tool_can_apply_to)>(pattern_scan("client.dll",
			{0x55, 0x8B, 0xEC, 0x83, 0xEC, 0x18, 0x53, 0x56, 0x8B, 0xF1, 0x57, 0x8B, 0xFA}));

		fn_create_econ_item = reinterpret_cast<decltype(fn_create_econ_item)>((pattern_scan("client.dll",
			{0xA1, 0, 0, 0, 0, 0x6A, 0x38, 0x8B, 0x08, 0x8B, 0x01, 0xFF, 0x50, 0x04, 0x85, 0xC0, 0x74, 0x07, 0x8B, 0xC8, 0xE9, 0, 0, 0, 0, 0x33, 0xC0, 0xC3, 0, 0, 0, 0, 0xA1, 0, 0, 0, 0})));

		fn_set_attribute_value = reinterpret_cast<decltype(fn_set_attribute_value)>(pattern_scan("client.dll",
			{0x55, 0x8B, 0xEC, 0x83, 0xE4, 0xF8, 0x83, 0xEC, 0x3C, 0x53, 0x8B, 0x5D, 0x08, 0x56, 0x57, 0x6A, 0}));

		fn_init_item_customization_notification_event = reinterpret_cast<decltype(fn_init_item_customization_notification_event)>(pattern_scan("client.dll",
			{0x55, 0x8B, 0xEC, 0xA1, 0, 0, 0, 0, 0x53, 0x56, 0x8B, 0xF1, 0x8B, 0xDA, 0x8B, 0x08, 0x57, 0x6A, 0x1C, 0x8B, 0x01, 0xFF, 0x50, 0x04, 0x8B, 0xF8, 0x85, 0xFF, 0x74, 0x48}));

		fn_initialize_attributes = reinterpret_cast<decltype(fn_initialize_attributes)>(pattern_scan("client.dll",
			{0x55, 0x8B, 0xEC, 0x83, 0xE4, 0xF8, 0x83, 0xEC, 0x0C, 0x53, 0x56, 0x8B, 0xF1, 0x8B, 0x86}));

		fn_equip_wearable = reinterpret_cast<decltype(fn_equip_wearable)>(pattern_scan("client.dll",
			{0x55, 0x8B, 0xEC, 0x83, 0xEC, 0x10, 0x53, 0x8B, 0x5D, 0x08, 0x57, 0x8B, 0xF9}));

		fn_get_econ_item_view = (reinterpret_cast<decltype(fn_get_econ_item_view)>(relative_to_absolute(pattern_scan("client.dll",
			{0xE8, 0, 0, 0, 0, 0x89, 0x44, 0x24, 0x54}) + 1)));

		fn_set_custom_name = reinterpret_cast<decltype(fn_set_custom_name)>(relative_to_absolute(pattern_scan("client.dll",
			{0xE8, 0, 0, 0, 0, 0x8B, 0x46, 0x78, 0xC1, 0xE8, 0x0A, 0xA8, 0x01, 0x74, 0x13, 0x8B, 0x46, 0x34}) + 1));

		fn_get_event_descriptor = reinterpret_cast<decltype(fn_get_event_descriptor)>(relative_to_absolute(pattern_scan("engine.dll",
			{0xE8, 0, 0, 0, 0, 0x8B, 0xD8, 0x85, 0xDB, 0x75, 0x27}) + 1));

		fn_perform_screen_overlay = reinterpret_cast<decltype(fn_perform_screen_overlay)>(pattern_scan("client.dll",
			{0x55, 0x8b, 0xec, 0x51, 0xa1, 0, 0, 0, 0, 0x53, 0x56, 0x8b, 0xd9}));

		fn_render_view = reinterpret_cast<decltype(fn_render_view)>(pattern_scan("client.dll",
			{0x55, 0x8B, 0xEC, 0x83, 0xE4, 0xC0, 0x81, 0xEC, 0, 0, 0, 0, 0x53, 0x56, 0x57, 0x8B, 0xF9, 0x89, 0x7C, 0x24, 0x44}));

		if (auto* const tier0_handle = GetModuleHandleA("tier0.dll"); tier0_handle)
		{
			fn_warning = GetProcAddress(tier0_handle, "Warning");
		}

		fn_find_hud_element = reinterpret_cast<decltype(fn_find_hud_element)>(relative_to_absolute(reinterpret_cast<std::uintptr_t>(temp) + 5));

		fn_clear_hud_weapon = reinterpret_cast<decltype(fn_clear_hud_weapon)>(pattern_scan("client.dll",
			{0x55, 0x8B, 0xEC, 0x51, 0x53, 0x56, 0x8B, 0x75, 0x08, 0x8B, 0xD9, 0x57, 0x6B, 0xFE, 0x2C}));

		{
			using namespace key_values;

			fn_init = reinterpret_cast<decltype(fn_init)>(pattern_scan("client.dll",
				{0x55, 0x8B, 0xEC, 0x51, 0x33, 0xC0, 0xC7, 0x45}));

			fn_load_from_buffer = reinterpret_cast<decltype(fn_load_from_buffer)>(pattern_scan("client.dll",
				{0x55, 0x8B, 0xEC, 0x83, 0xE4, 0xF8, 0x83, 0xEC, 0x34, 0x53, 0x8B, 0x5D, 0x0C, 0x89}));

			fn_find_key = reinterpret_cast<decltype(fn_find_key)>(pattern_scan("client.dll",
				{0x55, 0x8B, 0xEC, 0x83, 0xEC, 0x1C, 0x53, 0x8B, 0xD9, 0x85, 0xDB}));

			fn_set_string = reinterpret_cast<decltype(fn_set_string)>(pattern_scan("client.dll",
				{0x55, 0x8B, 0xEC, 0xA1, 0, 0, 0, 0, 0x53, 0x56, 0x57, 0x8B, 0xF9, 0x8B, 0x08, 0x8B, 0x01}));

			fn_get_int = reinterpret_cast<decltype(fn_get_int)>(pattern_scan("client.dll",
				{0x55, 0x8B, 0xEC, 0x6A, 0, 0xFF, 0x75, 0x08, 0xE8, 0, 0, 0, 0, 0x85, 0xC0, 0x74, 0x45}));

			fn_get_string = reinterpret_cast<decltype(fn_get_string)>(pattern_scan("client.dll",
				{0x55, 0x8B, 0xEC, 0x83, 0xE4, 0xC0, 0x81, 0xEC, 0, 0, 0, 0, 0x53, 0x8B, 0x5D, 0x08}));

			fn_save_to_file = reinterpret_cast<decltype(fn_save_to_file)>(relative_to_absolute(pattern_scan("client.dll",
				{0xE8, 0, 0, 0, 0, 0x8B, 0x8E, 0, 0, 0, 0, 0x8D, 0x45, 0xBC}) + 1));
		}

		fn_get_sequence_activity = reinterpret_cast<decltype(fn_get_sequence_activity)>(pattern_scan("client.dll",
			{0x55, 0x8B, 0xEC, 0x53, 0x8B, 0x5D, 0x08, 0x56, 0x8B, 0xF1, 0x83}));

		fn_static_prop_init = reinterpret_cast<decltype(fn_static_prop_init)>(relative_to_absolute(pattern_scan("engine.dll",
			{0xE8, 0, 0, 0, 0, 0x8B, 0x0F, 0x03, 0xCE}) + 1));

		fn_get_color_modulation = reinterpret_cast<decltype(fn_get_color_modulation)>(pattern_scan("materialsystem.dll",
			{0x55, 0x8B, 0xEC, 0x83, 0xEC, 0, 0x56, 0x8B, 0xF1, 0x8A, 0x46}));

		fn_pre_render_3d_skybox_world = reinterpret_cast<decltype(fn_pre_render_3d_skybox_world)>(pattern_scan("client.dll",
			{0x83, 0xF9, 0x01, 0x8B, 0x0D, 0, 0, 0, 0}));

		fn_set_skybox = reinterpret_cast<decltype(fn_set_skybox)>(pattern_scan("engine.dll",
			{0x55, 0x8B, 0xEC, 0x81, 0xEC, 0, 0, 0, 0, 0x56, 0x57, 0x8B, 0xF9, 0xC7, 0x45}));
	}
}