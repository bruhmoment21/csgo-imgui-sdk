#pragma once

#include "../other files/utilities/utilities.hpp"
#include "../core files/gui/gui.hpp"

#include "interfaces/iv_engine_client.hpp"
#include "interfaces/i_input_system.hpp"
#include "interfaces/i_surface.hpp"

#include <Windows.h>
#include <iostream>
#include <string>
#include <cstdio>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <vector>

#include <d3d9.h>

#include "../other files/imgui/imgui.h"
#include "../other files/imgui/imgui_impl_dx9.h"
#include "../other files/imgui/imgui_impl_win32.h"

namespace interfaces {
	template <typename T>
	T* get_interface( const std::string& module_name, const std::string& interface_name ) {
		
		using create_interface_fn = void* ( * )( const char*, int* );
		const auto fn = reinterpret_cast< create_interface_fn >( GetProcAddress( GetModuleHandleA( module_name.c_str( ) ), "CreateInterface" ) );

		if ( fn )
		{
			void* result = fn( interface_name.c_str( ), nullptr );
			
			if ( !result )
				throw std::runtime_error( interface_name + " wasn't found in " + module_name );
			
			return static_cast< T* >( result );
		}

		throw std::runtime_error( module_name + " wasn't found" );
	}

	inline iv_engine_client* engine;
	inline i_surface* surface;
	inline i_input_system* input_system;
	inline IDirect3DDevice9* directx;

	void initialize( );
}
