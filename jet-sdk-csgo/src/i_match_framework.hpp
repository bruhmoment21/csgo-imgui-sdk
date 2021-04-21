#pragma once

#include "i_app_system.hpp"
#include "c_key_values.hpp"

class i_match_session
{
public:
	// Get an internal pointer to session system-specific data
	virtual c_key_values* get_session_system_data() = 0;

	// Get an internal pointer to session settings
	virtual c_key_values* get_session_settings() = 0;

	// Update session settings, only changing keys and values need
	// to be passed and they will be updated
	virtual void update_session_settings(c_key_values* settings) = 0;

	// Issue a session command
	virtual void command(c_key_values* command) = 0;
};

class i_match_framework : public i_app_system
{
public:
	// Run frame of the matchmaking framework
	virtual void run_frame() = 0;

	// Get matchmaking extensions
	virtual void* get_match_extensions() = 0;

	// Get events container
	virtual void* get_events_subscription() = 0;

	// Get the matchmaking title interface
	virtual void* get_match_title() = 0;

	// Get the match session interface of the current match framework type
	virtual i_match_session* get_match_session() = 0;

	// Get the network msg encode/decode factory
	virtual void* get_match_network_msg_controller() = 0;

	// Get the match system
	virtual void* get_match_system() = 0;

	// Entry point to create session
	virtual void create_session(c_key_values* settings) = 0;

	// Entry point to match into a session
	virtual void match_session(c_key_values* settings) = 0;

	// Accept invite
	virtual void accept_invite(int controller) = 0;

	// Close the session
	virtual void close_session() = 0;
};