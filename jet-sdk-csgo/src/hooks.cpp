#include "pch.hpp"
#include "hooks.hpp"

#include "minhook/MinHook.h"

#include "events.hpp"

#include "signatures.hpp"
#include "utilities.hpp"
#include "config.hpp"
#include "menu.hpp"
#include "misc.hpp"
#include "sdk.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"

struct emit_sound_t
{
	void* filter;
	int ent_index;
	int channel;
	const char* entry;
	std::uint32_t hash;
	const char* sample;
	float volume;
	int seed;
	float attenuation;
	int flags;
	int pitch;
	const vec3_t* origin;
	const vec3_t* direction;
	void* utl_origin;
	bool update_pos;
	float sound_time;
	int speaker_entity;
	void* sound_parameters;
};

static void MH_CreateHookSafe(void* const target, void* const detour, void* const original, const char* detour_name)
{
	if (MH_CreateHook(static_cast<LPVOID>(target), static_cast<LPVOID>(detour), static_cast<LPVOID*>(original)) != MH_OK)
	{
		throw std::runtime_error{std::string{detour_name} + " failed to hook!"};
	}

#ifdef _DEBUG
	std::cout << detour_name << " hooked successfully!\n";
#endif
}

namespace hooks
{
	void setup()
	{
		utilities::allocate_console();

		signatures::initialize();
		config::create_directory();
		sdk::initialize();
		event_listener.setup();

		original::wnd_proc = reinterpret_cast<decltype(original::wnd_proc)>(SetWindowLongA(FindWindowA("Valve001", nullptr), GWL_WNDPROC, reinterpret_cast<LONG>(wnd_proc)));

		auto* const reset_target = utilities::get_virtual(signatures::directx, 16);
		auto* const present_target = utilities::get_virtual(signatures::directx, 17);
		auto* const lock_cursor_target = utilities::get_virtual(sdk::surface, 67);
		auto* const emit_sound_target = utilities::get_virtual(sdk::engine_sound, 5);
		auto* const dispatch_user_message_target = utilities::get_virtual(sdk::client, 38);
		auto* const frame_stage_notify_target = utilities::get_virtual(sdk::client, 37);

		MH_Initialize();

		MH_CreateHookSafe(reset_target, &reset, &original::reset, "reset");
		MH_CreateHookSafe(present_target, &present, &original::present, "present");
		MH_CreateHookSafe(lock_cursor_target, &lock_cursor, &original::lock_cursor, "lock_cursor");
		MH_CreateHookSafe(emit_sound_target, &emit_sound, &original::emit_sound, "emit_sound");
		MH_CreateHookSafe(dispatch_user_message_target, &dispatch_user_message, &original::dispatch_user_message, "dispatch_user_message");
		MH_CreateHookSafe(signatures::fn_perform_screen_overlay, &perform_screen_overlay, &original::perform_screen_overlay, "perform_screen_overlay");
		MH_CreateHookSafe(frame_stage_notify_target, &frame_stage_notify, &original::frame_stage_notify, "frame_stage_notify");

		MH_EnableHook(nullptr);
	}

	void release() noexcept // Call from another thread.
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		event_listener.release();

		MH_Uninitialize();
		MH_DisableHook(nullptr);

		if (original::wnd_proc)
		{
			SetWindowLongA(FindWindowA("Valve001", nullptr), GWL_WNDPROC, reinterpret_cast<LONG>(original::wnd_proc));
		}

		ImGui_ImplDX9_Shutdown();
		ImGui_ImplWin32_Shutdown();

		if (ImGui::GetCurrentContext())
		{
			ImGui::DestroyContext();
		}

		if (sdk::input_system)
		{
			sdk::input_system->enable_input(true);
		}

		utilities::release_console();
		FreeLibraryAndExitThread(instance, 0ul);
	}

	long __stdcall wnd_proc(HWND window, UINT msg, WPARAM wparm, LPARAM lparm) noexcept
	{
		[[maybe_unused]] static const auto once = [](const HWND& window) noexcept
		{
			ImGui::CreateContext();
			ImGui_ImplWin32_Init(window);
			menu::initialize();

			return true;
		}(window);

		long ImGui_ImplWin32_WndProcHandler(HWND window, UINT msg, WPARAM wparm, LPARAM lparm);
		ImGui_ImplWin32_WndProcHandler(window, msg, wparm, lparm);
		sdk::input_system->enable_input(!menu::is_open);

		return CallWindowProcA(original::wnd_proc, window, msg, wparm, lparm);
	}

	long __stdcall reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params) noexcept
	{
		ImGui_ImplDX9_InvalidateDeviceObjects();
		return original::reset(device, params);
	}

	long __stdcall present(IDirect3DDevice9* device, const RECT* src, const RECT* dest, HWND window_override, const RGNDATA* dirty_region) noexcept
	{
		IDirect3DVertexDeclaration9* vert_declaration;
		IDirect3DVertexShader9* vert_shader;
		DWORD old_d3drs_colorwriteenable;

		// Save State
		device->GetRenderState(D3DRS_COLORWRITEENABLE, &old_d3drs_colorwriteenable);
		device->GetVertexDeclaration(&vert_declaration);
		device->GetVertexShader(&vert_shader);

		device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
		device->SetRenderState(D3DRS_SRGBWRITEENABLE, false);
		device->SetSamplerState(NULL, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		device->SetSamplerState(NULL, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
		device->SetSamplerState(NULL, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP);
		device->SetSamplerState(NULL, D3DSAMP_SRGBTEXTURE, NULL);

		if (!ImGui::GetCurrentContext())
		{
			ImGui::CreateContext();
		}

		ImGui_ImplDX9_Init(device);

		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		if (ImGui::IsKeyPressed(VK_INSERT, false))
		{
			menu::is_open = !menu::is_open;
		}

		misc::bypass_invite_cooldown();
		misc::fake_prime();

		menu::render();

		ImGui::EndFrame();
		ImGui::Render();

		if (device->BeginScene() == D3D_OK)
		{
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
			device->EndScene();
		}

		// Restore State
		device->SetRenderState(D3DRS_COLORWRITEENABLE, old_d3drs_colorwriteenable);
		device->SetRenderState(D3DRS_SRGBWRITEENABLE, true);
		device->SetVertexDeclaration(vert_declaration);
		device->SetVertexShader(vert_shader);

		return original::present(device, src, dest, window_override, dirty_region);
	}

	void __stdcall lock_cursor() noexcept
	{
		if (menu::is_open)
		{
			return sdk::surface->unlock_cursor();
		}

		original::lock_cursor(sdk::surface);
	}

	void __stdcall emit_sound(emit_sound_t sound) noexcept
	{
		if (config::auto_accept && !sdk::engine->is_connected() && !std::strcmp(sound.entry, "UIPanorama.popup_accept_match_beep"))
		{
			sdk::panorama_ui_engine->access_ui_engine()->dispatch_event(signatures::match_assisted_accept(nullptr));
		}

		original::emit_sound(sdk::engine_sound, sound);
	}

	bool __stdcall dispatch_user_message(cstrike_user_messages_t type, int arg, int length, void* data) noexcept
	{
		if (config::block_advertisements && (type == cstrike_user_messages_t::cs_um_textmsg
			|| type == cstrike_user_messages_t::cs_um_hudmsg
			|| type == cstrike_user_messages_t::cs_um_saytext))
		{
			return true;
		}

		return original::dispatch_user_message(sdk::client, type, arg, length, data);
	}

	void __fastcall perform_screen_overlay(void* _this, void* edx, int x, int y, int w, int h) noexcept
	{
		if (config::block_advertisements) return;

		original::perform_screen_overlay(_this, edx, x, y, w, h);
	}

	void __stdcall frame_stage_notify(frame_stage_t frame_stage) noexcept
	{
		local_player::entity = sdk::entity_list->get_client_entity(sdk::engine->get_local_player());

		if (sdk::engine->is_connected() && local_player::entity && sdk::engine->get_player_info(local_player::entity->index(), local_player::info))
		{
			
		}

		original::frame_stage_notify(sdk::client, frame_stage);
	}

	void sequence(recv_proxy_data_t& proxy_data_ref, void* ent, void* output) noexcept
	{
		static const auto original = original::sequence->get_original_function();

		original(proxy_data_ref, ent, output);
	}
}