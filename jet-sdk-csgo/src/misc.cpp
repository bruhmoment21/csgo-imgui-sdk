#include "pch.hpp"
#include "config.hpp"

#include "signatures.hpp"

namespace misc
{
	void bypass_invite_cooldown() noexcept
	{
		static bool last_state = false;

		if (config::bypass_invite_cooldown != last_state)
		{
			last_state = config::bypass_invite_cooldown;

			if (DWORD old_protect; VirtualProtect(signatures::delayed_lobby, 1, PAGE_EXECUTE_READWRITE, &old_protect))
			{
				static constexpr std::uint8_t patch[2]{0x74, 0xEB};
				*signatures::delayed_lobby = patch[config::bypass_invite_cooldown];
				VirtualProtect(signatures::delayed_lobby, 1, old_protect, nullptr);
			}
		}
	}

	void fake_prime() noexcept
	{
		static bool last_state = false;

		if (config::fake_prime != last_state)
		{
			last_state = config::fake_prime;

			if (DWORD old_protect; VirtualProtect(signatures::fake_prime, 1, PAGE_EXECUTE_READWRITE, &old_protect))
			{
				static constexpr std::uint8_t patch[2]{0x74, 0xEB};
				*signatures::fake_prime = patch[config::fake_prime];
				VirtualProtect(signatures::fake_prime, 1, old_protect, nullptr);
			}
		}
	}
}