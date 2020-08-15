#include "sdk.hpp"

namespace interfaces {

	void initialize( ) noexcept {

		input_system			= get_interface<i_input_system>		( "inputsystem.dll", "InputSystemVersion001" );
		surface					= get_interface<i_surface>			( "vguimatsurface.dll", "VGUI_Surface031" );
		console					= get_interface<i_console>			( "vstdlib.dll", "VEngineCvar007" );
		effects					= get_interface<i_effects>			( "client.dll", "IEffects001" );
		engine					= get_interface<iv_engine_client>	( "engine.dll", "VEngineClient014" );
		client					= get_interface<i_base_client_dll>	( "client.dll", "VClient018" );
		engine_sound			= get_interface<void*>				( "engine.dll", "IEngineSoundClient003" );

		inventory				= **reinterpret_cast< c_player_inventory*** >( utilities::pattern_scan( "client.dll", "8B 3D ? ? ? ? 85 FF 74 1A" ) + 2 );
		inventory_manager		= *reinterpret_cast< c_player_inventory** >( utilities::pattern_scan( "client.dll", "B9 ? ? ? ? 8D 44 24 10 89 54 24 14" ) + 1 );

		globals					= **reinterpret_cast< c_global_vars_base*** >( ( *reinterpret_cast< uintptr_t** >( client ) )[ 11 ] + 10 );
		client_mode				= **reinterpret_cast< void*** >( ( *reinterpret_cast< uintptr_t** >( client ) )[ 10 ] + 5 );

		std::cout << "Interfaces are good!\n";
	}
}