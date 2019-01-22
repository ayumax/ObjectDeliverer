// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyClass.generated.h"

/**
 * 
 */
UCLASS()
class OBJECTDELIVERERTEST_API MyClass : public UObject
{
	GENERATED_BODY()

public:
	MyClass();
	~MyClass();

	void Start();

	UFUNCTION()
		void OnConnect(UObjectDelivererProtocol* ClientSocket);
	UFUNCTION()
		void OnDisConnect(UObjectDelivererProtocol* ClientSocket);
	UFUNCTION()
		void OnReceive(UObjectDelivererProtocol* ClientSocket, const TArray<uint8>& Buffer);
};
