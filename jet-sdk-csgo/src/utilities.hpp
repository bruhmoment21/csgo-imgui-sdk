#pragma once

class c_vpk_archive;

namespace utilities
{
	// Console
	inline void allocate_console(const char* title = "jet sdk console") noexcept
	{
#ifdef _DEBUG
		AllocConsole();
		freopen_s(reinterpret_cast<FILE**>stdin, "conin$", "r", stdin);
		freopen_s(reinterpret_cast<FILE**>stdout, "conout$", "w", stdout);
		SetConsoleTitleA(title);
#endif
	}

	inline void release_console() noexcept
	{
#ifdef _DEBUG
		fclose(stdin);
		fclose(stdout);
		FreeConsole();
#endif
	}

	// Virtual stuff
	template <typename T, std::uint32_t index, typename ...Arguments>
	inline auto call_virtual(void* const class_base, Arguments... args) noexcept
	{
		using fn = T(__thiscall***)(void*, Arguments...);
		return ((*static_cast<fn>(class_base))[index])(class_base, args...);
	}
	inline auto get_virtual(void* const class_base, const std::uint32_t index) noexcept
	{
		return reinterpret_cast<void*>((*static_cast<int**>(class_base))[index]);
	}
}