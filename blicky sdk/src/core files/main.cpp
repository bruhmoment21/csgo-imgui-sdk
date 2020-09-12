#include "../sdk files/sdk.hpp"
#include "hooks/hooks.hpp"

#include <thread>

static uint32_t alloc_count;

void* operator new( size_t size ) {		// Keep track of heap allocations
	++alloc_count;
	return malloc( size );
}

DWORD __stdcall StartAddress( LPVOID instance ) {

	while ( !GetModuleHandleA( "serverbrowser.dll" ) ) // Checks if CS:GO has been fully loaded.
		std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

	console::allocate( );

	try {
		sdk::initialize( );
		hooks::initialize( );

		console::log( "Cheat has been loaded successfully!", console_logs::successful_log );
	} catch ( const std::runtime_error& ex ) {
		if ( !sdk::engine->console_opened( ) && config::log_in_csgo ) sdk::engine->cmd_unrestricted( "toggleconsole" );
		console::log( ex.what( ), console_logs::error_log );

		FreeLibraryAndExitThread( static_cast< HMODULE >( instance ), 1 );
	}

	console::log( "Heap allocations: %u", console_logs::info_log, alloc_count );

	while ( !GetAsyncKeyState( VK_END ) )
		std::this_thread::sleep_for( std::chrono::milliseconds( 50 ) );

	FreeLibraryAndExitThread( static_cast< HMODULE >( instance ), 1 );
}

BOOL __stdcall DllMain( const HMODULE instance, const DWORD reason, LPVOID reserved ) {

	if ( reason == DLL_PROCESS_ATTACH ) {
		DisableThreadLibraryCalls( instance );
		CreateThread( nullptr, NULL, StartAddress, instance, NULL, nullptr );
	} else if ( reason == DLL_PROCESS_DETACH && !reserved ) {
		hooks::release( );
		console::log( "Cheat has been unloaded successfully!", console_logs::successful_log );
		console::release( );
	}

	return TRUE;
}
