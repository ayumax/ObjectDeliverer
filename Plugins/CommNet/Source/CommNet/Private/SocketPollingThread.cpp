#include "SocketPollingThread.h"
#include "Runtime/Core/Public/HAL/PlatformProcess.h"

FSocketPollingThread::FSocketPollingThread(TFunction<void()> InWork)
	: Work(InWork)
	, ContinueRun(true)
{

}

FSocketPollingThread::~FSocketPollingThread()
{

}

uint32 FSocketPollingThread::Run()
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

void FSocketPollingThread::Stop()
{
	ContinueRun = false;

}

void FSocketPollingThread::Exit()
{

}
