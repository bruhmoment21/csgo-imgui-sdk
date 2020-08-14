#include "hooks.hpp"

#include "../../other files/minhook/minhook.h"
#include "../config/config_system.hpp"
#include "../../sdk files/sdk.hpp"
#include "../hacks/misc/misc.hpp"

#include "../../other files/imgui/imgui.h"
#include "../../other files/imgui/imgui_impl_dx9.h"
#include "../../other files/imgui/imgui_impl_win32.h"

#include <intrin.h>

namespace hooks {

	bool context_created{ false }; // Prevents present hook running before WNDPROC hook.

	WNDPROC original_wnd_proc{ nullptr };

	// Used in present hook to fix server browser not showing after injecting.
	IDirect3DVertexDeclaration9* vert_declaration{ nullptr };
	IDirect3DVertexShader9* vert_shader{ nullptr };
	DWORD old_d3drs_colorwriteenable{ NULL };

	void initialize( ) noexcept {

		void* const reset_target{ **reinterpret_cast< void*** >( utilities::pattern_scan( "gameoverlayrenderer.dll", "FF 15 ? ? ? ? 8B F8 85 FF 78 18" ) + 2 ) };
		void* const present_target{ **reinterpret_cast< void*** >( utilities::pattern_scan( "gameoverlayrenderer.dll", "FF 15 ? ? ? ? 8B F8 85 DB" ) + 2 ) };
		void* const frame_stage_notify_target{ get_virtual( interfaces::client, 37 ) };
		void* const lock_cursor_target{ get_virtual( interfaces::surface, 67 ) };
		void* const sv_cheats_target{ get_virtual( interfaces::console->get_convar( "sv_cheats" ), 13 ) };

		original_wnd_proc = reinterpret_cast< WNDPROC >( SetWindowLongA( FindWindowA( "Valve001", nullptr ), GWL_WNDPROC, reinterpret_cast< LONG >( menu::wnd_proc ) ) );

		MH_Initialize( );

		MH_CreateHook( reset_target, static_cast< LPVOID >( &menu::reset ), reinterpret_cast< void** >( &menu::reset_original ) );
		MH_CreateHook( present_target, static_cast< LPVOID >( &menu::present ), reinterpret_cast< void** >( &menu::present_original ) );
		MH_CreateHook( frame_stage_notify_target, static_cast< LPVOID >( &client::frame_stage_notify ), reinterpret_cast< void** >( &client::frame_stage_notify_original ) );
		MH_CreateHook( lock_cursor_target, static_cast< LPVOID >( &surface::lock_cursor ), reinterpret_cast< void** >( &surface::lock_cursor_original ) );
		MH_CreateHook( sv_cheats_target, static_cast< LPVOID >( &sv_cheats::hook ), reinterpret_cast< void** >( &sv_cheats::original ) );

		MH_EnableHook( nullptr );

		std::cout << "Hooks are good!\n";
	}

	void release( ) noexcept {

		MH_Uninitialize( );

		MH_DisableHook( nullptr );

		SetWindowLongA( FindWindowA( "Valve001", nullptr ), GWL_WNDPROC, reinterpret_cast< LONG >( original_wnd_proc ) );

		interfaces::input_system->enable_input( true );

		ImGui_ImplDX9_Shutdown( );
		ImGui_ImplWin32_Shutdown( );
		ImGui::DestroyContext( );
	}

	LRESULT __stdcall menu::wnd_proc( HWND window, UINT msg, WPARAM wparm, LPARAM lparm ) noexcept {

		if ( !gui::initialized ) {

			ImGui::CreateContext( );
			ImGui_ImplWin32_Init( window );
			gui::init( );
			context_created = true;
		}

		LRESULT ImGui_ImplWin32_WndProcHandler( HWND hwnd, UINT msg, WPARAM wparm, LPARAM lparm );
		ImGui_ImplWin32_WndProcHandler( window, msg, wparm, lparm );
		interfaces::input_system->enable_input( !gui::is_open );

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

		/* uncomment this if you wish to disable anti aliasing. see gui.cpp too
		* device->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, FALSE );
		* device->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE, FALSE );
		*/

		ImGui_ImplDX9_Init( device );

		ImGui_ImplDX9_NewFrame( );
		ImGui_ImplWin32_NewFrame( );
		ImGui::NewFrame( );

		if ( gui::is_open )
			gui::render( );

		misc::thirdperson( );

		if ( ImGui::IsKeyPressed( VK_INSERT, false ) ) {

			gui::is_open = !gui::is_open;
			if ( !gui::is_open )
				interfaces::input_system->reset_input_state( );
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
		
		frame_stage_notify_original( interfaces::client, frame_stage );
	}

	void __stdcall surface::lock_cursor( ) noexcept {

		if ( gui::is_open )
			return interfaces::surface->unlock_cursor( );

		lock_cursor_original( interfaces::surface );
	}

	bool __fastcall sv_cheats::hook( void* convar, int edx ) noexcept {

		static auto cam_think{ utilities::pattern_scan( "client.dll", "85 C0 75 30 38 86" ) };

		if ( _ReturnAddress( ) == cam_think && config::thirdperson )
			return true;

		return original( convar );
	}
}