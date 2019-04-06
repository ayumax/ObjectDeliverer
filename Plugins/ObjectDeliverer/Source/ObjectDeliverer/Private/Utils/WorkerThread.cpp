// Copyright 2019 ayumax. All Rights Reserved.
#include "WorkerThread.h"
#include "Runtime/Core/Public/HAL/PlatformProcess.h"

FWorkerThread::FWorkerThread(TFunction<bool()> InWork, float WaitSeconds)
	: Work(InWork)
	, End([]() {})
	, Seconds(WaitSeconds)
	, ContinueRun(true)
{

}

FWorkerThread::FWorkerThread(TFunction<bool()> InWork, TFunction<void()> InEnd, float WaitSeconds)
	: Work(InWork)
	, End(InEnd)
	, Seconds(WaitSeconds)
	, ContinueRun(true)
{

}

FWorkerThread::~FWorkerThread()
{

}

uint32 FWorkerThread::Run()
{
	while (ContinueRun)
	{
		if (!Work())
		{
			return 0;
		}


		if (ContinueRun)
		{
			FPlatformProcess::Sleep(Seconds);
		}
	}

	return 0;
}

void FWorkerThread::Stop()
{
	ContinueRun = false;

}

void FWorkerThread::Exit()
{
	ContinueRun = false;

	End();
}
