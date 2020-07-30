#pragma once
#include <cstdint>

namespace utilities {
	void allocate_console( ) noexcept;
	void free_console( ) noexcept;
	std::uint8_t* pattern_scan( const char* module_name, const char* signature );
}
