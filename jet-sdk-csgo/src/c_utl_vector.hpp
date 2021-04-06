#pragma once

template <typename T>
class c_utl_vector
{
public:
	constexpr T& operator[](int i) noexcept
	{
		return memory[i];
	};
	constexpr const T& operator[](int i) const noexcept
	{
		return memory[i];
	};

	T* memory;
	int alloc_count;
	int grow_size;
	int size;
	T* elements;
};