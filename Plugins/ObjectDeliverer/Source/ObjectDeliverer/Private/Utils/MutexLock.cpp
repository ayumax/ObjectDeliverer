#include "MutexLock.h"
#if PLATFORM_WINDOWS
#include "Windows/WindowsHWrapper.h"
#endif

MutexLock::MutexLock(void* Mutex)
	:MutexHandle(Mutex)
{
#ifdef PLATFORM_WINDOWS
	if (MutexHandle)
	{
		WaitForSingleObject(MutexHandle, INFINITE);
	}
#endif
}

MutexLock::~MutexLock()
{
#ifdef PLATFORM_WINDOWS
	if (MutexHandle)
	{
		ReleaseMutex(MutexHandle);
	}
#endif
}