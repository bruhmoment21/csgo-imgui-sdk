#include "utilities.hpp"

#include <Windows.h>
#include <stdexcept>
#include <vector>

void utilities::allocate_console( ) {
	
	AllocConsole( );

	freopen_s( reinterpret_cast< FILE** >stdin, "conin$", "r", stdin );
	freopen_s( reinterpret_cast< FILE** >stdout, "conout$", "w", stdout );
	
	SetConsoleTitleA( "blicky sdk" );
}

void utilities::free_console( ) {
	
	fclose( static_cast< FILE* >( stdin ) );
	fclose( static_cast< FILE* >( stdout ) );

	FreeConsole( );
}

std::uint8_t* utilities::pattern_scan( const char* module, const char* signature ) {
	
    static auto pattern_to_byte = [ ]( const char* pattern ) {
        auto bytes = std::vector<int>{};
        auto* const start = const_cast< char* >( pattern );
        auto* const end = const_cast< char* >( pattern ) + strlen( pattern );

        for ( auto* current = start; current < end; ++current ) {
            if ( *current == '?' ) {
                ++current;
                if ( *current == '?' ) ++current;
            	
                bytes.push_back( -1 );
            } else {
                bytes.push_back( strtoul( current, &current, 16 ) );
            }
        }

        return bytes;
    };

    auto* const dos_headers = reinterpret_cast< PIMAGE_DOS_HEADER >( GetModuleHandleA( module ) );
    auto* const nt_headers = reinterpret_cast< PIMAGE_NT_HEADERS >( reinterpret_cast< std::uint8_t* >( GetModuleHandleA( module ) ) + dos_headers->e_lfanew );

    const auto size_of_image = nt_headers->OptionalHeader.SizeOfImage;
    auto pattern_bytes = pattern_to_byte( signature );
    auto* const scan_bytes = reinterpret_cast< std::uint8_t* >( GetModuleHandleA( module ) );

    const auto size = pattern_bytes.size( );
    auto* const data = pattern_bytes.data( );

    for ( auto i = 0ul; i < size_of_image - size; ++i ) {
        bool found = true;
        for ( auto j = 0ul; j < size; ++j ) {
            if ( scan_bytes[ i + j ] != data[ j ] && data[ j ] != -1 ) {
                found = false;
                break;
            }
        } if ( found ) return &scan_bytes[ i ];
    }

    throw std::runtime_error( std::string( "Wrong signature: " ) + signature );
}
