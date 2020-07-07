#pragma once
#include <cstdint>

namespace utilities {
	void allocate_console( );
	void free_console( );
	std::uint8_t* pattern_scan( const char* module, const char* signature );
}
