#pragma once

#include "CoreMinimal.h"

class MutexLock
{
public:
	MutexLock(void* Mutex);
	~MutexLock();

private:
	void* MutexHandle;
};
