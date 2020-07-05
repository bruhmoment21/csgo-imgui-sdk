#pragma once
#include <d3d9.h>

class i_surface; // forward declare instead of including header when possible(good practice)

namespace hooks {
	void initialize( );
	void release( );

	inline unsigned int get_virtual( void* class_name, const unsigned int index ) {
		return static_cast< unsigned int >( ( *static_cast< int** >( class_name ) )[ index ] );
	}
	
	namespace menu {
		using reset_fn = HRESULT( __stdcall* )( IDirect3DDevice9*, D3DPRESENT_PARAMETERS* );
		using present_fn = HRESULT( __stdcall* )( IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA* );
		
		LRESULT WINAPI wnd_proc( HWND window, UINT msg, WPARAM wparm, LPARAM lparm );
		HRESULT D3DAPI reset( IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params );
		HRESULT D3DAPI present( IDirect3DDevice9* device, const RECT* src, const RECT* dest, HWND window_override, const RGNDATA* dirty_region );
	}

	namespace surface {
		using lock_cursor_fn = void( __thiscall* )( i_surface* );
		void __stdcall lock_cursor( ) noexcept;
	}
}
