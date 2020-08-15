#pragma once

#include "../../other files/math/helpers/vec3_t.hpp"
#include "../virtual_method.hpp"

enum class client_frame_stage_t : int {

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

enum item_definition_indexes : short {

    weapon_deagle = 1,
    weapon_elite = 2,
    weapon_fiveseven = 3,
    weapon_glock = 4,
    weapon_ak47 = 7,
    weapon_aug = 8,
    weapon_awp = 9,
    weapon_famas = 10,
    weapon_g3sg1 = 11,
    weapon_galilar = 13,
    weapon_m249 = 14,
    weapon_m4a1 = 16,
    weapon_mac10 = 17,
    weapon_p90 = 19,
    weapon_mp5_sd = 23,
    weapon_ump45 = 24,
    weapon_xm1014 = 25,
    weapon_bizon = 26,
    weapon_mag7 = 27,
    weapon_negev = 28,
    weapon_sawedoff = 29,
    weapon_tec9 = 30,
    weapon_taser = 31,
    weapon_hkp2000 = 32,
    weapon_mp7 = 33,
    weapon_mp9 = 34,
    weapon_nova = 35,
    weapon_p250 = 36,
    weapon_scar20 = 38,
    weapon_sg556 = 39,
    weapon_ssg08 = 40,
    weapon_knife = 42,
    weapon_flashbang = 43,
    weapon_hegrenade = 44,
    weapon_smokegrenade = 45,
    weapon_molotov = 46,
    weapon_decoy = 47,
    weapon_incgrenade = 48,
    weapon_c4 = 49,
    weapon_knife_t = 59,
    weapon_m4a1_silencer = 60,
    weapon_usp_silencer = 61,
    weapon_cz75a = 63,
    weapon_revolver = 64,
    weapon_knife_bayonet = 500,
    weapon_knife_css = 503,
    weapon_knife_flip = 505,
    weapon_knife_gut = 506,
    weapon_knife_karambit = 507,
    weapon_knife_m9_bayonet = 508,
    weapon_knife_tactical = 509,
    weapon_knife_falchion = 512,
    weapon_knife_survival_bowie = 514,
    weapon_knife_butterfly = 515,
    weapon_knife_push = 516,
    weapon_knife_cord,
    weapon_knife_canis,
    weapon_knife_ursus = 519,
    weapon_knife_gypsy_jackknife = 520,
    weapon_knife_outdoor,
    weapon_knife_stiletto = 522,
    weapon_knife_widowmaker = 523,
    weapon_knife_skeleton = 525,
    glove_studded_bloodhound = 5027,
    glove_t_side = 5028,
    glove_ct_side = 5029,
    glove_sporty = 5030,
    glove_slick = 5031,
    glove_leather_wrap = 5032,
    glove_motorcycle = 5033,
    glove_specialist = 5034,
    glove_hydra = 5035,

    // agents in case you need them (shattered web).
    special_agent_ground_rebel = 5105,
    special_agent_osiris,
    special_agent_shahmat,
    special_agent_muhlik,
    special_agent_soldier = 5205,
    special_agent_enforcer,
    special_agent_slingshot,
    special_agent_operator = 5305,
    special_agent_markus_delrow,
    special_agent_michael_syfers,
    special_agent_ava,
    special_agent_3rd_commando_company = 5400,
    special_agent_seal_team_6_soldier,
    special_agent_buckshot,
    special_agent_two_times,
    special_agent_ricksaw,
    special_agent_dragomir = 5500,
    special_agent_maximus,
    special_agent_rezan_the_ready,
    special_agent_blackwolf,
    special_agent_the_doctor,
    special_agent_b_squadron_officer = 5601,
};

constexpr bool is_knife( const short i ) noexcept {
    return i >= weapon_knife_bayonet && i < glove_studded_bloodhound || i == weapon_knife_t || i == weapon_knife;
}

constexpr bool is_custom_agent( const short i ) noexcept {
    return ( i >= special_agent_ground_rebel && i <= special_agent_muhlik )
        || ( i >= special_agent_operator && i <= special_agent_slingshot )
        || ( i >= special_agent_soldier && i <= special_agent_ava )
        || ( i >= special_agent_3rd_commando_company && i <= special_agent_ricksaw )
        || ( i >= special_agent_dragomir && i <= special_agent_the_doctor )
        || ( i == special_agent_b_squadron_officer );
}

class collideable_t {
public:
    vec3_t mins( ) {
        return virtual_method::call_virtual< vec3_t&, 1 >( this );
    }

    vec3_t maxs( ) {
        return virtual_method::call_virtual< vec3_t&, 2 >( this );
    }
};

class entity_t {
public:
    // soon
};