// Copyright 2019 ayumax. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"


class OBJECTDELIVERER_API FODWorkerThread : public FRunnable
{
public:
	FODWorkerThread(TFunction<bool()> InWork, float WaitSeconds = 0.001f);
	FODWorkerThread(TFunction<bool()> InWork, TFunction<void()> InEnd, float WaitSeconds = 0.001f);
	~FODWorkerThread();

	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;

private:
	TFunction<bool()> Work;
	TFunction<void()> End;
	float Seconds;
	volatile bool ContinueRun;
};
