// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef COMMNET_TcpIpSocket_generated_h
#error "TcpIpSocket.generated.h already included, missing '#pragma once' in TcpIpSocket.h"
#endif
#define COMMNET_TcpIpSocket_generated_h

#define CommNet_Plugins_ayumax_CommNet_Source_CommNet_Public_TcpIpSocket_h_9_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execConnect) \
	{ \
		P_GET_PROPERTY(UStrProperty,Z_Param_IpAddress); \
		P_GET_PROPERTY(UIntProperty,Z_Param_Port); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=P_THIS->Connect(Z_Param_IpAddress,Z_Param_Port); \
		P_NATIVE_END; \
	}


#define CommNet_Plugins_ayumax_CommNet_Source_CommNet_Public_TcpIpSocket_h_9_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execConnect) \
	{ \
		P_GET_PROPERTY(UStrProperty,Z_Param_IpAddress); \
		P_GET_PROPERTY(UIntProperty,Z_Param_Port); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=P_THIS->Connect(Z_Param_IpAddress,Z_Param_Port); \
		P_NATIVE_END; \
	}


#define CommNet_Plugins_ayumax_CommNet_Source_CommNet_Public_TcpIpSocket_h_9_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUTcpIpSocket(); \
	friend struct Z_Construct_UClass_UTcpIpSocket_Statics; \
public: \
	DECLARE_CLASS(UTcpIpSocket, UObject, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/CommNet"), NO_API) \
	DECLARE_SERIALIZER(UTcpIpSocket)


#define CommNet_Plugins_ayumax_CommNet_Source_CommNet_Public_TcpIpSocket_h_9_INCLASS \
private: \
	static void StaticRegisterNativesUTcpIpSocket(); \
	friend struct Z_Construct_UClass_UTcpIpSocket_Statics; \
public: \
	DECLARE_CLASS(UTcpIpSocket, UObject, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/CommNet"), NO_API) \
	DECLARE_SERIALIZER(UTcpIpSocket)


#define CommNet_Plugins_ayumax_CommNet_Source_CommNet_Public_TcpIpSocket_h_9_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UTcpIpSocket(const FObjectInitializer& ObjectInitializer); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UTcpIpSocket) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UTcpIpSocket); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UTcpIpSocket); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UTcpIpSocket(UTcpIpSocket&&); \
	NO_API UTcpIpSocket(const UTcpIpSocket&); \
public:


#define CommNet_Plugins_ayumax_CommNet_Source_CommNet_Public_TcpIpSocket_h_9_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UTcpIpSocket(UTcpIpSocket&&); \
	NO_API UTcpIpSocket(const UTcpIpSocket&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UTcpIpSocket); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UTcpIpSocket); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(UTcpIpSocket)


#define CommNet_Plugins_ayumax_CommNet_Source_CommNet_Public_TcpIpSocket_h_9_PRIVATE_PROPERTY_OFFSET
#define CommNet_Plugins_ayumax_CommNet_Source_CommNet_Public_TcpIpSocket_h_6_PROLOG
#define CommNet_Plugins_ayumax_CommNet_Source_CommNet_Public_TcpIpSocket_h_9_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	CommNet_Plugins_ayumax_CommNet_Source_CommNet_Public_TcpIpSocket_h_9_PRIVATE_PROPERTY_OFFSET \
	CommNet_Plugins_ayumax_CommNet_Source_CommNet_Public_TcpIpSocket_h_9_RPC_WRAPPERS \
	CommNet_Plugins_ayumax_CommNet_Source_CommNet_Public_TcpIpSocket_h_9_INCLASS \
	CommNet_Plugins_ayumax_CommNet_Source_CommNet_Public_TcpIpSocket_h_9_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define CommNet_Plugins_ayumax_CommNet_Source_CommNet_Public_TcpIpSocket_h_9_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	CommNet_Plugins_ayumax_CommNet_Source_CommNet_Public_TcpIpSocket_h_9_PRIVATE_PROPERTY_OFFSET \
	CommNet_Plugins_ayumax_CommNet_Source_CommNet_Public_TcpIpSocket_h_9_RPC_WRAPPERS_NO_PURE_DECLS \
	CommNet_Plugins_ayumax_CommNet_Source_CommNet_Public_TcpIpSocket_h_9_INCLASS_NO_PURE_DECLS \
	CommNet_Plugins_ayumax_CommNet_Source_CommNet_Public_TcpIpSocket_h_9_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID CommNet_Plugins_ayumax_CommNet_Source_CommNet_Public_TcpIpSocket_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
