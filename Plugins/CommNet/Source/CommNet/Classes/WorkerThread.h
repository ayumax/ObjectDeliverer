#pragma once

#include "CoreMinimal.h"
#include "Runtime/Core/Public/HAL/Runnable.h"
#include "Runtime/Core/Public/HAL/RunnableThread.h"


class COMMNET_API FWorkerThread : public FRunnable
{
public:
	FWorkerThread(TFunction<void()> InWork);
	~FWorkerThread();

	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;

private:
	TFunction<void()> Work;
	volatile bool ContinueRun;
};
