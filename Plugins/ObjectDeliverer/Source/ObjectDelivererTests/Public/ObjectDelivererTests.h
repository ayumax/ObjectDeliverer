// Copyright 2019 ayumax. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

// APIマクロの定義
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4668)
#endif

// モジュールのAPIマクロを定義
#if PLATFORM_WINDOWS
	#ifdef OBJECTDELIVERERTESTS_API
		#undef OBJECTDELIVERERTESTS_API
	#endif
	#if OBJECTDELIVERERTESTS_EXPORTS
		#define OBJECTDELIVERERTESTS_API __declspec(dllexport)
	#else
		#define OBJECTDELIVERERTESTS_API __declspec(dllimport)
	#endif
#else
	#define OBJECTDELIVERERTESTS_API
#endif

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

class FObjectDelivererTestsModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
