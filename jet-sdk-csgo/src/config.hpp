#pragma once

namespace config
{
	void create_directory() noexcept;
	void update_configs() noexcept;
	void create_file(const std::string_view name) noexcept;
	void delete_file(const std::size_t index) noexcept;
	void save(const std::size_t index) noexcept;
	void load(const std::size_t index) noexcept;

	inline bool auto_accept;
	inline bool block_advertisements;
	inline bool bypass_invite_cooldown;
	inline bool fake_prime;

	struct item_t
	{
		char custom_name[32] = "";
		char preview_name[128] = "";

		// Used for loading/saving
		short index = 0;
		int quality = 0;
		int paint_kit = 0;
		float wear = -1.f;
		int seed = -1;
		int stattrak_kills = -1;
		std::uint64_t item_id = 0u;
		std::uint32_t stickers[5];
	};

	inline std::vector<item_t> added_items;
	
	// Don't save
	inline int stattrak_kills = -1, seed = -1;
	inline float wear = 0.f;
	inline float stattrak_chance = 17.f, maximum_wear_chance = 0.65f, knife_chance = 0.15f;
	inline bool is_souvenir;

	inline std::vector<std::string> configs;
}