#pragma once

template < typename T, typename I = int >
class c_utl_memory
{
public:
	T* get_buffer()
	{
		return memory;
	}

	int get_allocation_count()
	{
		return allocation_count;
	}

	T& operator[](I i)
	{
		return memory[i];
	}

	int get_grow_size()
	{
		return grow_size;
	}

	T* offset_buffer_by_index(std::size_t index)
	{
		return memory + index;
	}

private:
	T* memory;
	int allocation_count;
	int grow_size;
};

template <class T>
inline T* construct(T* memory)
{
	return new(memory) T;
}

template <class T>
inline void destruct(T* memory)
{
	memory->~T();
}

template < typename T, typename allocator = c_utl_memory< T > >
class c_utl_vector
{
public:
	allocator get_memory(void)
	{
		return memory;
	}

	T& element(int i)
	{
		return memory[i];
	}

	T& operator[](int i)
	{
		return memory[i];
	}

	int get_size()
	{
		return size;
	}

	T* get_elements()
	{
		return elements;
	}

	void remove_all()
	{
		for (int i = size; --i >= 0; )
		{
			destruct(&element(i));
		}

		size = 0;
	}

private:
	allocator memory;
	int size;
	T* elements;
};