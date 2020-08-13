#pragma once

#include <cstdint>

namespace utilities {

	void allocate_console( ) noexcept;
	void free_console( ) noexcept;
	std::uint8_t* pattern_scan( const char* module_name, const char* signature );

	template <typename T>
	inline constexpr auto relative_pattern_scan( const char* module_name, const char* signature, int offset = 0 ) {

		auto* const address = pattern_scan( module_name, signature ) + offset;
		return reinterpret_cast< T >( address + 4 + *reinterpret_cast< std::int32_t* >( address ) );
	}
}
