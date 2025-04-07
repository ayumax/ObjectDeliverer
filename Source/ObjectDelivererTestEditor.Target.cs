// Copyright 2019 ayumax. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ObjectDelivererTestEditorTarget : TargetRules
{
    public ObjectDelivererTestEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
        CppStandard = CppStandardVersion.Cpp20;
        ExtraModuleNames.Add("ObjectDelivererTest");
    }
}
