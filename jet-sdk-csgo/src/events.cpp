#include "pch.hpp"
#include "events.hpp"

#include "sdk.hpp"

void event_listener_t::fire_game_event(i_game_event* event) noexcept
{
	switch (fnv::hash(event->get_name()))
	{
		case fnv::hash("player_death"):

			break;
	}
}

void event_listener_t::setup() noexcept
{
	if (sdk::event_manager->add_listener(this, "player_death"))
	{
		if (const auto desc = signatures::fn_get_event_descriptor(sdk::event_manager, "player_death", nullptr))
		{
			auto& listeners = desc->listeners;
			std::swap(listeners[0], listeners[listeners.size - 1]);
		}
	}
}

void event_listener_t::release() noexcept
{
	sdk::event_manager->remove_listener(this);
}