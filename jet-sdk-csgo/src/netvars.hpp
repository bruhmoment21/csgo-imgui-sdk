#pragma once

namespace fnv_1a
{
	struct fnv_internal
	{
		static constexpr std::uint32_t default_offset_basis = 0x811c9dc5;
		static constexpr std::uint32_t prime = 0x1000193;
	};

	struct fnv1a : fnv_internal
	{
		static constexpr std::uint32_t hash(const char* string, const std::uint32_t& val = default_offset_basis) noexcept
		{
			return (string[0] == '\0') ? val : hash(&string[1], (val ^ std::uint32_t(string[0])) * prime);
		}

		static constexpr bool equal_hashes(const char* const a, const char* const b) noexcept
		{
			return hash(a) == hash(b);
		}
	};
}

using fnv = fnv_1a::fnv1a;

namespace netvar_manager
{
	std::uintptr_t get_offset(const std::uint32_t table, const std::uint32_t prop) noexcept;

	template < typename T >
	[[nodiscard]] inline std::add_lvalue_reference_t< T > get_netvar(const char* table, const char* prop, void* const class_base, const std::uintptr_t offset = 0) noexcept
	{
		const auto address = netvar_manager::get_offset(fnv::hash(table), fnv::hash(prop));
		
		const auto _this = reinterpret_cast<std::uintptr_t>(class_base);
		return *reinterpret_cast<std::add_pointer_t< T >>(_this + address + offset);
	}

	template < typename T >
	[[nodiscard]] inline auto get_netvar_pointer(const char* table, const char* prop, void* const class_base, const std::uintptr_t offset = 0) noexcept
	{
		const auto address = netvar_manager::get_offset(fnv::hash(table), fnv::hash(prop));

		const auto _this = reinterpret_cast<std::uintptr_t>(class_base);
		return reinterpret_cast<std::add_pointer_t< T >>(_this + address + offset);
	}
}