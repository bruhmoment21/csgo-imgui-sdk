#pragma once

#include <cstddef>

// Use this for convars as index when calling get_virtual.
constexpr int index_convar = 13;

namespace virtual_method {

    template <typename T, std::size_t index, typename ...Arguments>
    inline constexpr auto call_virtual( void* const class_base, Arguments... args ) noexcept {
        return ( ( *static_cast< T( __thiscall*** )( void*, Arguments... ) >( class_base ) )[ index ] )( class_base, args... );
    }

    inline void* get_virtual( void* const class_name, const unsigned int index ) noexcept {
        return reinterpret_cast< void* >( ( *static_cast< int** >( class_name ) )[ index ] );
    }
}