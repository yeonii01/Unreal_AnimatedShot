// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Animated_Shot : ModuleRules
{
	public Animated_Shot(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new string[] { "Animated_Shot" });

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "Niagara"});

		PublicDefinitions.Add("ENABLE_DRAW_DEBUG=1");
	}
}
