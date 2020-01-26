// Pierre Even et Mathieu Voyer, tous droits reservés

using UnrealBuildTool;
using System.Collections.Generic;

public class LePostierDeFakaravaTarget : TargetRules
{
	public LePostierDeFakaravaTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "LePostierDeFakarava" } );
    }
}
