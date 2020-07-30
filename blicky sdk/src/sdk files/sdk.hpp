#pragma once

#include "../other files/utilities/utilities.hpp"
#include "../core files/gui/gui.hpp"

#include "interfaces/iv_engine_client.hpp"
#include "interfaces/i_input_system.hpp"
#include "interfaces/i_surface.hpp"

#include <Windows.h>
#include <iostream>

namespace interfaces {

	template <typename T>
	T* get_interface( const char* module_name, const char* interface_name ) {

		const auto module{ GetModuleHandleA( module_name ) };
		auto* const create_interface_fn{ reinterpret_cast< void* ( * )( const char*, int* ) >( GetProcAddress( module, "CreateInterface" ) ) };

		if ( create_interface_fn ) {
			void* const result{ create_interface_fn( interface_name, nullptr ) };

			if ( !result )
				throw std::runtime_error( "One interface not found. Attach debugger for more info!" );

			return static_cast< T* >( result );
		}

		throw std::runtime_error( "One module name wasn't found!" );
	}

	inline iv_engine_client* engine{ nullptr };
	inline i_surface* surface{ nullptr };
	inline i_input_system* input_system{ nullptr };

	void initialize( );
}
