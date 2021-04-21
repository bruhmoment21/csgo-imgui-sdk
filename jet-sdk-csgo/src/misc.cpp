#include "pch.hpp"
#include "config.hpp"

#include "signatures.hpp"
#include "utilities.hpp"

namespace misc
{
	void bypass_invite_cooldown() noexcept
	{
		static bool last_state = false;

		if (config::bypass_invite_cooldown != last_state)
		{
			last_state = config::bypass_invite_cooldown;

			static constexpr std::uint8_t patch[2]{0x74, 0xEB};
			utilities::patch_at_address(signatures::delayed_lobby, &(patch[config::bypass_invite_cooldown]), 1);
		}
	}

	void fake_prime() noexcept
	{
		static bool last_state = false;

		if (config::fake_prime != last_state)
		{
			last_state = config::fake_prime;

			static constexpr std::uint8_t patch[2]{0x74, 0xEB};
			utilities::patch_at_address(signatures::fake_prime, &(patch[config::fake_prime]), 1);
		}
	}
}