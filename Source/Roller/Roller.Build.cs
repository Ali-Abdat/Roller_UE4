// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Roller : ModuleRules
{
    public Roller(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" , "UMG" });
	}
}
