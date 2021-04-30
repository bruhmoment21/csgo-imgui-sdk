#include "pch.hpp"
#include "sdk.hpp"

#include "config.hpp"

#include "hooks.hpp"

using fn_instantiate_interface_t = std::add_pointer_t<void* ()>;

struct interface_register_t
{
	fn_instantiate_interface_t create_fn;
	const char* name;
	interface_register_t* next;
};

template <typename T>
static void get_interface(T& value, const char* module_name, const std::string_view name)
{
	const auto get_register_list = [module_name]() -> interface_register_t*
	{
		const auto module_handle = GetModuleHandleA(module_name);
		if (!module_handle) throw std::runtime_error{"[get_interface] Couldn't get " + std::string{ module_name } + " handle"};

		const auto create_interface_address = GetProcAddress(module_handle, "CreateInterface");

		const auto create_interface_relative = reinterpret_cast<std::uintptr_t>(create_interface_address) + 5;
		const auto create_interface = create_interface_relative + 4U + *reinterpret_cast<int*>(create_interface_relative);

		return **reinterpret_cast<interface_register_t***>(create_interface + 6);
	};

	for (auto* current_register = get_register_list(); current_register; current_register = current_register->next)
	{
		// found needed interface
		if ((!std::string_view{current_register->name}.compare(0U, name.length(), name) && std::atoi(current_register->name + name.length()) > 0)
			|| !name.compare(current_register->name))
		{
			auto* const result = current_register->create_fn();

#ifdef _DEBUG
			std::cout << "[" << module_name << "] " << name << " found at -> 0x" << std::hex << std::uppercase << result << '\n' << std::dec;
#endif

			value = static_cast<T>(result);
			return;
		}
	}

	throw std::runtime_error{"Couldn't find " + std::string{name}};
}

namespace sdk
{
	void initialize()
	{
		get_interface(client, "client.dll", "VClient");
		get_interface(entity_list, "client.dll", "VClientEntityList");
		get_interface(engine, "engine.dll", "VEngineClient");
		get_interface(event_manager, "engine.dll", "GAMEEVENTSMANAGER002");
		get_interface(surface, "vguimatsurface.dll", "VGUI_Surface");
		get_interface(input_system, "inputsystem.dll", "InputSystemVersion");
		get_interface(localize, "localize.dll", "Localize_");
		get_interface(model_info, "engine.dll", "VModelInfoClient");
		get_interface(console, "vstdlib.dll", "VEngineCvar");
		get_interface(leaf_system, "client.dll", "ClientLeafSystem");
		get_interface(engine_sound, "engine.dll", "IEngineSoundClient");
		get_interface(panorama_ui_engine, "panorama.dll", "PanoramaUIEngine");
		get_interface(material_system, "materialsystem.dll", "VMaterialSystem");
		get_interface(match_framework, "matchmaking.dll", "MATCHFRAMEWORK_");
		get_interface(file_system, "filesystem_stdio.dll", "VBaseFileSystem");
		get_interface(game_ui, "engine.dll", "VENGINE_GAMEUIFUNCS_VERSION");

		globals = **reinterpret_cast<decltype(globals)**>((*reinterpret_cast<std::uintptr_t**>(client))[11] + 10);
		client_mode = **reinterpret_cast<decltype(client_mode)**>((*reinterpret_cast<std::uintptr_t**>(client))[10] + 5);

		if (auto* const tier0_handle = GetModuleHandleA("tier0.dll"); tier0_handle)
		{
			mem_alloc = *reinterpret_cast<decltype(mem_alloc)*>(GetProcAddress(tier0_handle, "g_pMemAlloc"));
		}

		if (auto* const steam_api_handle = GetModuleHandle("steam_api.dll"); steam_api_handle)
		{
			const auto steam_pipe = ((int(*)())GetProcAddress(steam_api_handle, "SteamAPI_GetHSteamPipe"))();
			const auto& steam_context = engine->get_steam_context();
			steam_coordinator = static_cast<i_steam_game_coordinator*>(steam_context->steam_client->get_steam_generic_interface(steam_context->steam_user->get_h_steam_user(), steam_pipe, "SteamGameCoordinator001"));
		}
	}
}

c_key_values::~c_key_values()
{
	sdk::mem_alloc->free(this);
}