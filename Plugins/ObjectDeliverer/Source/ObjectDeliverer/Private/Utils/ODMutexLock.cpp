// Copyright 2019 ayumax. All Rights Reserved.
#include "ODMutexLock.h"
#if PLATFORM_WINDOWS
#include "Windows/WindowsHWrapper.h"
#endif


void ODMutexLock::Lock(void* Mutex, TFunction<void()> InWork)
{
#if PLATFORM_WINDOWS
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