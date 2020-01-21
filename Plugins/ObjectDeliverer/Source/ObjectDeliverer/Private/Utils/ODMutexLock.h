// Copyright 2019 ayumax. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"

class ODMutexLock
{
public:
	static void Lock(void* Mutex, TFunction<void()> InWork);

};
