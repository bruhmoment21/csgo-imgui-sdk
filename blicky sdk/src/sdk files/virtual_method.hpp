#pragma once

#include <cstddef>

namespace virtual_method {

    template <typename T, std::size_t index, typename ...Arguments>
    inline constexpr auto call_virtual( void* class_base, Arguments... args ) noexcept {
        return ( ( *static_cast< T( __thiscall*** )( void*, Arguments... ) >( class_base ) )[ index ] )( class_base, args... );
    }
}