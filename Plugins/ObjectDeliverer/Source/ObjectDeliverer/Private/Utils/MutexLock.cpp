// Copyright 2019 ayumax. All Rights Reserved.
#include "MutexLock.h"
#if PLATFORM_WINDOWS
#include "Windows/WindowsHWrapper.h"
#endif


void MutexLock::Lock(void* Mutex, TFunction<void()> InWork)
{
#ifdef PLATFORM_WINDOWS
	if (Mutex)
	{
		WaitForSingleObject(Mutex, INFINITE);
	}

	InWork();

	if (Mutex)
	{
		ReleaseMutex(Mutex);
	}
#endif
}