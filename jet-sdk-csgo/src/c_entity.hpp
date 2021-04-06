#pragma once

#include "netvars.hpp"
#include "utilities.hpp"

#include "i_base_client.hpp"
#include "math.hpp"

class c_econ_item_view;

inline constexpr bool is_knife(const short i) noexcept
{
	return i >= 500 && i < 5027 || i == 42 || i == 59;
}

inline constexpr bool is_custom_knife(const short i) noexcept
{
	return i >= 500 && i < 5027;
}

enum class frame_stage_t : int
{
	// Haven't run any frames yet.
	frame_undefined = -1,
	frame_start,

	// A network packet is being recieved.
	frame_net_update_start,
	// Data has been received and we're going to start calling postdataupdate.
	frame_net_update_postdataupdate_start,
	// Data has been received and we've called postdataupdate on all data recipients.
	frame_net_update_postdataupdate_end,
	// We've received all packets, we can now do interpolation, prediction, etc...
	frame_net_update_end,

	// We're about to start rendering the scene.
	frame_render_start,
	// We've finished rendering the scene.
	frame_render_end
};

class collideable_t
{
public:
	vec3_t mins()
	{
		return utilities::call_virtual< vec3_t&, 1 >(this);
	}

	vec3_t maxs()
	{
		return utilities::call_virtual< vec3_t&, 2 >(this);
	}
};

class entity_t
{
public:
	void* animating()
	{
		return static_cast<void*>(this + 4);
	}

	void* networkable()
	{
		return static_cast<void*>(this + 8);
	}

	void release()
	{
		return utilities::call_virtual< void, 1 >(networkable());
	}

	client_class_t* client_class()
	{
		return utilities::call_virtual< client_class_t*, 2 >(networkable());
	}

	int handle()
	{
		return utilities::call_virtual< int&, 2 >(this);
	}

	collideable_t* collideable()
	{
		return utilities::call_virtual< collideable_t*, 3 >(this);
	}

	void on_data_changed(const int update_type)
	{
		return utilities::call_virtual< void, 5, int >(networkable(), update_type);
	}

	void pre_data_update(const int update_type)
	{
		return utilities::call_virtual< void, 6, int >(networkable(), update_type);
	}

	void post_data_update(const int update_type)
	{
		return utilities::call_virtual< void, 7, int >(networkable(), update_type);
	}

	const void* get_model()
	{
		return utilities::call_virtual< const void*, 8 >(animating());
	}

	bool dormant()
	{
		return utilities::call_virtual< bool, 9 >(networkable());
	}

	int index()
	{
		return utilities::call_virtual< int, 10 >(networkable());
	}

	void set_destroyed_on_recreate_entity()
	{
		return utilities::call_virtual< void, 13 >(networkable());
	}

	void set_model_index(const int index)
	{
		return utilities::call_virtual< void, 75, int >(this, index);
	}

	bool is_alive()
	{
		return utilities::call_virtual< bool, 155 >(this);
	}

	void send_viewmodel_matching_sequence(int sequence)
	{
		return utilities::call_virtual< void, 246, int >(this, sequence);
	}

	entity_t* active_weapon()
	{
		return utilities::call_virtual< entity_t*, 267 >(this);
	}

	c_econ_item_view* get_econ_item_view()
	{
		return signatures::fn_get_econ_item_view(this);
	}

	int equip_wearable(void* const owner)
	{
		return signatures::fn_equip_wearable(this, owner);
	}

	int initialize_attributes()
	{
		return signatures::fn_initialize_attributes(this);
	}

	// NETVARS
	auto& body()
	{
		return netvar_manager::get_netvar< int >("DT_BaseAnimating", "m_nBody", this);
	}

	auto& view_model_index()
	{
		return netvar_manager::get_netvar< int >("DT_BaseViewModel", "m_nViewModelIndex", this);
	}

	auto& weapon_handle()
	{
		return netvar_manager::get_netvar< int >("DT_BaseViewModel", "m_hWeapon", this);
	}

	auto& owner_handle()
	{
		return netvar_manager::get_netvar< int >("DT_BaseViewModel", "m_hOwner", this);
	}

	auto& item_index()
	{
		return netvar_manager::get_netvar< short >("DT_BaseAttributableItem", "m_iItemDefinitionIndex", this);
	}

	auto& original_owner_xuid_low()
	{
		return netvar_manager::get_netvar< std::uint32_t >("DT_BaseAttributableItem", "m_OriginalOwnerXuidLow", this);
	}

	auto& original_owner_xuid_high()
	{
		return netvar_manager::get_netvar< std::uint32_t >("DT_BaseAttributableItem", "m_OriginalOwnerXuidHigh", this);
	}

	auto& item_id_low()
	{
		return netvar_manager::get_netvar< std::uint64_t >("DT_BaseAttributableItem", "m_iItemIDLow", this);
	}

	auto& item_id_high()
	{
		return netvar_manager::get_netvar< int >("DT_BaseAttributableItem", "m_iItemIDHigh", this);
	}

	auto& entity_quality()
	{
		return netvar_manager::get_netvar< int >("DT_BaseAttributableItem", "m_iEntityQuality", this);
	}

	auto& account_id()
	{
		return netvar_manager::get_netvar< std::uint32_t >("DT_BaseAttributableItem", "m_iAccountID", this);
	}

	auto& initialized()
	{
		return netvar_manager::get_netvar< bool >("DT_BaseAttributableItem", "m_bInitialized", this);
	}

	auto& world_model_handle()
	{
		return netvar_manager::get_netvar< int >("DT_BaseCombatWeapon", "m_hWeaponWorldModel", this);
	}

	auto wearables()
	{
		return netvar_manager::get_netvar_pointer< int >("DT_BaseCombatCharacter", "m_hMyWearables", this);
	}

	auto& weapons()
	{
		return netvar_manager::get_netvar< int[64] >("DT_BaseCombatCharacter", "m_hMyWeapons", this);
	}

	auto& model_index()
	{
		return netvar_manager::get_netvar< int >("DT_BaseEntity", "m_nModelIndex", this);
	}

	auto& team()
	{
		return netvar_manager::get_netvar< int >("DT_BaseEntity", "m_iTeamNum", this);
	}

	auto& owner_entity()
	{
		return netvar_manager::get_netvar< int >("DT_BaseEntity", "m_hOwnerEntity", this);
	}

	auto& view_model_handle()
	{
		return netvar_manager::get_netvar< int >("DT_BasePlayer", "m_hViewModel[0]", this);
	}

	auto& tick_base()
	{
		return netvar_manager::get_netvar< int >("DT_BasePlayer", "m_nTickBase", this);
	}

	// Player related NETVARS
	auto& health()
	{
		return netvar_manager::get_netvar< int >("DT_CSPlayer", "m_iHealth", this);
	}

	auto& is_scoped()
	{
		return netvar_manager::get_netvar< bool >("DT_CSPlayer", "m_bIsScoped", this);
	}

	auto& is_controlling_bot()
	{
		return netvar_manager::get_netvar< bool >("DT_CSPlayer", "m_bIsControllingBot", this);
	}

	auto& ragdoll_handle()
	{
		return netvar_manager::get_netvar< int >("DT_CSPlayer", "m_hRagdoll", this);
	}

	auto& weapon_original_team_number()
	{
		return netvar_manager::get_netvar< int >("DT_WeaponCSBase", "m_iOriginalTeamNumber", this);
	}

	auto& weapon_previous_owner()
	{
		return netvar_manager::get_netvar< int >("DT_WeaponCSBase", "m_hPrevOwner", this);
	}

	// C4 netvars
	auto& started_arming()
	{
		return netvar_manager::get_netvar< bool >("DT_WeaponC4", "m_bStartedArming", this);
	}

	auto& c4_bombsite()
	{
		return netvar_manager::get_netvar< int >("DT_PlantedC4", "m_nBombSite", this);
	}

	auto& c4_blow_time()
	{
		return netvar_manager::get_netvar< float >("DT_PlantedC4", "m_flC4Blow", this);
	}

	auto& c4_defuser()
	{
		return netvar_manager::get_netvar< int >("DT_PlantedC4", "m_hBombDefuser", this);
	}

	auto& c4_defuse_time()
	{
		return netvar_manager::get_netvar< float >("DT_PlantedC4", "m_flDefuseCountDown", this);
	}
};