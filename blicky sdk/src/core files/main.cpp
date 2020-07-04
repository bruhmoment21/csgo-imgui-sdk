#include "../sdk files/sdk.hpp"

DWORD WINAPI StartAddress( LPVOID instance ) {

	utilities::allocate_console( );

	try {
		interfaces::initialize( );
		hooks::initialize( );
	} catch ( const std::runtime_error& ex ) {
		std::cout << "Exception: \n\n" << ex.what( ) << std::endl << std::endl;
		system( "pause" );
		FreeLibraryAndExitThread( static_cast< HMODULE >( instance ), 1 );
	}

	while ( !GetAsyncKeyState( VK_END ) )
		std::this_thread::sleep_for( std::chrono::milliseconds( 50 ) );

	FreeLibraryAndExitThread( static_cast< HMODULE >( instance ), 1 );
}

BOOL WINAPI DllMain( const HMODULE instance, const DWORD reason, [[maybe_unused]] LPVOID reserved ) {

	if ( reason == DLL_PROCESS_ATTACH ) {
		CreateThread( nullptr, NULL, StartAddress, instance, NULL, nullptr );
	} else if ( reason == DLL_PROCESS_DETACH ) {
		hooks::release( );
		utilities::free_console( );
	}

	return TRUE;
}