#include "pch.hpp"
#include "sdk.hpp"

#include "c_inventory.hpp"

// https://www.unknowncheats.me/forum/counterstrike-global-offensive/444181-vac-monitors-createinterface-calls.html
template <typename T>
static void get_interface(T& value, const char* module_name, const char* name)
{
	using fn_create_interface_t = std::add_pointer_t<void* (const char*, int*)>;

	const auto module_handle = GetModuleHandleA(module_name);
	if (!module_handle) throw std::runtime_error{"[get_interface] Couldn't get " + std::string{ module_name } + " handle"};

	if (const auto fn_create_interface = reinterpret_cast<fn_create_interface_t>(GetProcAddress(module_handle, "CreateInterface")))
	{
		if (auto* const result = fn_create_interface(name, nullptr))
		{
#ifdef _DEBUG
			std::cout << "[" << module_name << "] " << name << " found at -> 0x" << std::hex << std::uppercase << result << '\n' << std::dec;
#endif
			value = static_cast<T>(result);
		} else
		{
			throw std::runtime_error{"Couldn't find " + std::string{ name }};
		}
	}
}

namespace sdk
{
	void initialize()
	{
		get_interface(client, "client.dll", "VClient018");
		get_interface(entity_list, "client.dll", "VClientEntityList003");
		get_interface(engine, "engine.dll", "VEngineClient014");
		get_interface(event_manager, "engine.dll", "GAMEEVENTSMANAGER002");
		get_interface(surface, "vguimatsurface.dll", "VGUI_Surface031");
		get_interface(input_system, "inputsystem.dll", "InputSystemVersion001");
		get_interface(localize, "localize.dll", "Localize_001");
		get_interface(model_info, "engine.dll", "VModelInfoClient004");
		get_interface(console, "vstdlib.dll", "VEngineCvar007");
		get_interface(leaf_system, "client.dll", "ClientLeafSystem002");
		get_interface(engine_sound, "engine.dll", "IEngineSoundClient003");
		get_interface(panorama_ui_engine, "panorama.dll", "PanoramaUIEngine001");
	}
}