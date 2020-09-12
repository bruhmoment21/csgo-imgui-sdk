#include "sdk.hpp"

template <typename T>
static T* get_interface( const char* module_name, const char* interface_name ) { // Moved because logging.

	const auto module{ GetModuleHandleA( module_name ) };
	if ( !module ) throw std::runtime_error( module_name + std::string{ " is not a good module!" } );

	if ( auto* const create_interface_fn{ reinterpret_cast< std::add_pointer_t< void* ( const char*, int* ) > >( GetProcAddress( module, "CreateInterface" ) ) } )
		if ( void* const result{ create_interface_fn( interface_name, nullptr ) } ) {

			console::log( "%s => 0x%x", console_logs::info_log, interface_name, result );
			return static_cast< T* >( result );
		}

	throw std::runtime_error( interface_name + std::string{ " is not a good interface!" } );
}

namespace sdk {

	void initialize( ) {
		console					= get_interface< i_console >			( "vstdlib.dll", "VEngineCvar007" );
		engine					= get_interface< iv_engine_client >		( "engine.dll", "VEngineClient014" );
		input_system			= get_interface< i_input_system >		( "inputsystem.dll", "InputSystemVersion001" );
		surface					= get_interface< i_surface >			( "vguimatsurface.dll", "VGUI_Surface031" );
		effects					= get_interface< i_effects >			( "client.dll", "IEffects001" );
		client					= get_interface< i_base_client_dll >	( "client.dll", "VClient018" );
		engine_sound			= get_interface< void* >				( "engine.dll", "IEngineSoundClient003" );

		inventory				= **reinterpret_cast< c_player_inventory*** >( utilities::pattern_scan( "client.dll", "8B 3D ? ? ? ? 85 FF 74 1A" ) + 2 );
		inventory_manager		= *reinterpret_cast< c_player_inventory** >( utilities::pattern_scan( "client.dll", "B9 ? ? ? ? 8D 44 24 10 89 54 24 14" ) + 1 );

		globals					= **reinterpret_cast< c_global_vars_base*** >( ( *reinterpret_cast< uintptr_t** >( client ) )[ 11 ] + 10 );
		client_mode				= **reinterpret_cast< void*** >( ( *reinterpret_cast< uintptr_t** >( client ) )[ 10 ] + 5 );

		console::log( "Interfaces initialized!", console_logs::successful_log );

		// Cache
		cam_think = utilities::pattern_scan( "client.dll", "85 C0 75 30 38 86" );
		set_local_player_ready = reinterpret_cast< decltype( set_local_player_ready ) >( utilities::pattern_scan( "client.dll", "55 8B EC 83 E4 F8 8B 4D 08 BA ? ? ? ? E8 ? ? ? ? 85 C0 75 12" ) );
		view_matrix = *reinterpret_cast< std::uint8_t* >( utilities::pattern_scan( "client.dll", "0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9" ) + 3 ) + 176;

		console::log( "Cache initialized!", console_logs::successful_log );
	}
}