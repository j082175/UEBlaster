// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Blaster : ModuleRules
{
	public Blaster(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new string[] { "Blaster" });
		/*		PublicIncludePathModuleNames.Add("Blaster");*/

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });

		/*		PublicDependencyModuleNames.AddRange(new string[] { "Blaster", "Niagara", "OnlineSubsystem", "OnlineSubsystemSteam", "MotionWarping", "ControlRig", "GeometryCollectionEngine", "AIModule", "NavigationSystem", "UMG", "FieldSystemEngine" });

				PublicDependencyModuleNames.AddRange(new string[] { "MultiplayerSessions", "GameplayCameras", "AnimGraphRuntime" });*/

		PublicDependencyModuleNames.AddRange(new string[] { "Niagara", "OnlineSubsystem", "OnlineSubsystemSteam", "MotionWarping", "ControlRig", "GeometryCollectionEngine", "AIModule", "NavigationSystem", "UMG", "FieldSystemEngine" });

		PublicDependencyModuleNames.AddRange(new string[] { "MultiplayerSessions", "GameplayCameras", "AnimGraphRuntime" });

		PublicDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		PublicDependencyModuleNames.AddRange(new string[] { "AdvancedSessions", "AdvancedSteamSessions" });

		PrivateDependencyModuleNames.AddRange(new string[] { /*"AnimGraph",*/ /*"PropertyEditor",*/ /*"EditorStyle",*/ /*"BlueprintGraph"*/ });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
