#include "pch.hpp"
#include "config.hpp"

#include "json.hpp"
#include "sdk.hpp"

#include "imgui/imgui.h"

static std::string path = "jet sdk configs/";
static const std::filesystem::path path_fs = "jet sdk configs";

template <typename T>
static void read_value(const nlohmann::json& src, T& dest) noexcept
{
	if (src.is_null()) return;

	dest = src.get<T>();
}

template <typename T>
static T read_value(const nlohmann::json& src) noexcept
{
	if (!src.is_null())
	{
		return src.get<T>();
	}

	return NULL;
}

template <typename T>
static void write_value(nlohmann::json& dest, const T& src) noexcept
{
	dest = src;
}

namespace config
{
	void create_directory() noexcept
	{
		if (!std::filesystem::exists(path_fs)) // std::string will alloc.
		{
			std::filesystem::create_directory(path_fs);
		} else
		{
			update_configs();
		}
	}

	void update_configs() noexcept
	{
		configs.clear();

		for (const auto& entry : std::filesystem::directory_iterator{path_fs})
		{
			if (!entry.is_regular_file() || entry.path().extension() != ".json") continue;

			const auto& file_name = entry.path().stem().string();
			configs.emplace_back(file_name);
		}
	}

	void create_file(const std::string_view name) noexcept
	{
		if (name.empty() || name.find_first_not_of(' ') == std::string_view::npos) return;
		if (std::find(configs.cbegin(), configs.cend(), name) != configs.cend()) return;

		configs.emplace_back(name);
		save(configs.size() - 1);
	}

	void delete_file(const std::size_t index) noexcept
	{
		if (configs.size() <= index) return;

		path.append(configs.at(index));
		path.append(".json");
		std::filesystem::remove(path);
		path.erase(path.size() - configs.at(index).size() - 5);

		configs.erase(configs.cbegin() + index);
		configs.shrink_to_fit();
	}

	void save(const std::size_t index) noexcept
	{
		nlohmann::json json;

		write_value(json["Miscellaneous"]["Auto accept"], auto_accept);
		write_value(json["Miscellaneous"]["Block Advertisements"], block_advertisements);
		write_value(json["Miscellaneous"]["Bypass Invite Cooldown"], bypass_invite_cooldown);
		write_value(json["Miscellaneous"]["Fake Prime"], fake_prime);
		
		path.append(configs.at(index));

		const bool has_json_in_filename = path.rfind(".json") != std::string::npos;
		if (!has_json_in_filename)
		{
			path.append(".json");
		}

		if (std::ofstream output_file{path}; output_file.good())
		{
			output_file << std::setw(4) << json;
		}

		path.erase(path.size() - configs.at(index).size() - (!has_json_in_filename ? 5 : 0));
	}

	void load(const std::size_t index) noexcept
	{	
		path.append(configs.at(index));
		path.append(".json");

		if (std::ifstream input_file{path}; input_file.good())
		{
			nlohmann::json json;
			input_file >> json;

			read_value(json["Miscellaneous"]["Auto accept"], auto_accept);
			read_value(json["Miscellaneous"]["Block Advertisements"], block_advertisements);
			read_value(json["Miscellaneous"]["Bypass Invite Cooldown"], bypass_invite_cooldown);
			read_value(json["Miscellaneous"]["Fake Prime"], fake_prime);
		}

		path.erase(path.size() - configs.at(index).size() - 5);
	}
}