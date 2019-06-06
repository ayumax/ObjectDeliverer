// Copyright 2019 ayumax. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "TextureUtil.generated.h"

/**
 * 
 */
UCLASS()
class OBJECTDELIVERERTEST_API UTextureUtil : public UObject
{
	GENERATED_BODY()

public:
	UTextureUtil();
	~UTextureUtil();

	UFUNCTION(BlueprintPure, Category = "ObjectDeliverer Test")
	static UTexture2D* CreateTexture(int32 Width, int32 Height);

	UFUNCTION(BlueprintCallable, Category = "ObjectDeliverer Test")
	static void UpdateTexture(UTexture2D* Texture, const TArray<uint8>& PixelsBuffer);

	UFUNCTION(BlueprintCallable, Category = "ObjectDeliverer Test")
	static void GetPixelBufferFromRenderTarget(class UTextureRenderTarget2D* TextureRenderTarget, UPARAM(ref)TArray<uint8>& Buffer);

};
