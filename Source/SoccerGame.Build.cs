// Copyright Frank Dev Code. All Rights Reserved.

using UnrealBuildTool;

public class SoccerGame : ModuleRules
{
	public SoccerGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		// Core Unreal Engine modules
		PublicDependencyModuleNames.AddRange(new string[] 
		{ 
			"Core",
			"CoreUObject",
			"Engine",
			"EnhancedInput",
			"EnhancedInputEditor",
			"InputCore",
			"Slate",
			"SlateCore",
			"UMG",
			"PhysicsCore",
			"MediaAssets",
			"PixelStreaming"
		});

		// Private dependency modules
		PrivateDependencyModuleNames.AddRange(new string[] 
		{ 
			"Engine",
			"Slate",
			"SlateCore"
		});

		// Online and networking
		if (Target.Platform == UnrealTargetPlatform.Win64 || Target.Platform == UnrealTargetPlatform.Linux)
		{
			PublicDependencyModuleNames.AddRange(new string[]
			{
				"OnlineSubsystem",
				"OnlineSubsystemSteam"
			});
		}

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
