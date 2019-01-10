#pragma once

#include "CoreMinimal.h"
#include "Runtime/Core/Public/HAL/Runnable.h"
#include "Runtime/Core/Public/HAL/RunnableThread.h"


class COMMNET_API FSocketPollingThread : public FRunnable
{
public:
	FSocketPollingThread(TFunction<void()> InWork);
	~FSocketPollingThread();

	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;

private:
	TFunction<void()> Work;
	volatile bool ContinueRun;
};
