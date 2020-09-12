#include "utilities.hpp"

#include "../../sdk files/sdk.hpp"

#include <stdexcept>
#include <vector>

namespace utilities {
    std::uint8_t* pattern_scan( const char* module_name, const char* signature ) {
        const auto pattern_to_byte = [ & ]( ) {
            auto bytes{ std::vector< int >{ } };

            const auto pattern_length{ strlen( signature ) };
            bytes.reserve( pattern_length );

            char* const start{ const_cast< char* >( signature ) };
            char* const end{ start + pattern_length };

            for ( char* current{ start }; current < end; ++current ) {
                if ( *current == '?' ) {
                    ++current;
                    if ( *current == '?' ) ++current;

                    bytes.emplace_back( -1 );
                } else {
                    bytes.emplace_back( strtoul( current, &current, 16 ) );
                }
            }

            return bytes;
        };

        void* const module{ GetModuleHandleA( module_name ) };
        if ( !module ) throw std::runtime_error( module_name + std::string{ " is not a good module!" } );

        auto* const dos_headers{ static_cast< PIMAGE_DOS_HEADER >( module ) };
        auto* const nt_headers{ reinterpret_cast< PIMAGE_NT_HEADERS >( static_cast< std::uint8_t* >( module ) + dos_headers->e_lfanew ) };

        const auto size_of_image{ nt_headers->OptionalHeader.SizeOfImage };
        const auto pattern_bytes{ pattern_to_byte( ) };
        auto* const scan_bytes{ static_cast< std::uint8_t* >( module ) };

        const auto size{ pattern_bytes.size( ) };
        auto* const data{ pattern_bytes.data( ) };

        for ( auto i{ 0ul }; i < size_of_image - size; ++i ) {

            bool found{ true };
            for ( auto j{ 0ul }; j < size; ++j ) {
                if ( scan_bytes[ i + j ] != data[ j ] && data[ j ] != -1 ) {
                    found = false;
                    break;
                }
            } if ( found ) {

                console::log( "%s => 0x%x ", console_logs::info_log, signature, &scan_bytes[ i ] );
                return &scan_bytes[ i ];
            }
        }

        throw std::runtime_error( signature + std::string{ " is not a good signature!" } );
    }
}