#include "pch.hpp"

#include "signatures.hpp"
#include "hooks.hpp"

#ifdef _DEBUG
void* operator new(const std::size_t size)
{
	++hooks::alloc_count;
	return malloc(size);
}

void operator delete(void* const p)
{
	++hooks::freed_alloc_count;
	return free(p);
}
#endif

int __stdcall DllMain(HMODULE instance, DWORD reason, void* reserved)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		hooks::instance = instance;
		hooks::setup();
	}

	return TRUE;
}