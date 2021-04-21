#pragma once

enum class egc_result_t
{
	egc_result_ok = 0,
	egc_result_no_message = 1,			// There is no message in the queue
	egc_result_buffer_too_small = 2,		// The buffer is too small for the requested message
	egc_result_not_logged_on = 3,			// The client is not logged onto Steam
	egc_result_invalid_message = 4,		// Something was wrong with the message being sent with SendMessage
};

class i_steam_game_coordinator
{
public:
	// sends a message to the Game Coordinator
	virtual egc_result_t send_message(std::uint32_t msg_type, const void* data, std::uint32_t cub_data) = 0;

	// returns true if there is a message waiting from the game coordinator
	virtual bool is_message_available(std::uint32_t* ptr_cub_msg_size) = 0;

	// fills the provided buffer with the first message in the queue and returns k_EGCResultOK or 
	// returns k_EGCResultNoMessage if there is no message waiting. pcubMsgSize is filled with the message size.
	// If the provided buffer is not large enough to fit the entire message, k_EGCResultBufferTooSmall is returned
	// and the message remains at the head of the queue.
	virtual egc_result_t retrieve_message(std::uint32_t* msg_type, void* dest, std::uint32_t cub_dest, std::uint32_t* ptr_cub_msg_size) = 0;
};