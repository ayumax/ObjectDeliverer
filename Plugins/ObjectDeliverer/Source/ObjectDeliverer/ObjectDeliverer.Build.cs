// Copyright 2019 ayumax. All Rights Reserved.
using System.IO;

namespace UnrealBuildTool.Rules
{
	public class ObjectDeliverer : ModuleRules
	{
		public ObjectDeliverer(ReadOnlyTargetRules Target) : base(Target)
		{
            PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
            
            PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Private"));

            PublicDependencyModuleNames.AddRange(
				new string[]
				{
                    "Core",
                    "CoreUObject",
                    "Engine",
                    "Sockets",
                    "Networking",
                    "Json",
                    "JsonUtilities",
                    "WebSockets"
                }
				);
		}
	}
}
