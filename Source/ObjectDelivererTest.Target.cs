// Copyright 2019 ayumax. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ObjectDelivererTestTarget : TargetRules
{
    public ObjectDelivererTestTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.Latest;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        ExtraModuleNames.Add("ObjectDelivererTest");
    }
}
