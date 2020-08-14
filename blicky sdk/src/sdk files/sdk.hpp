#pragma once

#include "../other files/utilities/utilities.hpp"
#include "../core files/gui/gui.hpp"

#include "interfaces/c_global_vars_bars.hpp"
#include "interfaces/c_player_inventory.hpp"
#include "interfaces/iv_engine_client.hpp"
#include "interfaces/i_input_system.hpp"
#include "interfaces/i_base_client.hpp"
#include "interfaces/i_surface.hpp"
#include "interfaces/i_effects.hpp"
#include "interfaces/i_console.hpp"

#include <iostream>

namespace interfaces {

	template <typename T>
	inline T* get_interface( const char* module_name, const char* interface_name ) {

		const auto module{ GetModuleHandleA( module_name ) };
		if ( !module ) throw std::runtime_error( module_name + std::string{ " is not a good module!" } );

		if ( auto* const create_interface_fn{ reinterpret_cast< std::add_pointer_t< void* ( const char*, int* ) > >( GetProcAddress( module, "CreateInterface" ) ) } )
			if ( void* const result{ create_interface_fn( interface_name, nullptr ) } )
				return static_cast< T* >( result );

		throw std::runtime_error( interface_name + std::string{ " is not a good interface!" } );
	}

	inline i_input_system*			input_system{ nullptr };
	inline c_global_vars_base*		globals{ nullptr };
	inline i_surface*				surface{ nullptr };
	inline i_console*				console{ nullptr };
	inline i_effects*				effects{ nullptr };
	inline iv_engine_client*		engine{ nullptr };
	inline i_base_client_dll*		client{ nullptr };

	// Used mostly in inventory changers.
	inline c_player_inventory* inventory{ nullptr };
	inline c_player_inventory* inventory_manager{ nullptr };

	void initialize( ) noexcept;
}
