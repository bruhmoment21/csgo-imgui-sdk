#pragma once

#include <unordered_map>

namespace fnv_1a {

	template< typename S >
	struct fnv_internal;
	template< typename S >
	struct fnv1a;

	template< >
	struct fnv_internal< std::uint32_t > {
		constexpr static std::uint32_t default_offset_basis = 0x811C9DC5;
		constexpr static std::uint32_t prime = 0x01000193;
	};

	template< >
	struct fnv1a< std::uint32_t > : fnv_internal< std::uint32_t > {
		constexpr static std::uint32_t hash( char const* string, const std::uint32_t val = default_offset_basis ) {
			return ( string[ 0 ] == '\0' ) ? val : hash( &string[ 1 ], ( val ^ std::uint32_t( string[ 0 ] ) ) * prime );
		}

		constexpr static std::uint32_t hash( wchar_t const* string, const uint32_t val = default_offset_basis ) {
			return ( string[ 0 ] == L'\0' ) ? val : hash( &string[ 1 ], ( val ^ std::uint32_t( string[ 0 ] ) ) * prime );
		}
	};
}

using fnv = fnv_1a::fnv1a< std::uint32_t >;

namespace netvar_manager {

	// soon
}