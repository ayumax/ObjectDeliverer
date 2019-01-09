// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "CommNet/Public/TcpIpSocket.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeTcpIpSocket() {}
// Cross Module References
	COMMNET_API UClass* Z_Construct_UClass_UTcpIpSocket_NoRegister();
	COMMNET_API UClass* Z_Construct_UClass_UTcpIpSocket();
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
	UPackage* Z_Construct_UPackage__Script_CommNet();
	COMMNET_API UFunction* Z_Construct_UFunction_UTcpIpSocket_Connect();
// End Cross Module References
	void UTcpIpSocket::StaticRegisterNativesUTcpIpSocket()
	{
		UClass* Class = UTcpIpSocket::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "Connect", &UTcpIpSocket::execConnect },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_UTcpIpSocket_Connect_Statics
	{
		struct TcpIpSocket_eventConnect_Parms
		{
			FString IpAddress;
			int32 Port;
			bool ReturnValue;
		};
		static void NewProp_ReturnValue_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_Port;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_IpAddress_MetaData[];
#endif
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_IpAddress;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	void Z_Construct_UFunction_UTcpIpSocket_Connect_Statics::NewProp_ReturnValue_SetBit(void* Obj)
	{
		((TcpIpSocket_eventConnect_Parms*)Obj)->ReturnValue = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UTcpIpSocket_Connect_Statics::NewProp_ReturnValue = { UE4CodeGen_Private::EPropertyClass::Bool, "ReturnValue", RF_Public|RF_Transient|RF_MarkAsNative, (EPropertyFlags)0x0010000000000580, 1, nullptr, sizeof(bool), UE4CodeGen_Private::ENativeBool::Native, sizeof(TcpIpSocket_eventConnect_Parms), &Z_Construct_UFunction_UTcpIpSocket_Connect_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UTcpIpSocket_Connect_Statics::NewProp_Port = { UE4CodeGen_Private::EPropertyClass::Int, "Port", RF_Public|RF_Transient|RF_MarkAsNative, (EPropertyFlags)0x0010000000000080, 1, nullptr, STRUCT_OFFSET(TcpIpSocket_eventConnect_Parms, Port), METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UTcpIpSocket_Connect_Statics::NewProp_IpAddress_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UTcpIpSocket_Connect_Statics::NewProp_IpAddress = { UE4CodeGen_Private::EPropertyClass::Str, "IpAddress", RF_Public|RF_Transient|RF_MarkAsNative, (EPropertyFlags)0x0010000000000080, 1, nullptr, STRUCT_OFFSET(TcpIpSocket_eventConnect_Parms, IpAddress), METADATA_PARAMS(Z_Construct_UFunction_UTcpIpSocket_Connect_Statics::NewProp_IpAddress_MetaData, ARRAY_COUNT(Z_Construct_UFunction_UTcpIpSocket_Connect_Statics::NewProp_IpAddress_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UTcpIpSocket_Connect_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTcpIpSocket_Connect_Statics::NewProp_ReturnValue,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTcpIpSocket_Connect_Statics::NewProp_Port,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTcpIpSocket_Connect_Statics::NewProp_IpAddress,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UTcpIpSocket_Connect_Statics::Function_MetaDataParams[] = {
		{ "Category", "CommNet" },
		{ "ModuleRelativePath", "Public/TcpIpSocket.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UTcpIpSocket_Connect_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UTcpIpSocket, "Connect", RF_Public|RF_Transient|RF_MarkAsNative, nullptr, (EFunctionFlags)0x04020401, sizeof(TcpIpSocket_eventConnect_Parms), Z_Construct_UFunction_UTcpIpSocket_Connect_Statics::PropPointers, ARRAY_COUNT(Z_Construct_UFunction_UTcpIpSocket_Connect_Statics::PropPointers), 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UTcpIpSocket_Connect_Statics::Function_MetaDataParams, ARRAY_COUNT(Z_Construct_UFunction_UTcpIpSocket_Connect_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UTcpIpSocket_Connect()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UTcpIpSocket_Connect_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	UClass* Z_Construct_UClass_UTcpIpSocket_NoRegister()
	{
		return UTcpIpSocket::StaticClass();
	}
	struct Z_Construct_UClass_UTcpIpSocket_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UTcpIpSocket_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UObject,
		(UObject* (*)())Z_Construct_UPackage__Script_CommNet,
	};
	const FClassFunctionLinkInfo Z_Construct_UClass_UTcpIpSocket_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_UTcpIpSocket_Connect, "Connect" }, // 677282112
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTcpIpSocket_Statics::Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IncludePath", "TcpIpSocket.h" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Public/TcpIpSocket.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UTcpIpSocket_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UTcpIpSocket>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UTcpIpSocket_Statics::ClassParams = {
		&UTcpIpSocket::StaticClass,
		DependentSingletons, ARRAY_COUNT(DependentSingletons),
		0x001000A0u,
		FuncInfo, ARRAY_COUNT(FuncInfo),
		nullptr, 0,
		nullptr,
		&StaticCppClassTypeInfo,
		nullptr, 0,
		METADATA_PARAMS(Z_Construct_UClass_UTcpIpSocket_Statics::Class_MetaDataParams, ARRAY_COUNT(Z_Construct_UClass_UTcpIpSocket_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UTcpIpSocket()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UTcpIpSocket_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UTcpIpSocket, 3765173996);
	static FCompiledInDefer Z_CompiledInDefer_UClass_UTcpIpSocket(Z_Construct_UClass_UTcpIpSocket, &UTcpIpSocket::StaticClass, TEXT("/Script/CommNet"), TEXT("UTcpIpSocket"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UTcpIpSocket);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
