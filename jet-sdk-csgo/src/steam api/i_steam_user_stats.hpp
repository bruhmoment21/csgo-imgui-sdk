#pragma once

class i_steam_user_stats
{
public:
	virtual bool request_current_stats() = 0;
	virtual bool get_stat(const char* name, int* data) = 0;
	virtual bool get_stat(const char* name, float* data) = 0;
	virtual bool set_stat(const char* name, int data) = 0;
	virtual bool set_stat(const char* name, float data) = 0;
	virtual bool update_average_rate_stat(const char* name, float count_this_session, double session_length) = 0;
	virtual bool get_achievement(const char* name, bool* achieved) = 0;
	virtual bool set_achievement(const char* name) = 0;
	virtual bool clear_achievement(const char* name) = 0;
	virtual bool get_achievement_and_unlock_time(const char* name, bool* achieved, std::uint32_t* unlock_time) = 0;
	virtual bool store_stats() = 0;
	virtual int get_achievement_icon(const char* name) = 0;
	virtual const char* get_achievement_display_attribute(const char* name, const char* key) = 0;
	virtual bool indicate_achievement_progress(const char* name, std::uint32_t current_progress, std::uint32_t max_progress) = 0;
	virtual std::uint32_t achievements_count() = 0;
	virtual const char* get_achievement_name(std::uint32_t achievement) = 0;
};