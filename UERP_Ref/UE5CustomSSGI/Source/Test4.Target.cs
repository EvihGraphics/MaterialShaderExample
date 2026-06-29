using UnrealBuildTool;

public class Test4Target : TargetRules
{
	public Test4Target(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
		ExtraModuleNames.Add("Test4");
		CppCompileWarningSettings.UndefinedIdentifierWarningLevel = WarningLevel.Error;
	}
}
