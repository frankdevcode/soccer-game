// Copyright Frank Dev Code. All Rights Reserved.

using UnrealBuildTool;

public class SoccerGameEditor : ModuleRules
{
	public SoccerGameEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
		{ 
			"Core", 
			"CoreUObject", 
			"Engine",
			"UnrealEd",
			"SoccerGame"
		});

		PrivateDependencyModuleNames.AddRange(new string[] 
		{ 
		});

		// ...
	}
}
