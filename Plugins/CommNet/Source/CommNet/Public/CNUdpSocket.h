#pragma once

#include "CoreMinimal.h"
#include "CNSocketBase.h"
#include "CNUdpSocket.generated.h"

UCLASS(BlueprintType, Blueprintable)
class COMMNET_API UCNUdpSocket : public UCNSocketBase
{
	GENERATED_BODY()

public:
	UCNUdpSocket();
	~UCNUdpSocket();

	UFUNCTION(BlueprintCallable, Category = "CommNet")
		void Start();

	UFUNCTION(BlueprintCallable, Category = "CommNet")
		void Close();

protected:
	virtual void OnStart() {}
	virtual void OnClose() {}

};
