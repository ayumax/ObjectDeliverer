#include "WorkerThread.h"
#include "Runtime/Core/Public/HAL/PlatformProcess.h"

FWorkerThread::FWorkerThread(TFunction<void()> InWork)
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
		Work();

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
