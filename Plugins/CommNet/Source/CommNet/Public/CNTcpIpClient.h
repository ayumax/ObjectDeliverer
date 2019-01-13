#pragma once

#include "CoreMinimal.h"
#include "CNTcpIpSocket.h"
#include "CNTcpIpClient.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTcpIpClientConnected, class UCNTcpIpSocket*, ClientSocket);


UCLASS(BlueprintType, Blueprintable)
class COMMNET_API UCNTcpIpClient : public UCNTcpIpSocket
{
	GENERATED_BODY()

public:
	UCNTcpIpClient();
	~UCNTcpIpClient();

	/**
	 * Initialize TCP/IP server.
	 * @param IpAddress - The ip address of the connection destination.
	 * @param Port - The port number of the connection destination.
	 */
	UFUNCTION(BlueprintCallable, Category = "CommNet")
	void Initialize(const FString& IpAddress, int32 Port);

	virtual void Start_Implementation() override;

public:
	UPROPERTY(BlueprintAssignable, Category = "CommNet")
	FTcpIpClientConnected Connected;

	UPROPERTY(EditAnywhere, Category = "CommNet")
	FString ServerIpAddress;

	UPROPERTY(EditAnywhere, Category = "CommNet")
	int32 ServerPort;
};
