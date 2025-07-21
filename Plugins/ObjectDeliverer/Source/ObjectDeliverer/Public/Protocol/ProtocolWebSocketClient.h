// Copyright 2019 ayumax. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "ObjectDelivererProtocol.h"
#include "IWebSocket.h"
#include "ProtocolWebSocketClient.generated.h"

UCLASS(BlueprintType, Blueprintable)
class OBJECTDELIVERER_API UProtocolWebSocketClient : public UObjectDelivererProtocol
{
	GENERATED_BODY()

public:
	UProtocolWebSocketClient();
	~UProtocolWebSocketClient();

	/**
	 * Initialize WebSocket client.
	 * @param Url - The WebSocket URL to connect to (e.g., ws://127.0.0.1:8080 or wss://example.com)
	 */
	UFUNCTION(BlueprintCallable, Category = "ObjectDeliverer|Protocol")
	void Initialize(const FString& Url = "ws://127.0.0.1:8080");

	/**
	 * Initialize WebSocket client with protocols.
	 * @param Url - The WebSocket URL to connect to
	 * @param Protocols - Array of sub-protocols
	 */
	UFUNCTION(BlueprintCallable, Category = "ObjectDeliverer|Protocol")
	void InitializeWithProtocols(const FString& Url, const TArray<FString>& Protocols);

	/**
	 * Initialize WebSocket client with full options.
	 * @param Url - The WebSocket URL to connect to
	 * @param Protocols - Array of sub-protocols
	 * @param Headers - Headers to send with the connection request
	 */
	UFUNCTION(BlueprintCallable, Category = "ObjectDeliverer|Protocol")
	void InitializeWithHeaders(const FString& Url, const TArray<FString>& Protocols, const TMap<FString, FString>& Headers);

	/**
	 * Set whether to automatically reconnect after disconnection.
	 * @param bAutoReconnect - If true, will attempt to reconnect after disconnection
	 */
	UFUNCTION(BlueprintCallable, Category = "ObjectDeliverer|Protocol")
	void SetAutoReconnect(bool bAutoReconnect);

	/**
	 * Set the reconnection delay in seconds.
	 * @param Delay - Delay in seconds before attempting to reconnect
	 */
	UFUNCTION(BlueprintCallable, Category = "ObjectDeliverer|Protocol")
	void SetReconnectDelay(float Delay);

	virtual void Start() override;
	virtual void Close() override;
	virtual void Send(const TArray<uint8>& DataBuffer, const FDeliveryDataType& KindOfData) const override;
	virtual void RequestSend(const TArray<uint8> &DataBuffer, const FDeliveryDataType& DataType) override;

private:
	void OnWebSocketConnected();
	void OnWebSocketConnectionError(const FString& Error);
	void OnWebSocketClosed(int32 StatusCode, const FString& Reason, bool bWasClean);
	void OnWebSocketMessage(const FString& Message);
	void OnWebSocketBinaryMessage(const void* Data, SIZE_T Size, bool bIsLastFragment);

	void CreateWebSocket();
	bool TryReconnect();

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "ObjectDeliverer|Protocol")
	FString ServerUrl = "ws://localhost:8080";

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "ObjectDeliverer|Protocol")
	TArray<FString> SubProtocols;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "ObjectDeliverer|Protocol")
	TMap<FString, FString> ConnectionHeaders;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "ObjectDeliverer|Protocol")
	bool bAutoReconnect = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "ObjectDeliverer|Protocol")
	float ReconnectDelay = 5.0f;

protected:
	TSharedPtr<IWebSocket> WebSocket;
	class FODWorkerThread* ReconnectInnerThread = nullptr;
	class FRunnableThread* ReconnectThread = nullptr;
	bool bIsManualClose = false;
	bool bIsReconnecting = false;
};