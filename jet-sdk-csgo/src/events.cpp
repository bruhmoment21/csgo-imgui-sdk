#include "pch.hpp"
#include "events.hpp"

#include "sdk.hpp"

// If you need to modify event contents you must set overwrite to true otherwise if you inject when connected nothing will happen when modifying.
static void add_listener(i_game_event_listener* const _this, const char* event_name, const bool overwrite = false) noexcept
{
	if (sdk::event_manager->add_listener(_this, event_name))
	{
		if (const auto desc = signatures::fn_get_event_descriptor(sdk::event_manager, event_name, nullptr); overwrite && desc)
		{
			auto& listeners = desc->listeners;
			std::swap(listeners[0], listeners[listeners.size - 1]);
		}
	}

#ifdef _DEBUG
	std::cout << "Added listener on " << event_name << " successfully!" << (overwrite ? " [OVERWRITTEN]" : "") << '\n';
#endif
}

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
	add_listener(this, "player_death", true);
}

void event_listener_t::release() noexcept
{
	sdk::event_manager->remove_listener(this);
}