#pragma once

#include "../classes/econ_item.hpp"
#include "../virtual_method.hpp"

class entity_t;

class c_shared_object_type_cache {
public:
	void add_object( void* obj ) {
		return virtual_method::call_virtual< void, 1, void* >( this, obj );
	}
};

class c_player_inventory { // NEEDS REWRITE
public:
	c_shared_object_type_cache* get_base_type_cache( ) {
		static auto fn_find_so_cache = reinterpret_cast< std::uintptr_t( __thiscall* )( std::uintptr_t, std::uint64_t, std::uint64_t, bool ) >( utilities::pattern_scan( "client.dll", "55 8B EC 83 E4 F8 83 EC 1C 0F 10 45 08" ) );

		const auto client_system = **reinterpret_cast< std::uintptr_t** >( utilities::pattern_scan( "client.dll", "8B 0D ? ? ? ? 6A ? 83 EC 10" ) + 0x2 );
		const auto so_cache = fn_find_so_cache( client_system + 0x70, *reinterpret_cast< std::uint64_t* >( this + 0x8 ), *reinterpret_cast< std::uint64_t* >( this + 0x10 ), false );

		const auto unk1 = *reinterpret_cast< DWORD* >( so_cache + 0x1C );
		const auto unk2 = *reinterpret_cast< DWORD* >( so_cache + 0x10 );

		static auto fn_find_base_type_cache = utilities::pattern_scan( "client.dll", "55 8B EC 56 57 8B F2 8B F9" );
		const auto unk3 = unk2 + 4 * unk1;
		c_shared_object_type_cache** cache;
		
		const int t = 1;
		__asm {
			push this;
			lea eax, [ t ];
			push eax;
			mov ecx, unk2;
			mov edx, unk3;
			call fn_find_base_type_cache;
			mov cache, eax;
			add esp, 8;
		}

		return *cache;
	}

	std::uint32_t get_steam_id( ) {
		return *reinterpret_cast< std::uint32_t* >( this + 0x8 );
	}

	bool add_econ_item( c_econ_item* item, int a3, int a4, char a5 ) {
		static auto fn = reinterpret_cast< c_econ_item * ( __thiscall* )( void*, c_econ_item*, int, int, char ) >( utilities::pattern_scan( "client.dll", "55 8B EC 83 E4 F8 A1 ? ? ? ? 83 EC 14 53 56 57 8B F9 8B 08" ) );
		get_base_type_cache( )->add_object( item );

		return fn( this, item, a3, a4, a5 );
	}

	entity_t* get_item_in_loadout( unsigned int team, signed int slot ) {
		return virtual_method::call_virtual< entity_t*, 8, unsigned int, signed int >( this, team, slot );
	}

	entity_t* get_inventory_item_by_item_id( std::int64_t item_id ) {
		static auto fn = ( entity_t * ( __thiscall* )( void*, std::int64_t ) )( utilities::pattern_scan( "client.dll", "55 8B EC 8B 55 08 83 EC 10 8B C2" ) );
		auto* const econ = fn( this, item_id );
		if ( !econ || !*reinterpret_cast< BYTE* >( reinterpret_cast< unsigned int >( econ ) + 0x204 ) )
			return nullptr;

		return econ;
	}

	bool equip_item_in_loadout( int team, int slot, unsigned long long item_id ) {
		static auto fn = ( bool( __thiscall* )( void*, int, int, unsigned long long, bool ) )( utilities::pattern_scan( "client.dll", "55 8B EC 83 E4 F8 83 EC 24 83 3D ? ? ? ? ? 53 56 57 8B F9" ) );
		return fn( this, team, slot, item_id, true );
	}
};