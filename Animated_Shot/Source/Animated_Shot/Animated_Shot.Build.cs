// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Animated_Shot : ModuleRules
{
	public Animated_Shot(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new string[] { "Animated_Shot" });

		PublicDependencyModuleNames.AddRange(new string[] { "ProtobufCore","Sockets", "Networking","Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "MovieScene", "LevelSequence", "MovieSceneTracks", "UMG", "Niagara", "NavigationSystem","AIModule", "GameplayTasks", "Paper2D"});

		PrivateIncludePaths.AddRange(new string[]
		{	"Animated_Shot/",
			"Animated_Shot/Network/",
			"Animated_Shot/Game/"
		});
		
		PublicDefinitions.Add("ENABLE_DRAW_DEBUG=1");
	}
}
