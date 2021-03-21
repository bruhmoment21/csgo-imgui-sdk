#pragma once

#include "utilities.hpp"
#include "c_recv_props.hpp"

class entity_t;

enum class cstrike_user_messages_t : int
{
	cs_um_vguimenu = 1,
	cs_um_geiger = 2,
	cs_um_train = 3,
	cs_um_hudtext = 4,
	cs_um_saytext = 5,
	cs_um_saytext2 = 6,
	cs_um_textmsg = 7,
	cs_um_hudmsg = 8,
	cs_um_resethud = 9,
	cs_um_gametitle = 10,
	cs_um_shake = 12,
	cs_um_fade = 13,
	cs_um_rumble = 14,
	cs_um_closecaption = 15,
	cs_um_closecaptiondirect = 16,
	cs_um_sendaudio = 17,
	cs_um_rawaudio = 18,
	cs_um_voicemask = 19,
	cs_um_requeststate = 20,
	cs_um_damage = 21,
	cs_um_radiotext = 22,
	cs_um_hinttext = 23,
	cs_um_keyhinttext = 24,
	cs_um_processspottedentityupdate = 25,
	cs_um_reloadeffect = 26,
	cs_um_adjustmoney = 27,
	cs_um_updateteammoney = 28,
	cs_um_stopspectatormode = 29,
	cs_um_killcam = 30,
	cs_um_desiredtimescale = 31,
	cs_um_currenttimescale = 32,
	cs_um_achievementevent = 33,
	cs_um_matchendconditions = 34,
	cs_um_disconnecttolobby = 35,
	cs_um_playerstatsupdate = 36,
	cs_um_displayinventory = 37,
	cs_um_warmuphasended = 38,
	cs_um_clientinfo = 39,
	cs_um_xrankget = 40,
	cs_um_xrankupd = 41,
	cs_um_callvotefailed = 45,
	cs_um_votestart = 46,
	cs_um_votepass = 47,
	cs_um_votefailed = 48,
	cs_um_votesetup = 49,
	cs_um_serverrankrevealall = 50,
	cs_um_sendlastkillerdamagetoclient = 51,
	cs_um_serverrankupdate = 52,
	cs_um_itempickup = 53,
	cs_um_showmenu = 54,
	cs_um_bartime = 55,
	cs_um_ammodenied = 56,
	cs_um_markachievement = 57,
	cs_um_matchstatsupdate = 58,
	cs_um_itemdrop = 59,
	cs_um_glowpropturnoff = 60,
	cs_um_sendplayeritemdrops = 61,
	cs_um_roundbackupfilenames = 62,
	cs_um_sendplayeritemfound = 63,
	cs_um_reporthit = 64,
	cs_um_xpupdate = 65,
	cs_um_questprogress = 66,
	cs_um_scoreleaderboarddata = 67,
	cs_um_playerdecaldigitalsignature = 68,
	max_ecstrike15usermessages
};

struct client_class_t
{
	std::add_pointer_t<entity_t* (int, int)> create_fn;
	void* create_event_fn;
	char* network_name;
	recv_table_t* recv_table;
	client_class_t* next;
	int class_id;
};

class i_base_client
{
public:
	client_class_t* get_all_classes()
	{
		return utilities::call_virtual< client_class_t*, 8 >(this);
	}

	bool dispatch_user_message(const int msg_type, const std::uint32_t a1, const std::uint32_t length, const void* data = nullptr)
	{
		return utilities::call_virtual< bool, 38, int, std::uint32_t, std::uint32_t, const void* >(this, msg_type, a1, length, data);
	}
};