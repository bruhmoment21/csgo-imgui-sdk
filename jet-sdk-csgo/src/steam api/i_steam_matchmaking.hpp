#pragma once

#include "utilities.hpp"

class c_steam_id_t
{
public:
	inline bool operator==(const c_steam_id_t& val) const
	{
		return steam_id.all_64_bits == val.steam_id.all_64_bits;
	}

	union steam_id_t
	{
		struct steam_id_component_t
		{
			std::uint32_t account_id : 32; // unique account identifier
			std::uint32_t account_instance : 20;	// dynamic instance ID
			std::uint32_t account_type : 4; // type of account - can't show as EAccountType, due to signed / unsigned difference
			int universe : 8; // universe this account belongs to
		} m_comp;

		std::uint64_t all_64_bits;
	} steam_id;
};

class i_steam_matchmaking
{
public:
	// game server favorites storage
	// saves basic details about a multiplayer game server locally

	// returns the number of favorites servers the user has stored
	virtual int GetFavoriteGameCount() = 0;

	// returns the details of the game server
	// iGame is of range [0,GetFavoriteGameCount())
	// *pnIP, *pnConnPort are filled in the with IP:port of the game server
	// *punFlags specify whether the game server was stored as an explicit favorite or in the history of connections
	// *pRTime32LastPlayedOnServer is filled in the with the Unix time the favorite was added
	virtual bool GetFavoriteGame(int iGame, std::uint32_t* pnAppID, std::uint32_t* pnIP, std::uint16_t* pnConnPort, std::uint16_t* pnQueryPort, std::uint32_t* punFlags, std::uint32_t* pRTime32LastPlayedOnServer) = 0;

	// adds the game server to the local list; updates the time played of the server if it already exists in the list
	virtual int AddFavoriteGame(std::uint32_t nAppID, std::uint32_t nIP, std::uint16_t nConnPort, std::uint16_t nQueryPort, std::uint32_t unFlags, std::uint32_t rTime32LastPlayedOnServer) = 0;

	// removes the game server from the local storage; returns true if one was removed
	virtual bool RemoveFavoriteGame(std::uint32_t nAppID, std::uint32_t nIP, std::uint16_t nConnPort, std::uint16_t nQueryPort, std::uint32_t unFlags) = 0;
	virtual int RequestLobbyList() = 0;
	// filters for lobbies
	// this needs to be called before RequestLobbyList() to take effect
	// these are cleared on each call to RequestLobbyList()
	virtual void AddRequestLobbyListStringFilter(const char* key_to_match, const char* pchValueToMatch, int comparison_type) = 0;
	// numerical comparison
	virtual void AddRequestLobbyListNumericalFilter(const char* key_to_match, int nValueToMatch, int comparison_type) = 0;
	// returns results closest to the specified value. Multiple near filters can be added, with early filters taking precedence
	virtual void AddRequestLobbyListNearValueFilter(const char* key_to_match, int nValueToBeCloseTo) = 0;
	// returns only lobbies with the specified number of slots available
	virtual void AddRequestLobbyListFilterSlotsAvailable(int nSlotsAvailable) = 0;
	// sets the distance for which we should search for lobbies (based on users IP address to location map on the Steam backed)
	virtual void AddRequestLobbyListDistanceFilter(int eLobbyDistanceFilter) = 0;
	// sets how many results to return, the lower the count the faster it is to download the lobby results & details to the client
	virtual void AddRequestLobbyListResultCountFilter(int cMaxResults) = 0;

	virtual void AddRequestLobbyListCompatibleMembersFilter(c_steam_id_t steam_id_lobby) = 0;

	// returns the c_steam_id_t of a lobby, as retrieved by a RequestLobbyList call
	// should only be called after a LobbyMatchList_t callback is received
	// iLobby is of the range [0, LobbyMatchList_t::m_nLobbiesMatching)
	// the returned c_steam_id_t::IsValid() will be false if iLobby is out of range
	virtual c_steam_id_t GetLobbyByIndex(int iLobby) = 0;

	// Create a lobby on the Steam servers.
	// If private, then the lobby will not be returned by any RequestLobbyList() call; the c_steam_id_t
	// of the lobby will need to be communicated via game channels or via InviteUserToLobby()
	// this is an asynchronous request
	// results will be returned by LobbyCreated_t callback and call result; lobby is joined & ready to use at this point
	// a LobbyEnter_t callback will also be received (since the local user is joining their own lobby)
	virtual int CreateLobby(int lobby_type, int max_members) = 0;

	// Joins an existing lobby
	// this is an asynchronous request
	// results will be returned by LobbyEnter_t callback & call result, check m_EChatRoomEnterResponse to see if was successful
	// lobby metadata is available to use immediately on this call completing
	virtual int JoinLobby(c_steam_id_t steam_id_lobby) = 0;

	// Leave a lobby; this will take effect immediately on the client side
	// other users in the lobby will be notified by a LobbyChatUpdate_t callback
	virtual void LeaveLobby(c_steam_id_t steam_id_lobby) = 0;

	// Invite another user to the lobby
	// the target user will receive a LobbyInvite_t callback
	// will return true if the invite is successfully sent, whether or not the target responds
	// returns false if the local user is not connected to the Steam servers
	// if the other user clicks the join link, a GameLobbyJoinRequested_t will be posted if the user is in-game,
	// or if the game isn't running yet the game will be launched with the parameter +connect_lobby <64-bit lobby id>
	virtual bool InviteUserToLobby(c_steam_id_t steam_id_lobby, c_steam_id_t steamIDInvitee) = 0;

	// Lobby iteration, for viewing details of users in a lobby
	// only accessible if the lobby user is a member of the specified lobby
	// persona information for other lobby members (name, avatar, etc.) will be asynchronously received
	// and accessible via ISteamFriends interface

	// returns the number of users in the specified lobby
	virtual int GetNumLobbyMembers(c_steam_id_t steam_id_lobby) = 0;
	// returns the c_steam_id_t of a user in the lobby
	// iMember is of range [0,GetNumLobbyMembers())
	// note that the current user must be in a lobby to retrieve c_steam_id_ts of other users in that lobby
	virtual c_steam_id_t GetLobbyMemberByIndex(c_steam_id_t steam_id_lobby, int iMember) = 0;

	// Get data associated with this lobby
	// takes a simple key, and returns the string associated with it
	// "" will be returned if no value is set, or if steam_id_lobby is invalid
	virtual const char* GetLobbyData(c_steam_id_t steam_id_lobby, const char* key) = 0;
	// Sets a key/value pair in the lobby metadata
	// each user in the lobby will be broadcast this new value, and any new users joining will receive any existing data
	// this can be used to set lobby names, map, etc.
	// to reset a key, just set it to ""
	// other users in the lobby will receive notification of the lobby data change via a LobbyDataUpdate_t callback
	virtual bool SetLobbyData(c_steam_id_t steam_id_lobby, const char* key, const char* pchValue) = 0;

	// returns the number of metadata keys set on the specified lobby
	virtual int GetLobbyDataCount(c_steam_id_t steam_id_lobby) = 0;

	// returns a lobby metadata key/values pair by index, of range [0, GetLobbyDataCount())
	virtual bool GetLobbyDataByIndex(c_steam_id_t steam_id_lobby, int iLobbyData, char* key, int cchKeyBufferSize, char* pchValue, int cchValueBufferSize) = 0;

	// removes a metadata key from the lobby
	virtual bool DeleteLobbyData(c_steam_id_t steam_id_lobby, const char* key) = 0;

	// Gets per-user metadata for someone in this lobby
	virtual const char* GetLobbyMemberData(c_steam_id_t steam_id_lobby, c_steam_id_t steamIDUser, const char* key) = 0;
	// Sets per-user metadata (for the local user implicitly)
	virtual void SetLobbyMemberData(c_steam_id_t steam_id_lobby, const char* key, const char* pchValue) = 0;

	// Broadcasts a chat message to the all the users in the lobby
	// users in the lobby (including the local user) will receive a LobbyChatMsg_t callback
	// returns true if the message is successfully sent
	// pvMsgBody can be binary or text data, up to 4k
	// if pvMsgBody is text, cubMsgBody should be strlen( text ) + 1, to include the null terminator
	virtual bool SendLobbyChatMsg(c_steam_id_t steam_id_lobby, const void* pvMsgBody, int cubMsgBody) = 0;
	// Get a chat message as specified in a LobbyChatMsg_t callback
	// iChatID is the LobbyChatMsg_t::m_iChatID value in the callback
	// *pSteamIDUser is filled in with the c_steam_id_t of the member
	// *pvData is filled in with the message itself
	// return value is the number of bytes written into the buffer
	virtual int GetLobbyChatEntry(c_steam_id_t steam_id_lobby, int iChatID, c_steam_id_t* pSteamIDUser, void* pvData, int cubData, int* peChatEntryType) = 0;

	// Refreshes metadata for a lobby you're not necessarily in right now
	// you never do this for lobbies you're a member of, only if your
	// this will send down all the metadata associated with a lobby
	// this is an asynchronous call
	// returns false if the local user is not connected to the Steam servers
	// results will be returned by a LobbyDataUpdate_t callback
	// if the specified lobby doesn't exist, LobbyDataUpdate_t::m_bSuccess will be set to false
	virtual bool RequestLobbyData(c_steam_id_t steam_id_lobby) = 0;

	// sets the game server associated with the lobby
	// usually at this point, the users will join the specified game server
	// either the IP/Port or the steamID of the game server has to be valid, depending on how you want the clients to be able to connect
	virtual void SetLobbyGameServer(c_steam_id_t steam_id_lobby, std::uint32_t unGameServerIP, std::uint16_t unGameServerPort, c_steam_id_t steamIDGameServer) = 0;
	// returns the details of a game server set in a lobby - returns false if there is no game server set, or that lobby doesn't exist
	virtual bool GetLobbyGameServer(c_steam_id_t steam_id_lobby, std::uint32_t* punGameServerIP, std::uint16_t* punGameServerPort, c_steam_id_t* psteamIDGameServer) = 0;

	// set the limit on the # of users who can join the lobby
	virtual bool SetLobbyMemberLimit(c_steam_id_t steam_id_lobby, int max_members) = 0;
	// returns the current limit on the # of users who can join the lobby; returns 0 if no limit is defined
	virtual int GetLobbyMemberLimit(c_steam_id_t steam_id_lobby) = 0;

	// updates which type of lobby it is
	// only lobbies that are k_ELobbyTypePublic or k_ELobbyTypeInvisible, and are set to joinable, will be returned by RequestLobbyList() calls
	virtual bool SetLobbyType(c_steam_id_t steam_id_lobby, int lobby_type) = 0;

	// sets whether or not a lobby is joinable - defaults to true for a new lobby
	// if set to false, no user can join, even if they are a friend or have been invited
	virtual bool SetLobbyJoinable(c_steam_id_t steam_id_lobby, bool joinable) = 0;

	// returns the current lobby owner
	// you must be a member of the lobby to access this
	// there always one lobby owner - if the current owner leaves, another user will become the owner
	// it is possible (bur rare) to join a lobby just as the owner is leaving, thus entering a lobby with self as the owner
	virtual c_steam_id_t GetLobbyOwner(c_steam_id_t steam_id_lobby) = 0;

	// changes who the lobby owner is
	// you must be the lobby owner for this to succeed, and steam_id_new_owner must be in the lobby
	// after completion, the local user will no longer be the owner
	virtual bool SetLobbyOwner(c_steam_id_t steam_id_lobby, c_steam_id_t steam_id_new_owner) = 0;

	// link two lobbies for the purposes of checking player compatibility
	// you must be the lobby owner of both lobbies
	virtual bool SetLinkedLobby(c_steam_id_t steam_id_lobby, c_steam_id_t steam_id_lobbyDependent) = 0;
};