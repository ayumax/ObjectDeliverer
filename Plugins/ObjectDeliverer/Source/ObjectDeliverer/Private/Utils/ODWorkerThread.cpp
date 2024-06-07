// Copyright 2019 ayumax. All Rights Reserved.
#include "ODWorkerThread.h"
#include "HAL/PlatformProcess.h"

FODWorkerThread::FODWorkerThread(TFunction<bool()> InWork, float WaitSeconds)
	: Work(InWork)
	, End([]() {})
	, Seconds(WaitSeconds)
	, ContinueRun(true)
{

}

FODWorkerThread::FODWorkerThread(TFunction<bool()> InWork, TFunction<void()> InEnd, float WaitSeconds)
	: Work(InWork)
	, End(InEnd)
	, Seconds(WaitSeconds)
	, ContinueRun(true)
{

}

FODWorkerThread::~FODWorkerThread()
{

}

uint32 FODWorkerThread::Run()
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

void FODWorkerThread::Stop()
{
	ContinueRun = false;

}

void FODWorkerThread::Exit()
{
	ContinueRun = false;

	End();
}
