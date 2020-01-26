// Pierre Even et Mathieu Voyer, tous droits reservés

using UnrealBuildTool;
using System.Collections.Generic;

public class LePostierDeFakaravaEditorTarget : TargetRules
{
	public LePostierDeFakaravaEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "LePostierDeFakarava" } );
	}
}
