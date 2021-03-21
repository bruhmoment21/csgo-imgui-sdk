#pragma once

#include "i_game_event_manager.hpp"
#include "i_client_leaf_system.hpp"
#include "ui_engine_source.hpp"
#include "iv_engine_client.hpp"
#include "i_input_system.hpp"
#include "i_entity_list.hpp"
#include "iv_model_info.hpp"
#include "i_base_client.hpp"
#include "i_localize.hpp"
#include "i_surface.hpp"
#include "i_console.hpp"

struct player_info_t;

namespace sdk
{
	inline i_base_client* client;
	inline iv_engine_client* engine;
	inline i_surface* surface;
	inline i_entity_list* entity_list;
	inline i_input_system* input_system;
	inline c_panorama_ui_engine* panorama_ui_engine;
	inline void* engine_sound;
	inline i_localize* localize;
	inline i_game_event_manager* event_manager;
	inline iv_model_info* model_info;
	inline i_client_leaf_system* leaf_system;
	inline i_console* console;

	void initialize();
}

namespace local_player
{
	inline entity_t* entity;
	inline player_info_t info;
}