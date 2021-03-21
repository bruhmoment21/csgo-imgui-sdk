#pragma once

#include "i_steam_user_stats.hpp"

struct steam_api_context_t
{
	void* steam_client;
	void* steam_user;
	void* steam_friends;
	void* steam_utils;
	void* steam_matchmaking;
	void* steam_game_search;
	i_steam_user_stats* steam_user_stats;
	void* steam_apps;
	void* steam_matchmaking_servers;
	void* steam_networking;
	void* steam_remote_storage;
	void* steam_steam_screenshots;
	void* steam_http;
	void* controller;
	void* steam_ugc;
	void* steam_app_list;
	void* steam_music;
	void* steam_music_remote;
	void* steam_html_surface;
	void* steam_inventory;
	void* steam_video;
	void* steam_parental_settings;
	void* steam_input;
};