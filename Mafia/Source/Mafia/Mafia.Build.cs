// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Mafia : ModuleRules
{
	public Mafia(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		// 시작 경로
        PublicIncludePaths.AddRange(new string[]
        {
            "Mafia",
			"Mafia/GameFeatures",
			"Mafia/MafiaCore",
		});
        
		// 모듈
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			
		});

		PublicDependencyModuleNames.AddRange(new string[]
		{
            "UMG",
        });

		bUseUnity = false;
	}
}
