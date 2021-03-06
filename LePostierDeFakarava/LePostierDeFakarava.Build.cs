// Pierre Even et Mathieu Voyer, tous droits reservés

using UnrealBuildTool;

public class LePostierDeFakarava : ModuleRules
{
	public LePostierDeFakarava(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "ProceduralMeshComponent", "SimplexNoise", "ProceduralLandscapeTool" });

        bEnableExceptions = true;

        PrivateDependencyModuleNames.AddRange(new string[] { "ProceduralLandscapeTool" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
