#pragma once

#include <unordered_map>

namespace fnv_1a {

	struct fnv_internal {
		constexpr static std::uint32_t default_offset_basis = 0x811C9DC5;
		constexpr static std::uint32_t prime = 0x01000193;
	};

	struct fnv1a : fnv_internal {
		constexpr static std::uint32_t hash( const char* string, const std::uint32_t& val = default_offset_basis ) {
			return ( string[ 0 ] == '\0' ) ? val : hash( &string[ 1 ], ( val ^ std::uint32_t( string[ 0 ] ) ) * prime );
		}

		constexpr static bool equal( const char* string, const char* string2 ) {
			return hash( string ) == hash( string2 );
		}
	};
}

using fnv = fnv_1a::fnv1a;

namespace netvar_manager {

	// soon
}