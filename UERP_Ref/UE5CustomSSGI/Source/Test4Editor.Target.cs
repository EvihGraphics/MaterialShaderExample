using UnrealBuildTool;

public class Test4EditorTarget : TargetRules
{
	public Test4EditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
		ExtraModuleNames.Add("Test4");
		CppCompileWarningSettings.UndefinedIdentifierWarningLevel = WarningLevel.Error;
	}
}
