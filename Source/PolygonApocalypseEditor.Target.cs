// Copyright (c) 2024 Synty Studios Limited. All rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class PolygonApocalypseEditorTarget : TargetRules
{
	public PolygonApocalypseEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "PolygonApocalypse" } );
	}
}
