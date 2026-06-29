using UnrealBuildTool;

public class ComputeShaderDemoEditorTarget : TargetRules
{
	public ComputeShaderDemoEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
		ExtraModuleNames.Add("ComputeShaderDemo");
		CppCompileWarningSettings.UndefinedIdentifierWarningLevel = WarningLevel.Error;
	}
}
