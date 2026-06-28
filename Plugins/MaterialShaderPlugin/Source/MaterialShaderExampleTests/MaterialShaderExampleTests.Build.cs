using UnrealBuildTool;

public class MaterialShaderExampleTests : ModuleRules
{
	public MaterialShaderExampleTests(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"UnrealEd",
				"RenderCore",
				"RHI",
				"Renderer",
				"Projects",
				"Json",
				"JsonUtilities",
				"ImageWrapper",
				"MaterialShaderExample"
			}
		);
	}
}
