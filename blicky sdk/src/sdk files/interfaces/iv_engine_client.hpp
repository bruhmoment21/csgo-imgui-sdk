#pragma once

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
	char pad[ 0x10 ];
	unsigned long isteam_id;
	char friends_name[ 128 ];
	bool fake_player; // is bot
	bool is_hltv;
	unsigned int custom_files[ 4 ];
	unsigned char files_downloaded;
};

class iv_engine_client {
public:
	int max_clients( ) {
		using original_fn = int( __thiscall* )( iv_engine_client* );
		return ( *reinterpret_cast< original_fn** >( this ) )[ 20 ]( this );
	}
};