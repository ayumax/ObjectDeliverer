#pragma once

#include "CoreMinimal.h"

class MutexLock
{
public:
	static void Lock(void* Mutex, TFunction<void()> InWork);

};
