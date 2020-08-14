#pragma once

#include <type_traits>
#include <d3d9.h>

enum class client_frame_stage_t; // Forward declare instead of including header when possible (Good practice).
class i_base_client_dll;

class i_surface;

namespace hooks {

	void initialize( ) noexcept;
	void release( ) noexcept;

	inline void* get_virtual( void* const class_name, const unsigned int index ) noexcept {
		return reinterpret_cast< void* >( ( *static_cast< int** >( class_name ) )[ index ] );
	}

	namespace menu {
		inline std::add_pointer_t< HRESULT __stdcall( IDirect3DDevice9*, D3DPRESENT_PARAMETERS* ) > reset_original{ nullptr };
		inline std::add_pointer_t< HRESULT __stdcall( IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA* ) > present_original{ nullptr };

		LRESULT __stdcall wnd_proc( HWND window, UINT msg, WPARAM wparm, LPARAM lparm ) noexcept;
		HRESULT __stdcall reset( IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params ) noexcept;
		HRESULT __stdcall present( IDirect3DDevice9* device, const RECT* src, const RECT* dest, HWND window_override, const RGNDATA* dirty_region ) noexcept;
	}

	namespace client {
		inline void( __thiscall* frame_stage_notify_original )( i_base_client_dll*, client_frame_stage_t ) { nullptr };

		void __stdcall frame_stage_notify( const client_frame_stage_t frame_stage ) noexcept;
	}

	namespace surface {
		inline void( __thiscall* lock_cursor_original )( i_surface* ) { nullptr };
		void __stdcall lock_cursor( ) noexcept;
	}

	namespace sv_cheats {
		inline std::add_pointer_t< bool __fastcall( void* ) > original{ nullptr };
		bool __fastcall hook( void* convar, int edx ) noexcept;
	}
}
