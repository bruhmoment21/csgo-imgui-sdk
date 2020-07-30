#pragma once
#include <d3d9.h>

class i_surface; // forward declare instead of including header when possible(good practice)

namespace hooks {

	void initialize( );
	void release( );

	inline void* get_virtual( void* class_name, const unsigned int index ) noexcept {
		return reinterpret_cast< void* >( ( *static_cast< int** >( class_name ) )[ index ] );
	}

	namespace menu {
		inline HRESULT( __stdcall* reset_original )( IDirect3DDevice9*, D3DPRESENT_PARAMETERS* ) { nullptr };
		inline HRESULT( __stdcall* present_original )( IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA* ) { nullptr };

		LRESULT WINAPI wnd_proc( HWND window, UINT msg, WPARAM wparm, LPARAM lparm );
		HRESULT D3DAPI reset( IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params );
		HRESULT D3DAPI present( IDirect3DDevice9* device, const RECT* src, const RECT* dest, HWND window_override, const RGNDATA* dirty_region );
	}

	namespace surface {
		inline void( __thiscall* lock_cursor_original )( i_surface* ) { nullptr };
		void __stdcall lock_cursor( ) noexcept;
	}
}
