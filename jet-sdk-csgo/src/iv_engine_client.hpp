#pragma once

#include "utilities.hpp"
#include "steam api/steam.hpp"

struct player_info_t
{
	std::uint64_t version;
	union
	{
		std::uint64_t steam_id;
		struct
		{
			std::uint32_t xuid_low;
			std::uint32_t xuid_high;
		};
	};

	char name[128];
	int user_id;
	char guid[33];
	std::uint32_t friends_id;
	char friends_name[128];
	bool fake_player;
	bool hltv;
	int custom_files[4];
	std::uint8_t files_downloaded;
};

class iv_engine_client
{
public:
	bool get_player_info(const int index, player_info_t& info)
	{
		return utilities::call_virtual<bool, 8, int, player_info_t&>(this, index, std::ref(info));
	}

	int get_player_for_user_id(const int user_id)
	{
		return utilities::call_virtual<int, 9, int>(this, user_id);
	}

	int get_local_player()
	{
		return utilities::call_virtual<int, 12>(this);
	}

	bool console_opened()
	{
		return utilities::call_virtual<bool, 11>(this);
	}

	bool is_in_game()
	{
		return utilities::call_virtual<bool, 26>(this);
	}

	bool is_connected()
	{
		return utilities::call_virtual<bool, 27>(this);
	}

	bool connected_and_in_game()
	{
		return is_in_game() && is_connected();
	}

	matrix_4x4 world_to_screen_matrix()
	{
		return utilities::call_virtual<const matrix_4x4&, 37>(this);
	}

	const char* get_level_name()
	{
		return utilities::call_virtual<const char*, 52>(this);
	}

	void cmd_unrestricted(const char* command)
	{
		return utilities::call_virtual<void, 114, const char*>(this, command, false);
	}

	// unknowncheats.me -> butterflybankai
	steam_api_context_t* get_steam_context()
	{ 
		return utilities::call_virtual<steam_api_context_t*, 185>(this);
	}
};