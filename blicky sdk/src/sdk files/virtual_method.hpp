#pragma once

#include <cstddef>

namespace virtual_method {

    template <typename T, std::size_t index, typename ...Args>
    constexpr auto call_virtual( void* classBase, Args... args ) noexcept {
        return ( ( *reinterpret_cast< T( __thiscall*** )( void*, Args... ) >( classBase ) )[ index ] )( classBase, args... );
    }
}