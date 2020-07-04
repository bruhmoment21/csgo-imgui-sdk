#include "sdk.hpp"

void interfaces::initialize( ) {
	
	engine = get_interface<iv_engine_client>( "engine.dll", "VEngineClient014" );
	surface = get_interface<i_surface>( "vguimatsurface.dll", "VGUI_Surface031" );
	input_system = get_interface<i_input_system>( "inputsystem.dll", "InputSystemVersion001" );
	
	directx = **reinterpret_cast< IDirect3DDevice9*** >( utilities::pattern_scan( GetModuleHandleA( "shaderapidx9.dll" ), "A1 ? ? ? ? 50 8B 08 FF 51 0C" ) + 1 );
	
	std::cout << "Interfaces are good!" << std::endl;
}
