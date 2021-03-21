#pragma once

#include "i_game_event_manager.hpp"

class event_listener_t : public i_game_event_listener
{
public:
	void fire_game_event(i_game_event* event) noexcept;
	void setup() noexcept;
	void release() noexcept;
};

inline event_listener_t event_listener;