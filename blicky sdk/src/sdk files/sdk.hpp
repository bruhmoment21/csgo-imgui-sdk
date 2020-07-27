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

		using create_interface_fn = void* ( * )( const char*, int* );
		const auto fn{ reinterpret_cast< create_interface_fn >( GetProcAddress( GetModuleHandleA( module_name ), "CreateInterface" ) ) };

		if ( fn ) {
			void* result{ fn( interface_name, nullptr ) };

			if ( !result )
				throw std::runtime_error( "One interface not found. Attach debugger for more info!" );

			return static_cast< T* >( result );
		}

		throw std::runtime_error( "Module name wasn't found!" );
	}

	inline iv_engine_client* engine;
	inline i_surface* surface;
	inline i_input_system* input_system;

	void initialize( );
}
