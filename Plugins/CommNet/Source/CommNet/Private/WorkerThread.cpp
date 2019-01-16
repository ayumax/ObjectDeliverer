#include "WorkerThread.h"
#include "Runtime/Core/Public/HAL/PlatformProcess.h"

FWorkerThread::FWorkerThread(TFunction<bool()> InWork)
	: Work(InWork)
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
			FPlatformProcess::Sleep(0.001);
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
}
