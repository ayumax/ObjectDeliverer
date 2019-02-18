// Copyright 2019 ayumax. All Rights Reserved.

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "IObjectDeliverer.h"


class FObjectDeliverer : public IObjectDeliverer
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

IMPLEMENT_MODULE(FObjectDeliverer, ObjectDeliverer)



void FObjectDeliverer::StartupModule()
{
}


void FObjectDeliverer::ShutdownModule()
{
}



