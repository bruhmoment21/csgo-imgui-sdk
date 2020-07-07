#include "hooks.hpp"

#include "../../other files/minhook/minhook.h"
#include "../../sdk files/sdk.hpp"

static bool context_created, should_lock_cursor{ true };

hooks::menu::present_fn present_original = nullptr;
hooks::menu::reset_fn reset_original = nullptr;

hooks::surface::lock_cursor_fn lock_cursor_original = nullptr;

static WNDPROC original_wnd_proc;

// used in present hook to fix server browser not showing when injecting
static IDirect3DVertexDeclaration9* vert_declaration;
static IDirect3DVertexShader9* vert_shader;
static DWORD old_d3drs_colorwriteenable;

void hooks::initialize( ) {

	auto* const lock_cursor_target = reinterpret_cast< void* >( get_virtual( interfaces::surface, 67 ) );

	original_wnd_proc = reinterpret_cast< WNDPROC >( SetWindowLongPtrA( FindWindowA( "Valve001", nullptr ), GWL_WNDPROC, reinterpret_cast< LONG >( menu::wnd_proc ) ) );
	
	MH_Initialize( );

	MH_CreateHook( **reinterpret_cast< void*** >( utilities::pattern_scan( GetModuleHandleA( "gameoverlayrenderer.dll" ), "FF 15 ? ? ? ? 8B F8 85 FF 78 18" ) + 2 ), reinterpret_cast< LPVOID >( &menu::reset ), reinterpret_cast< void** >( &reset_original ) );
	MH_CreateHook( **reinterpret_cast< void*** >( utilities::pattern_scan( GetModuleHandleA( "gameoverlayrenderer.dll" ), "FF 15 ? ? ? ? 8B F8 85 DB" ) + 2 ), reinterpret_cast< LPVOID >( &menu::present ), reinterpret_cast< void** >( &present_original ) );
	MH_CreateHook( lock_cursor_target, reinterpret_cast< LPVOID >( &surface::lock_cursor ), reinterpret_cast< void** >( &lock_cursor_original ) );
	
	MH_EnableHook( nullptr );

	std::cout << "Hooks are good!\n";
}

void hooks::release( ) {

	MH_Uninitialize( );

	MH_DisableHook( nullptr );

	SetWindowLongPtrA( FindWindowA( "Valve001", nullptr ), GWL_WNDPROC, reinterpret_cast< LONG >( original_wnd_proc ) );

	interfaces::input_system->enable_input( true );

	ImGui_ImplDX9_Shutdown( );
	ImGui_ImplWin32_Shutdown( );
	ImGui::DestroyContext( );
}

LRESULT WINAPI hooks::menu::wnd_proc( HWND window, UINT msg, WPARAM wparm, LPARAM lparm ) {

	if ( !gui ) { // checks if gui is nullptr
		ImGui::CreateContext( );
		ImGui_ImplWin32_Init( window );
		gui = std::make_unique<c_gui>( ); // after this line has been hit this code block won't run anymore
		context_created = true; // if you delete this bool present hook will run before wnd_proc hook and will result in a error(exception) and your game will crash
		// it crashes because Context isn't created
	}
	
	LRESULT ImGui_ImplWin32_WndProcHandler( HWND hwnd, UINT msg, WPARAM wparm, LPARAM lparm );
	ImGui_ImplWin32_WndProcHandler( window, msg, wparm, lparm );
	interfaces::input_system->enable_input( !gui->is_open );
	
	return CallWindowProcA( original_wnd_proc, window, msg, wparm, lparm );
}

HRESULT D3DAPI hooks::menu::reset( IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params ) {

	ImGui_ImplDX9_InvalidateDeviceObjects( );
	return reset_original( device, params );
}

HRESULT D3DAPI hooks::menu::present( IDirect3DDevice9* device, const RECT* src, const RECT* dest, HWND window_override, const RGNDATA* dirty_region ) {

	if ( !context_created ) return FALSE; // this prevents this hook from running before wndproc
	
	// save state so you see server browser
	device->GetRenderState( D3DRS_COLORWRITEENABLE, &old_d3drs_colorwriteenable );
	device->GetVertexDeclaration( &vert_declaration );
	device->GetVertexShader( &vert_shader );
	device->SetRenderState( D3DRS_COLORWRITEENABLE, 0xffffffff );
	device->SetRenderState( D3DRS_SRGBWRITEENABLE, false );
	device->SetSamplerState( NULL, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
	device->SetSamplerState( NULL, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );
	device->SetSamplerState( NULL, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP );
	device->SetSamplerState( NULL, D3DSAMP_SRGBTEXTURE, NULL );
	
	ImGui_ImplDX9_Init( device );
	
	/* uncomment this if you wish to disable anti aliasing. see gui.cpp too
	* device->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, FALSE );
	* device->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE, FALSE );
	*/

	ImGui_ImplDX9_NewFrame( );
	ImGui_ImplWin32_NewFrame( );
	ImGui::NewFrame( );

	if( gui->is_open )
		gui->render( );
	
	if ( ImGui::IsKeyPressed( VK_INSERT, false ) ) {
		gui->is_open = !gui->is_open;
		should_lock_cursor = true;
		if ( !gui->is_open ) {
			interfaces::input_system->reset_input_state( );
			should_lock_cursor = false;
		}
	}

	ImGui::EndFrame( );
	ImGui::Render( );

	if ( device->BeginScene( ) == D3D_OK ) {
		ImGui_ImplDX9_RenderDrawData( ImGui::GetDrawData( ) );
		device->EndScene( );
	}

	// restore state save state so you see server browser
	device->SetRenderState( D3DRS_COLORWRITEENABLE, old_d3drs_colorwriteenable );
	device->SetRenderState( D3DRS_SRGBWRITEENABLE, true );
	device->SetVertexDeclaration( vert_declaration );
	device->SetVertexShader( vert_shader );
	
	return present_original( device, src, dest, window_override, dirty_region );
}

void __stdcall hooks::surface::lock_cursor( ) noexcept {

	if ( should_lock_cursor ) // if you use gui->open instead of this bool you'll get a crash
		return interfaces::surface->unlock_cursor( );

	lock_cursor_original( interfaces::surface );
}
