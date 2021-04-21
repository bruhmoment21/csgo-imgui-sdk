#include "pch.hpp"

#include "signatures.hpp"
#include "hooks.hpp"

/*

Thanks to cristeigabriel for his ideas.

*/

static std::exception exception;

static void show_messagebox() // MessageBox won't work if not created from another thread.
{
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	MessageBoxA(nullptr, exception.what(), "Exception caught!", MB_OK | MB_ICONERROR);
	hooks::release();
}

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
		try
		{
			hooks::instance = instance;
			hooks::setup();
		} catch (const std::exception& ex)
		{
			exception = ex;

			if (auto* const exception_thread = CreateThread(nullptr, 0, LPTHREAD_START_ROUTINE(show_messagebox), nullptr, 0, nullptr))
			{
				CloseHandle(exception_thread);
			}
		}
	}

	return TRUE;
}