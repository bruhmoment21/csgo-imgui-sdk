#pragma once

#include "utilities.hpp"

// pxnch420 - unknowncheats
class i_game_ui
{
public:
	bool is_game_ui_visible()
	{
		return utilities::call_virtual<std::uint8_t, 14, int>(this, 16);
	}
};