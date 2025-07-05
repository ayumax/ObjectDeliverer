// Copyright 2019 ayumax. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class ObjectDelivererTests : ModuleRules
{
	public ObjectDelivererTests(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		CppCompileWarningSettings.ShadowVariableWarningLevel = WarningLevel.Warning;
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"Sockets",
				"Networking",
				"Json",
				"JsonUtilities"
			}
		);
        
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"ObjectDeliverer",
				"Slate",
				"SlateCore"
			}
		);
		
		if (Target.bBuildEditor)
		{
			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					"UnrealEd",
					"FunctionalTesting"
				}
			);
		}
	}
}