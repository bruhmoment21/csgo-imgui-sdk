#pragma once

#include "c_recv_props.hpp"

struct emit_sound_t;
enum class cstrike_user_messages_t : int;

class c_econ_item_view;
enum class frame_stage_t;

namespace hooks
{
	namespace original
	{
		inline WNDPROC wnd_proc;
		inline std::add_pointer_t<long __stdcall(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*)> reset;
		inline std::add_pointer_t<long __stdcall(IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA*)> present;
		inline void(__thiscall* lock_cursor)(void*);
		inline void(__thiscall* emit_sound)(void*, emit_sound_t);
		inline bool(__thiscall* dispatch_user_message)(void*, cstrike_user_messages_t, int, int, void*);
		inline std::add_pointer_t<void __fastcall(void*, void*, int, int, int, int)> perform_screen_overlay;
		inline void(__thiscall* frame_stage_notify)(void*, frame_stage_t);
		inline std::unique_ptr< recv_prop_hook_t > sequence;
	}

	void setup();
	void release() noexcept;

#ifdef _DEBUG
	inline std::uint32_t alloc_count, freed_alloc_count;
#endif
	inline HMODULE instance;
	
	long __stdcall wnd_proc(HWND window, UINT msg, WPARAM wparm, LPARAM lparm) noexcept;
	long __stdcall reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params) noexcept;
	long __stdcall present(IDirect3DDevice9* device, const RECT* src, const RECT* dest, HWND window_override, const RGNDATA* dirty_region) noexcept;
	void __stdcall lock_cursor() noexcept;
	void __stdcall emit_sound(emit_sound_t sound) noexcept;
	bool __stdcall dispatch_user_message(cstrike_user_messages_t type, int arg, int length, void* data) noexcept;
	void __fastcall perform_screen_overlay(void* _this, void* edx, int x, int y, int w, int h) noexcept;
	void __stdcall frame_stage_notify(frame_stage_t frame_stage) noexcept;
	void sequence(recv_proxy_data_t& proxy_data_ref, void* ent, void* output) noexcept;
}
