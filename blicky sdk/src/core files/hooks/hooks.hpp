#pragma once

#include <type_traits>
#include <d3d9.h>

class i_base_client_dll; // Forward declare instead of including header when possible (Good practice).
enum class client_frame_stage_t;

class i_recipient_filter;
struct vec3_t;

class i_surface;

namespace hooks {

	void initialize( );
	void release( ) noexcept;

	namespace menu {
		inline std::add_pointer_t< HRESULT __stdcall( IDirect3DDevice9*, D3DPRESENT_PARAMETERS* ) > reset_original{ nullptr };
		inline std::add_pointer_t< HRESULT __stdcall( IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA* ) > present_original{ nullptr };

		LRESULT __stdcall wnd_proc( HWND window, UINT msg, WPARAM wparm, LPARAM lparm ) noexcept;
		HRESULT __stdcall reset( IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params ) noexcept;
		HRESULT __stdcall present( IDirect3DDevice9* device, const RECT* src, const RECT* dest, HWND window_override, const RGNDATA* dirty_region ) noexcept;
	}

	namespace client {
		inline void( __thiscall* frame_stage_notify_original )( i_base_client_dll*, client_frame_stage_t ){ nullptr };

		void __stdcall frame_stage_notify( const client_frame_stage_t frame_stage ) noexcept;
	}

	namespace engine {
		inline void( __thiscall* emit_sound_original )( void*, i_recipient_filter&, int, int, const char*, unsigned int, const char*, float, int, float, int, int, const vec3_t*, const vec3_t*, void*, bool, float, int, int ){ nullptr };
	
		void __stdcall emit_sound( i_recipient_filter& filter, const int ent_index, const int channel, const char* entry, const unsigned int hash, const char* sample, const float vol, const int seed, const float attenuation, const int flags, const int pitch, const vec3_t* origin, const vec3_t* direction, void* const utl_origin, const bool update_pos, const float sound_time, const int speaker_entity, const int unk ) noexcept;
	}

	namespace custom_layout_popup_parameters {
		inline std::add_pointer_t< int __stdcall( int, char*, char* ) > original{ nullptr };
		int __stdcall hook( const int a1, char* popup_file, char* popup_params ) noexcept;
	}

	namespace surface {
		inline void( __thiscall* lock_cursor_original )( i_surface* ){ nullptr };
		void __stdcall lock_cursor( ) noexcept;
	}

	namespace sv_cheats {
		inline std::add_pointer_t< bool __fastcall( void* ) > original{ nullptr };
		bool __fastcall hook( void* const convar ) noexcept;
	}
}
