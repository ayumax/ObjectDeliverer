#pragma once

#include "CoreMinimal.h"
#include "Runtime/Core/Public/HAL/Runnable.h"
#include "Runtime/Core/Public/HAL/RunnableThread.h"


class COMMNET_API FWorkerThread : public FRunnable
{
public:
	FWorkerThread(TFunction<bool()> InWork);
	~FWorkerThread();

	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;

private:
	TFunction<bool()> Work;
	volatile bool ContinueRun;
};
