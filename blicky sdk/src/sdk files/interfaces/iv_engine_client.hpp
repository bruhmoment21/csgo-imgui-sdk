#pragma once
#include "../virtual_method.hpp"

struct player_info_t {

	__int64 unknown;
	union {
		__int64 steam_id_64;
		struct {
			__int32 xuid_low;
			__int32 xuid_high;
		};
	};
	
	char name[ 128 ];
	int user_id;
	char steam_id[ 20 ];
	char pad[ 16 ];
	unsigned long isteam_id;
	char friends_name[ 128 ];
	bool fake_player; // is bot
	bool is_hltv;
	unsigned int custom_files[ 4 ];
	unsigned char files_downloaded;
};

class iv_engine_client {
public:
	int max_clients( ) { // Always 64.
		return virtual_method::call_virtual< int, 20 >( this );
	}

	bool is_in_game( ) {
		return virtual_method::call_virtual< bool, 26 >( this );
	}

	bool is_connected( ) {
		return virtual_method::call_virtual< bool, 27 >( this );
	}

	bool connected_and_in_game( ) {
		if ( is_in_game( ) && is_connected( ) )
			return true;

		return false;
	}
};