#pragma once

#include "../../other files/utilities/utilities.hpp"

#include <type_traits>
#include <windows.h>
#include <cstdint>

enum class item_quality : int {
	item_quality_default,
	item_quality_genuine,
	item_quality_vintage,
	item_quality_unusual,
	item_quality_unknown,
	item_quality_community,
	item_quality_developer,
	item_quality_selfmade,
	item_quality_customized,
	item_quality_strange,
	item_quality_completed,
	item_quality_unknown2,
	item_quality_tournament
};

enum class item_rarity : int {

	item_rarity_default,
	item_rarity_common,
	item_rarity_uncommon,
	item_rarity_rare,
	item_rarity_mythical,
	item_rarity_legendary,
	item_rarity_ancient,
	item_rarity_immortal
};

class c_econ_item {
	unsigned short* get_econ_item_data( ) {
		return reinterpret_cast< unsigned short* >( this + 0x26 );
	}

public:
	c_econ_item* create_econ_item( ) {
		static auto fn{ reinterpret_cast< std::add_pointer_t< c_econ_item* __stdcall( ) > >( *reinterpret_cast< uintptr_t* >( utilities::pattern_scan( "client.dll", "C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? E8 ? ? ? ? 83 F8 FF 75 09 8D 45 E4 50 E8 ? ? ? ? 8D 45 E4 C7 45 ? ? ? ? ? 50 C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? E8 ? ? ? ? 83 F8 FF 75 09 8D 45 E4 50 E8 ? ? ? ? 8D 45 E4 C7 45 ? ? ? ? ? 50 C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? E8 ? ? ? ? 83 F8 FF 75 09 8D 45 E4 50 E8 ? ? ? ? 8D 45 E4 C7 45 ? ? ? ? ? 50 C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? E8 ? ? ? ? 83 F8 FF 75 09 8D 45 E4 50 E8 ? ? ? ? 8D 45 E4" ) + 3 ) ) };
		return fn( );
	}

	std::uintptr_t get_item_schema( ) {
		static auto fn{ reinterpret_cast< std::add_pointer_t< std::uintptr_t __stdcall( ) > >( utilities::pattern_scan( "client.dll", "A1 ? ? ? ? 85 C0 75 53" ) ) };
		return fn( );
	}

	std::uint16_t* get_definition_index( ) {
		return reinterpret_cast< std::uint16_t* >( this + 0x24 );
	}

	std::uint32_t* get_account_id( ) {
		return reinterpret_cast< std::uint32_t* >( this + 0x1C );
	}

	std::uint32_t* get_inventory( ) {
		return reinterpret_cast< std::uint32_t* >( this + 0x20 );
	}

	std::uint64_t* get_item_id( ) {
		return reinterpret_cast< std::uint64_t* >( this + 0x8 );
	}

	std::uint64_t* get_original_id( ) {
		return reinterpret_cast< std::uint64_t* >( this + 0x10 );
	}

	unsigned char* get_flags( ) {
		return reinterpret_cast< unsigned char* >( this + 0x30 );
	}

	void add_sticker( const int index, const int kit, const float wear, const float scale, const float rotation ) {
		set_attribute_value<int>( 113 + 4 * index, kit );
		set_attribute_value<float>( 114 + 4 * index, wear );
		set_attribute_value<float>( 115 + 4 * index, scale );
		set_attribute_value<float>( 116 + 4 * index, rotation );
	}

	/*void set_stattrak( const int val ) {				// NEEDS FIX PROBABLY
		set_attribute_value<int>( 80, 150 );
		set_attribute_value<int>( 81, 0 );
		set_quality( item_quality::ITEM_QUALITY_STRANGE );
	}*/

	void set_paint_kit( const float kit ) {
		set_attribute_value<float>( 6, kit );
	}
	int get_paint_kit( ) {
		static int kit{ NULL };
		get_attribute_value( 6, kit );
		return kit;
	}
	void set_paint_seed( const float seed ) {
		set_attribute_value<float>( 7, seed );
	}

	void set_paint_wear( const float wear ) {
		set_attribute_value<float>( 8, wear );
	}

	void set_quality( const item_quality quality ) {
		const auto data{ *get_econ_item_data( ) };
		*get_econ_item_data( ) = data ^ ( data ^ 32 * static_cast< int >( quality ) ) & 0x1E0;
	}

	void set_rarity( const item_rarity rarity ) {
		const auto data{ *get_econ_item_data( ) };
		*get_econ_item_data( ) = ( data ^ ( static_cast< int >( rarity ) << 11 ) ) & 0x7800 ^ data;
	}

	void set_origin( const int origin ) {
		const auto data{ *get_econ_item_data( ) };
		*get_econ_item_data( ) = data ^ ( static_cast< unsigned __int8 >( data ) ^ static_cast< unsigned __int8 >( origin ) ) & 0x1F;
	}

	void set_level( const int level ) {
		const auto data{ *get_econ_item_data( ) };
		*get_econ_item_data( ) = data ^ ( data ^ ( level << 9 ) ) & 0x600;
	}
	bool get_in_use( ) {
		const auto data{ *get_econ_item_data( ) };
		return *get_econ_item_data( ) = data & 0x7FFF;
	}
	void set_in_use( const bool in_use ) {
		const auto data{ *get_econ_item_data( ) };
		*get_econ_item_data( ) = data & 0x7FFF | ( in_use << 15 );
	}

	void set_custom_name( const char* name ) {
		static auto address{ utilities::pattern_scan( "client.dll", "E8 ? ? ? ? 8B 46 78 C1 E8 0A A8 01 74 13 8B 46 34" ) };
		static auto fn{ reinterpret_cast< c_econ_item * ( __thiscall* )( void*, const char* ) >( *reinterpret_cast< uintptr_t* >( address + 1 ) + address + 5 ) };

		fn( this, name );
	}

	template<typename T>
	void set_attribute_value( const int index, T val ) {
		const auto v15{ reinterpret_cast< DWORD* >( get_item_schema( ) ) };
		const auto v16{ *reinterpret_cast< DWORD* >( v15[ 72 ] + 4 * index ) };

		static auto fn{ reinterpret_cast< int( __thiscall* )( c_econ_item*, DWORD, void* ) >( utilities::pattern_scan( "client.dll", "55 8B EC 83 E4 F8 83 EC 3C 53 8B 5D 08 56 57 6A ?" ) ) };

		fn( this, v16, &val );
	}

	void get_attribute_value( const int index, int val ) {
		const auto v15{ reinterpret_cast< DWORD* >( get_item_schema( ) ) };
		const auto v16{ *reinterpret_cast< DWORD* >( v15[ 72 ] + 4 * index ) };

		static auto fn{ reinterpret_cast< int( __thiscall* )( c_econ_item*, DWORD ) >( utilities::pattern_scan( "client.dll", "55 8B EC 83 E4 F8 83 EC 3C 53 8B 5D 08 56 57 6A ?" ) ) };

		val = fn( this, v16 );
	}
};