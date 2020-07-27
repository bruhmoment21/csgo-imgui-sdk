#include "sdk.hpp"

namespace interfaces {

	void initialize( ) {

		engine = get_interface<iv_engine_client>( "engine.dll", "VEngineClient014" );
		surface = get_interface<i_surface>( "vguimatsurface.dll", "VGUI_Surface031" );
		input_system = get_interface<i_input_system>( "inputsystem.dll", "InputSystemVersion001" );

		std::cout << "Interfaces are good!\n";
	}
}