#include "sdk.hpp"

namespace interfaces {

	void initialize( ) {

		engine = get_interface<iv_engine_client>( "engine.dll", "VEngineClient014" );
		surface = get_interface<i_surface>( "vguimatsurface.dll", "VGUI_Surface031" );
		input_system = get_interface<i_input_system>( "inputsystem.dll", "InputSystemVersion001" );

		inventory = **reinterpret_cast< c_player_inventory*** >( utilities::pattern_scan( "client.dll", "8B 3D ? ? ? ? 85 FF 74 1A" ) + 2 );
		inventory_manager = *reinterpret_cast< c_player_inventory** >( utilities::pattern_scan( "client.dll", "B9 ? ? ? ? 8D 44 24 10 89 54 24 14" ) + 1 );

		std::cout << "Interfaces are good!\n";
	}
}