#include "hooks.hpp"

#include "../../sdk files/netvars/netvar_manager.hpp"
#include "../../other files/minhook/minhook.h"
#include "../config/config_system.hpp"
#include "../../sdk files/sdk.hpp"
#include "../hacks/misc/misc.hpp"

#include "../../other files/imgui/imgui.h"
#include "../../other files/imgui/imgui_impl_dx9.h"
#include "../../other files/imgui/imgui_impl_win32.h"

#include <intrin.h>

static void MH_CreateHookSafe( void* const target, void* const detour, void* const original, const char* name = "Unnamed hook" ) {

	if ( MH_CreateHook( static_cast< LPVOID >( target ), static_cast< LPVOID >( detour ), static_cast< LPVOID* >( original ) ) != MH_OK )
		throw std::runtime_error( std::string{ name } + " hook failed!" );

	console::log( "%s hooked successfully!", console_logs::info_log, name );
}

namespace hooks {

	bool context_created{ false }; // Prevents present hook running before WNDPROC hook.

	WNDPROC original_wnd_proc{ nullptr };

	// Used in present hook to fix server browser not showing after injecting.
	IDirect3DVertexDeclaration9* vert_declaration{ nullptr };
	IDirect3DVertexShader9* vert_shader{ nullptr };
	DWORD old_d3drs_colorwriteenable{ NULL };

	void initialize( ) {
		void* const reset_target{ **reinterpret_cast< void*** >( utilities::pattern_scan( "gameoverlayrenderer.dll", "FF 15 ? ? ? ? 8B F8 85 FF 78 18" ) + 2 ) };
		void* const present_target{ **reinterpret_cast< void*** >( utilities::pattern_scan( "gameoverlayrenderer.dll", "FF 15 ? ? ? ? 8B F8 85 DB" ) + 2 ) };
		void* const frame_stage_notify_target{ virtual_method::get_virtual( sdk::client, 37 ) };
		void* const emit_sound_target{ virtual_method::get_virtual( sdk::engine_sound, 5 ) };
		void* const custom_layout_popup_parameters_target{ utilities::pattern_scan( "client.dll", "55 8B EC A1 ? ? ? ? 56 57 68 ? ? ? ? 8B 78 24" ) }; // All credits go to hinnie. Better methods have been posted on the same thread tho.
		void* const lock_cursor_target{ virtual_method::get_virtual( sdk::surface, 67 ) };
		void* const sv_cheats_target{ virtual_method::get_virtual( sdk::console->get_convar( "sv_cheats" ), index_convar ) };

		original_wnd_proc = reinterpret_cast< WNDPROC >( SetWindowLongA( FindWindowA( "Valve001", nullptr ), GWL_WNDPROC, reinterpret_cast< LONG >( menu::wnd_proc ) ) );

		MH_Initialize( );
		
		MH_CreateHookSafe( reset_target, &menu::reset, &menu::reset_original, "reset" );
		MH_CreateHookSafe( present_target, &menu::present, &menu::present_original, "present" );
		MH_CreateHookSafe( frame_stage_notify_target, &client::frame_stage_notify, &client::frame_stage_notify_original, "frame_stage_notify" );
		MH_CreateHookSafe( emit_sound_target, &engine::emit_sound, &engine::emit_sound_original, "emit_sound" );
		MH_CreateHookSafe( custom_layout_popup_parameters_target, &custom_layout_popup_parameters::hook, &custom_layout_popup_parameters::original, "custom_layout_popup_parameters" );
		MH_CreateHookSafe( lock_cursor_target, &surface::lock_cursor, &surface::lock_cursor_original, "lock_cursor" );
		MH_CreateHookSafe( sv_cheats_target, &sv_cheats::hook, &sv_cheats::original, "sv_cheats" );

		MH_EnableHook( nullptr );
		
		console::log( "Hooks initialized!", console_logs::successful_log );
	}

	void release( ) noexcept {
		
		MH_Uninitialize( );
		MH_DisableHook( nullptr );
		
		if ( gui::initialized ) {
			SetWindowLongA( FindWindowA( "Valve001", nullptr ), GWL_WNDPROC, reinterpret_cast< LONG >( original_wnd_proc ) );

			ImGui_ImplDX9_Shutdown( );
			ImGui_ImplWin32_Shutdown( );
			ImGui::DestroyContext( );
			sdk::input_system->enable_input( true );
		}
	}

	LRESULT __stdcall menu::wnd_proc( HWND window, UINT msg, WPARAM wparm, LPARAM lparm ) noexcept {

		if ( !gui::initialized ) { // Is true after calling gui::init( );
			console::log( "ImGui Version: %s", console_logs::info_log, IMGUI_VERSION );
			ImGui::CreateContext( );
			ImGui_ImplWin32_Init( window );
			gui::init( );
			context_created = true;
		}

		LRESULT ImGui_ImplWin32_WndProcHandler( HWND hwnd, UINT msg, WPARAM wparm, LPARAM lparm );
		ImGui_ImplWin32_WndProcHandler( window, msg, wparm, lparm );
		sdk::input_system->enable_input( !gui::is_open );
		
		return CallWindowProcA( original_wnd_proc, window, msg, wparm, lparm );
	}

	HRESULT __stdcall menu::reset( IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params ) noexcept {
		ImGui_ImplDX9_InvalidateDeviceObjects( );
		return reset_original( device, params );
	}

	HRESULT __stdcall menu::present( IDirect3DDevice9* device, const RECT* src, const RECT* dest, HWND window_override, const RGNDATA* dirty_region ) noexcept {

		if ( !context_created ) return FALSE;

		// Save state
		device->GetRenderState( D3DRS_COLORWRITEENABLE, &old_d3drs_colorwriteenable );
		device->GetVertexDeclaration( &vert_declaration );
		device->GetVertexShader( &vert_shader );
		device->SetRenderState( D3DRS_COLORWRITEENABLE, 0xFFFFFFFF );
		device->SetRenderState( D3DRS_SRGBWRITEENABLE, false );
		device->SetSamplerState( NULL, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
		device->SetSamplerState( NULL, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );
		device->SetSamplerState( NULL, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP );
		device->SetSamplerState( NULL, D3DSAMP_SRGBTEXTURE, NULL );

		/* Uncomment this if you wish to disable anti aliasing. See gui.cpp too.
		* device->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, FALSE );
		* device->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE, FALSE );
		*/

		ImGui_ImplDX9_Init( device );

		ImGui_ImplDX9_NewFrame( );
		ImGui_ImplWin32_NewFrame( );
		ImGui::NewFrame( );

		if ( ImGui::IsKeyPressed( VK_INSERT, false ) ) {
			gui::is_open = !gui::is_open;
			if ( !gui::is_open )
				sdk::input_system->reset_input_state( );
		}

		if ( gui::is_open )
			gui::render( );

		if ( sdk::engine->connected_and_in_game( ) ) {
			misc::thirdperson( );
		}

		ImGui::EndFrame( );
		ImGui::Render( );

		if ( device->BeginScene( ) == D3D_OK ) {
			ImGui_ImplDX9_RenderDrawData( ImGui::GetDrawData( ) );
			device->EndScene( );
		}

		// Restore state
		device->SetRenderState( D3DRS_COLORWRITEENABLE, old_d3drs_colorwriteenable );
		device->SetRenderState( D3DRS_SRGBWRITEENABLE, true );
		device->SetVertexDeclaration( vert_declaration );
		device->SetVertexShader( vert_shader );

		return present_original( device, src, dest, window_override, dirty_region );
	}

	void __stdcall client::frame_stage_notify( client_frame_stage_t frame_stage ) noexcept {
		


		frame_stage_notify_original( sdk::client, frame_stage );
	}

	void __stdcall hooks::engine::emit_sound( i_recipient_filter& filter, int ent_index, int channel, const char* entry, unsigned int hash, const char* sample, float vol, int seed, float attenuation, int flags, int pitch, const vec3_t* origin, const vec3_t* direction, void* utl_origin, bool update_pos, float sound_time, int speaker_entity, int unk ) noexcept {
		
		if ( fnv::equal( entry, "UIPanorama.popup_accept_match_beep" ) && config::auto_accept ) {
			sdk::set_local_player_ready( "" );
			auto* const window{ FindWindowA( "Valve001", nullptr ) };
			FLASHWINFO flash{ sizeof FLASHWINFO, window, FLASHW_TRAY | FLASHW_TIMERNOFG, 0, 0 };
			FlashWindowEx( &flash );
			ShowWindow( window, SW_RESTORE );
		}

		emit_sound_original( sdk::engine_sound, filter, ent_index, channel, entry, hash, sample, vol, seed, attenuation, flags, pitch, origin, direction, utl_origin, update_pos, sound_time, speaker_entity, unk );
	}

	int __stdcall custom_layout_popup_parameters::hook( int a1, char* popup_file, char* popup_params ) noexcept {
		
		if ( char* occurance = strstr( popup_params, "false" ); occurance && config::auto_accept )
			strncpy_s( occurance, 5, "true", 4 ); // strncpy deprecated.

		return original( a1, popup_file, popup_params );
	}

	void __stdcall surface::lock_cursor( ) noexcept {

		if ( gui::is_open )
			return sdk::surface->unlock_cursor( );

		lock_cursor_original( sdk::surface );
	}

	bool __fastcall sv_cheats::hook( void* convar ) noexcept {

		if ( _ReturnAddress( ) == sdk::cam_think && config::thirdperson )
			return true;

		return original( convar );
	}
}