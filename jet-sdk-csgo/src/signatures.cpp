#include "pch.hpp"
#include "signatures.hpp"

template <std::size_t size> // How many bytes. You can get rid of the template by replacing std::array with std::vector but i don't want heap allocations.
static std::byte* pattern_scan(const char* module_name, const std::array<short, size>& pattern)
{
	/*
	* Faster pattern scanning function. (Avoids heap allocations).
	*
	* Use -1 instead of '?' and '0xCC'
	* Add 0x before bytes
	*
	* Normal pattern scanning parameter : "80 BF ? ? ? ? ? 0F 84 ? ? ? ? 32 DB"
	* Faster pattern scanning parameter : {0x80, 0xBF, -1, -1, -1, -1, -1, 0x0F, 0x84, -1, -1, -1, -1, 0x32, 0xDB}
	*/

	void* const module_handle = GetModuleHandleA(module_name);
	if (!module_handle) throw std::runtime_error("[pattern_scan] Couldn't get " + std::string{ module_name } + " handle");

	auto* const dos_headers = static_cast<PIMAGE_DOS_HEADER>(module_handle);
	auto* const nt_headers = reinterpret_cast<PIMAGE_NT_HEADERS>(static_cast<std::byte*>(module_handle) + dos_headers->e_lfanew);

	const auto size_of_image = nt_headers->OptionalHeader.SizeOfImage;
	const auto scan_bytes = static_cast<std::uint8_t*>(module_handle);

	const auto& pattern_c_array = pattern.data(); // Accessing std::array is expensive in debug mode.

	for (auto i = 0ul; i < size_of_image - size; ++i)
	{
		bool found = true;
		for (auto j = 0ul; j < size; ++j)
		{
			if (scan_bytes[i + j] != pattern_c_array[j] && pattern_c_array[j] != -1)
			{
				found = false;
				break;
			}
		}

		if (found)
		{
#ifdef _DEBUG
			std::cout << '[' << module_name << ']';

			for (auto i = 0u; i < size; ++i)
			{
				if (pattern_c_array[i] != -1)
				{
					std::cout << ' ' << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << pattern_c_array[i] << std::dec;
				} else
				{
					std::cout << " ?";
				}
			}

			std::cout << " -> 0x" << reinterpret_cast<std::byte*>(&scan_bytes[i]) << '\n';
#endif

			return reinterpret_cast<std::byte*>(&scan_bytes[i]);
		}
	}

	throw std::runtime_error("Pattern with " + std::to_string(size) + " bytes is wrong!");
}

static auto relative_to_absolute(const std::uintptr_t address) noexcept
{
	return address + 4 + *reinterpret_cast<int*>(address);
}

static auto relative_to_absolute(const std::byte* address) noexcept
{
	const auto new_address = reinterpret_cast<std::uintptr_t>(address);
	return new_address + 4 + *reinterpret_cast<int*>(new_address);
}

namespace signatures
{
	void initialize()
	{
		panorama_marshall_helper = *reinterpret_cast<decltype(panorama_marshall_helper)*>(pattern_scan<35>("client.dll",
			{0x68, -1, -1, -1, -1, 0x8B, 0xC8, 0xE8, -1, -1, -1, -1, 0x8D, 0x4D, 0xF4, 0xFF, 0x15, -1, -1, -1, -1, 0x8B, 0xCF, 0xFF, 0x15, -1, -1, -1, -1, 0x5F, 0x5E, 0x8B, 0xE5, 0x5D, 0xC3}) + 1);

		directx = **reinterpret_cast<decltype(directx)**>(pattern_scan<11>("shaderapidx9.dll",
			{0xA1, -1, -1, -1, -1, 0x50, 0x8B, 0x08, 0xFF, 0x51, 0x0C}) + 1);

		match_assisted_accept = *reinterpret_cast<decltype(match_assisted_accept)*>(pattern_scan<174>("client.dll",
			{0xC7, 0x45, -1, -1, -1, -1, -1, 0xC7, 0x45, -1, -1, -1, -1, -1, 0xC7, 0x45, -1, -1, -1, -1, -1, 0xC7, 0x45, -1, -1, -1, -1, -1, 0xC7, 0x45, -1, -1, -1, -1, -1, 0xC7, 0x45, -1, -1, -1, -1, -1, 0xC6, 0x45, 0xF4, -1, 0xC7, 0x45, -1, -1, -1, -1, -1, 0xE8, -1, -1, -1, -1, 0xA1, -1, -1, -1, -1, 0xA8, 0x01, 0x75, 0x39, 0x0F, 0x28, 0x05, -1, -1, -1, -1, 0x83, 0xC8, 0x01, 0xA3, -1, -1, -1, -1, 0x8A, 0x45, 0xFF, 0x0F, 0x11, 0x05, -1, -1, -1, -1, 0x68, -1, -1, -1, -1, 0x0F, 0x28, 0x05, -1, -1, -1, -1, 0xA2, -1, -1, -1, -1, 0x0F, 0x11, 0x05, -1, -1, -1, -1, 0xE8, -1, -1, -1, -1, 0x83, 0xC4, 0x04, 0x8D, 0x45, 0xD4, 0xB9, -1, -1, -1, -1, 0x50, 0x8D, 0x45, 0xFE, 0x50, 0xE8, -1, -1, -1, -1, 0xB8, -1, -1, -1, -1, 0x8B, 0xE5, 0x5D, 0xC2, 0x10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0xA1, -1, -1, -1, -1, 0x85, 0xC0, 0x75, 0x2E}) + 3);

		delayed_lobby = reinterpret_cast<decltype(delayed_lobby)>(pattern_scan<4>("client.dll",
			{0x63, 0x8B, 0x76, 0x24}) - 1);

		fake_prime = reinterpret_cast<decltype(fake_prime)>(pattern_scan<5>("client.dll",
			{0x17, 0xF6, 0x40, 0x14, 0x10}) - 1);

		client_system = **reinterpret_cast<decltype(client_system)**>(pattern_scan<11>("client.dll",
			{0x8B, 0x0D, -1, -1, -1, -1, 0x6A, -1, 0x83, 0xEC, 0x10}) + 2);

		local_inventory_offset = *reinterpret_cast<decltype(local_inventory_offset)*>(pattern_scan<10>("client.dll",
			{0x8B, 0x8F, -1, -1, -1, -1, 0x0F, 0xB7, 0xC0, 0x50}) + 2);

		inventory_offset = *reinterpret_cast<decltype(inventory_offset)*>(pattern_scan<8>("client.dll",
			{0x8D, 0x9E, -1, -1, -1, -1, 0x8B, 0x0B}) + 2);

		inventory_manager = *reinterpret_cast<decltype(inventory_manager)*>(pattern_scan<13>("client.dll",
			{0xB9, -1, -1, -1, -1, 0x8D, 0x44, 0x24, 0x10, 0x89, 0x54, 0x24, 0x14}) + 1);

		failed_to_open_crate = *reinterpret_cast<decltype(failed_to_open_crate)*>(pattern_scan<56>("client.dll",
			{0x68, -1, -1, -1, -1, 0x8B, 0x01, 0x8B, 0x80, -1, -1, -1, -1, 0xFF, 0xD0, 0x84, 0xC0, 0x74, 0x1A, 0x8B, 0x35, -1, -1, -1, -1, 0x8B, 0xD3, 0x33, 0xC9, 0x8B, 0x3E, 0xE8, -1, -1, -1, -1, 0x50, 0x8B, 0xCE, 0xFF, 0x97, -1, -1, -1, -1, 0x5F, 0x5E, 0xB0, 0x01, 0x5B, 0x8B, 0xE5, 0x5D, 0xC2, 0x04, -1}) + 1);

		item_customization_notification = *reinterpret_cast<decltype(item_customization_notification)*>(pattern_scan<17>("client.dll",
			{0x68, -1, -1, -1, -1, 0x8B, 0x80, -1, -1, -1, -1, 0xFF, 0xD0, 0x84, 0xC0, 0x74, 0x28}) + 1);

		loot_list_items_count_return_address = reinterpret_cast<decltype(loot_list_items_count_return_address)>(pattern_scan<39>("client.dll",
			{0x85, 0xC0, 0x0F, 0x84, -1, -1, -1, -1, 0x8B, 0xC8, 0xE8, -1, -1, -1, -1, 0x52, 0x50, 0xE8, -1, -1, -1, -1, 0x83, 0xC4, 0x08, 0x89, 0x44, 0x24, 0x14, 0x85, 0xC0, 0x0F, 0x84, -1, -1, -1, -1, 0x8B, 0x0D}));

		// Functions
		fn_add_econ_item = reinterpret_cast<decltype(fn_add_econ_item)>(pattern_scan<21>("client.dll",
			{0x55, 0x8B, 0xEC, 0x83, 0xE4, 0xF8, 0xA1, -1, -1, -1, -1, 0x83, 0xEC, 0x14, 0x53, 0x56, 0x57, 0x8B, 0xF9, 0x8B, 0x08}));

		fn_get_item_schema = reinterpret_cast<decltype(fn_get_item_schema)>(pattern_scan<9>("client.dll",
			{0xA1, -1, -1, -1, -1, 0x85, 0xC0, 0x75, 0x53}));

		fn_find_so_cache = reinterpret_cast<decltype(fn_find_so_cache)>(pattern_scan<13>("client.dll",
			{0x55, 0x8B, 0xEC, 0x83, 0xE4, 0xF8, 0x83, 0xEC, 0x1C, 0x0F, 0x10, 0x45, 0x08}));

		fn_create_base_type_cache = reinterpret_cast<decltype(fn_create_base_type_cache)>(pattern_scan<11>("client.dll",
			{0x55, 0x8B, 0xEC, 0x51, 0x53, 0x56, 0x8B, 0xD9, 0x8D, 0x45, 0x08}));

		fn_get_inventory_item_by_item_id = reinterpret_cast<decltype(fn_get_inventory_item_by_item_id)>(pattern_scan<11>("client.dll",
			{0x55, 0x8B, 0xEC, 0x8B, 0x55, 0x08, 0x83, 0xEC, 0x10, 0x8B, 0xC2}));

		fn_get_crate_series_by_id = reinterpret_cast<decltype(fn_get_crate_series_by_id)>(pattern_scan<32>("client.dll",
			{0x55, 0x8B, 0xEC, 0x8B, 0x45, 0x08, 0x56, 0x57, 0x8B, 0x30, 0x8B, 0x41, 0x10, 0x83, 0xF8, 0xFF, 0x74, 0x1E, 0x8B, 0x79, 0x04, 0x8D, 0x0C, 0x40, 0x8B, 0x54, 0xCF, 0x10, 0x3B, 0xD6, 0x7E, 0x05}));

		fn_update_inventory = reinterpret_cast<decltype(fn_update_inventory)>(relative_to_absolute(pattern_scan<17>("client.dll",
			{0xE8, -1, -1, -1, -1, 0x83, 0xC4, 0x04, 0x80, 0xBF, -1, -1, -1, -1, -1, 0x74, 0x0C}) + 1));

		fn_clear_inventory_image_rgba = reinterpret_cast<decltype(fn_clear_inventory_image_rgba)>(pattern_scan<14>("client.dll",
			{0x55, 0x8B, 0xEC, 0x81, 0xEC, -1, -1, -1, -1, 0x57, 0x8B, 0xF9, 0xC7, 0x47}));

		fn_get_soc_data = reinterpret_cast<decltype(fn_get_soc_data)>(pattern_scan<15>("client.dll",
			{0x55, 0x8B, 0xEC, 0x83, 0xE4, 0xF0, 0x83, 0xEC, 0x18, 0x56, 0x8B, 0xF1, 0x57, 0x8B, 0x86}));

		fn_get_static_data = reinterpret_cast<decltype(fn_get_static_data)>(pattern_scan<17>("client.dll",
			{0x55, 0x8B, 0xEC, 0x51, 0x53, 0x8B, 0xD9, 0x8B, 0x0D, -1, -1, -1, -1, 0x56, 0x57, 0x8B, 0xB9}));

		fn_tool_can_apply_to = reinterpret_cast<decltype(fn_tool_can_apply_to)>(pattern_scan<13>("client.dll",
			{0x55, 0x8B, 0xEC, 0x83, 0xEC, 0x18, 0x53, 0x56, 0x8B, 0xF1, 0x57, 0x8B, 0xFA}));

		fn_create_econ_item = reinterpret_cast<decltype(fn_create_econ_item)>((pattern_scan<37>("client.dll",
			{0xA1, -1, -1, -1, -1, 0x6A, 0x38, 0x8B, 0x08, 0x8B, 0x01, 0xFF, 0x50, 0x04, 0x85, 0xC0, 0x74, 0x07, 0x8B, 0xC8, 0xE9, -1, -1, -1, -1, 0x33, 0xC0, 0xC3, -1, -1, -1, -1, 0xA1, -1, -1, -1, -1})));

		fn_set_attribute_value = reinterpret_cast<decltype(fn_set_attribute_value)>(pattern_scan<17>("client.dll",
			{0x55, 0x8B, 0xEC, 0x83, 0xE4, 0xF8, 0x83, 0xEC, 0x3C, 0x53, 0x8B, 0x5D, 0x08, 0x56, 0x57, 0x6A, -1}));

		fn_init_item_customization_notification_event = reinterpret_cast<decltype(fn_init_item_customization_notification_event)>(pattern_scan<30>("client.dll",
			{0x55, 0x8B, 0xEC, 0xA1, -1, -1, -1, -1, 0x53, 0x56, 0x8B, 0xF1, 0x8B, 0xDA, 0x8B, 0x08, 0x57, 0x6A, 0x1C, 0x8B, 0x01, 0xFF, 0x50, 0x04, 0x8B, 0xF8, 0x85, 0xFF, 0x74, 0x48}));

		fn_initialize_attributes = reinterpret_cast<decltype(fn_initialize_attributes)>(pattern_scan<15>("client.dll",
			{0x55, 0x8B, 0xEC, 0x83, 0xE4, 0xF8, 0x83, 0xEC, 0x0C, 0x53, 0x56, 0x8B, 0xF1, 0x8B, 0x86}));

		fn_equip_wearable = reinterpret_cast<decltype(fn_equip_wearable)>(pattern_scan<13>("client.dll",
			{0x55, 0x8B, 0xEC, 0x83, 0xEC, 0x10, 0x53, 0x8B, 0x5D, 0x08, 0x57, 0x8B, 0xF9}));

		fn_get_econ_item_view = (reinterpret_cast<decltype(fn_get_econ_item_view)>(relative_to_absolute(pattern_scan<9>("client.dll",
			{0xE8, -1, -1, -1, -1, 0x89, 0x44, 0x24, 0x54}) + 1)));

		fn_set_custom_name = reinterpret_cast<decltype(fn_set_custom_name)>(relative_to_absolute(pattern_scan<18>("client.dll",
			{0xE8, -1, -1, -1, -1, 0x8B, 0x46, 0x78, 0xC1, 0xE8, 0x0A, 0xA8, 0x01, 0x74, 0x13, 0x8B, 0x46, 0x34}) + 1));

		fn_get_event_descriptor = reinterpret_cast<decltype(fn_get_event_descriptor)>(relative_to_absolute(pattern_scan<11>("engine.dll",
			{0xE8, -1, -1, -1, -1, 0x8B, 0xD8, 0x85, 0xDB, 0x75, 0x27}) + 1));
		
		fn_perform_screen_overlay = reinterpret_cast<decltype(fn_perform_screen_overlay)>(pattern_scan<13>("client.dll",
			{0x55, 0x8b, 0xec, 0x51, 0xa1, -1, -1, -1, -1, 0x53, 0x56, 0x8b, 0xd9}));
	}
}