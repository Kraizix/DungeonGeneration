// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DungeonGenerationEditorTarget : TargetRules
{
	public DungeonGenerationEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
		ExtraModuleNames.Add("DungeonGeneration");
	}
}