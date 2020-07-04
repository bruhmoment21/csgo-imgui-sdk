#include "hooks.hpp"
#include "../../other files/minhook/minhook.h"

static bool context_created = false, should_lock_cursor = true;

hooks::menu::present_fn present_original = nullptr;
hooks::menu::reset_fn reset_original = nullptr;

hooks::surface::lock_cursor_fn lock_cursor_original = nullptr;

WNDPROC	hooks::original_wnd_proc;

void hooks::initialize( ) {

	auto* const lock_cursor_target = reinterpret_cast< void* >( get_virtual( interfaces::surface, 67 ) );

	original_wnd_proc = reinterpret_cast< WNDPROC >( SetWindowLongPtrA( FindWindowA( "Valve001", nullptr ), GWL_WNDPROC, reinterpret_cast< LONG >( menu::wnd_proc ) ) );
	
	MH_Initialize( );

	MH_CreateHook( **reinterpret_cast< void*** >( utilities::pattern_scan( GetModuleHandleA( "gameoverlayrenderer.dll" ), "FF 15 ? ? ? ? 8B F8 85 FF 78 18" ) + 2 ), &menu::reset, reinterpret_cast< void** >( &reset_original ) );
	MH_CreateHook( **reinterpret_cast< void*** >( utilities::pattern_scan( GetModuleHandleA( "gameoverlayrenderer.dll" ), "FF 15 ? ? ? ? 8B F8 85 DB" ) + 2 ), &menu::present, reinterpret_cast< void** >( &present_original ) );
	MH_CreateHook( lock_cursor_target, &surface::lock_cursor, reinterpret_cast< void** >( &lock_cursor_original ) );
	
	MH_EnableHook( nullptr );

	std::cout << "Hooks are good!" << std::endl;
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

	static bool once;
	if ( !once ) {
		ImGui::CreateContext( );
		ImGui_ImplWin32_Init( window );
		gui = std::make_unique<c_gui>( );
		once = true;
		context_created = true;
	}
	
	LRESULT ImGui_ImplWin32_WndProcHandler( HWND hwnd, UINT msg, WPARAM wparm, LPARAM lparm );
	ImGui_ImplWin32_WndProcHandler( window, msg, wparm, lparm );
	interfaces::input_system->enable_input( !gui->is_open );
	
	return CallWindowProcW( original_wnd_proc, window, msg, wparm, lparm );
}

HRESULT D3DAPI hooks::menu::reset( IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params ) {

	ImGui_ImplDX9_InvalidateDeviceObjects( );
	return reset_original( device, params );
}

HRESULT D3DAPI hooks::menu::present( IDirect3DDevice9* device, const RECT* src, const RECT* dest, HWND window_override, const RGNDATA* dirty_region ) {

	if ( !context_created ) return FALSE;
	
	ImGui_ImplDX9_Init( device );
	
	device->SetRenderState( D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE );
	IDirect3DVertexDeclaration9* vertexDeclaration;
	device->GetVertexDeclaration( &vertexDeclaration );

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
	ImGui::GetIO( ).MouseDrawCursor = gui->is_open;

	ImGui::EndFrame( );
	ImGui::Render( );

	if ( device->BeginScene( ) == D3D_OK ) {
		ImGui_ImplDX9_RenderDrawData( ImGui::GetDrawData( ) );
		device->EndScene( );
	}
	
	return present_original( device, src, dest, window_override, dirty_region );
}

void __stdcall hooks::surface::lock_cursor( ) noexcept {

	if ( should_lock_cursor )
		return interfaces::surface->unlock_cursor( );

	lock_cursor_original( interfaces::surface );
}