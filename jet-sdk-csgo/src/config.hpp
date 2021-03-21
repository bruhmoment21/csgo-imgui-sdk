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

	inline std::vector<std::string> configs;
}
