// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class Building_EscapeTarget : TargetRules
{
	public Building_EscapeTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "Building_Escape" } );

		ExtraModuleNames.AddRange(new string[] { "FirstPersonCharacter" });
	}
}
