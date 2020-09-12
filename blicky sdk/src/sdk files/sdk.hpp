#pragma once

#include "../other files/utilities/utilities.hpp"
#include "../core files/config/config_system.hpp"
#include "../core files/gui/gui.hpp"

#include "interfaces/c_global_vars_bars.hpp"
#include "interfaces/c_player_inventory.hpp"
#include "interfaces/iv_engine_client.hpp"
#include "interfaces/i_input_system.hpp"
#include "interfaces/i_base_client.hpp"
#include "interfaces/i_surface.hpp"
#include "interfaces/i_effects.hpp"
#include "interfaces/i_console.hpp"

#include <stdexcept>

namespace sdk {

	// Void pointers are classes that I think their methods aren't helpful.

	inline i_console*				console{ nullptr };
	inline iv_engine_client*		engine{ nullptr };
	inline i_input_system*			input_system{ nullptr };
	inline void*					engine_sound{ nullptr };
	inline void*					client_mode{ nullptr };
	inline c_global_vars_base*		globals{ nullptr };
	inline i_surface*				surface{ nullptr };
	inline i_effects*				effects{ nullptr };
	inline i_base_client_dll*		client{ nullptr };

	// Used mostly in inventory changers.
	inline c_player_inventory* inventory{ nullptr };
	inline c_player_inventory* inventory_manager{ nullptr };

	// Cache
	inline std::uint8_t* cam_think{ nullptr };
	inline std::add_pointer_t< bool __stdcall( const char* ) > set_local_player_ready{ nullptr };
	inline std::uint8_t view_matrix{ NULL };

	void initialize( );
}

enum class console_logs : int {
	default_log = 0, // Doesn't add anything in front of string, color stays white.
	info_log,
	successful_log,
	warning_log,
	error_log,
};

namespace console {

	inline void allocate( ) noexcept {
#ifdef _DEBUG
		if constexpr ( !config::log_in_csgo ) {

			AllocConsole( );

			freopen_s( reinterpret_cast< FILE** >stdin, "conin$", "r", stdin );
			freopen_s( reinterpret_cast< FILE** >stdout, "conout$", "w", stdout );

			SetConsoleTitleA( "blicky sdk" );
		}
#endif
	}

	inline void release( ) noexcept {
#ifdef _DEBUG
		if constexpr ( !config::log_in_csgo ) {

			fclose( stdin );
			fclose( stdout );

			FreeConsole( );
		}
#endif
	}

	template <typename ...values>
	inline void log( const char* message, const console_logs& log_type = console_logs::default_log, const values&... parameters ) noexcept { // Main log function.

#ifdef _DEBUG
		const char* tag{ "[LOG] " };
		console_color color{ console_color::white( ) };

		switch ( log_type ) {
			case console_logs::info_log:		tag = "[INFO] ";		color = console_color::blue( ); break;
			case console_logs::successful_log:	tag = "[SUCCESS] ";	color = console_color::green( ); break;
			case console_logs::warning_log:		tag = "[WARNING] ";	color = console_color::yellow( ); break;
			case console_logs::error_log:		tag = "[ERROR] ";	color = console_color::red( ); break;
		}

		// Prints tag then the rest. Hacky code.
		// No std::string because a lot of memory will be allocated on the heap.
		if constexpr ( config::log_in_csgo ) {
			if ( !sdk::console ) return;

			sdk::console->printf_color( color, tag );
			sdk::console->printf_color( color, message, parameters... );
			sdk::console->printf_color( color, "\n" );
		} else if constexpr ( !config::log_in_csgo ) {

			printf( tag );
			printf( message, parameters... );
			printf( "\n" );
		}
#endif
	}
}