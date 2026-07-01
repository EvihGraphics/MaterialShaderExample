using System.IO;
using UnrealBuildTool;

public class MaterialShaderExampleNiagara : ModuleRules
{
	public MaterialShaderExampleNiagara(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		bUseUnity = false;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"MaterialShaderExample",
			"Niagara",
			"NiagaraCore",
			"NiagaraVertexFactories",
			"RenderCore",
			"Renderer",
			"RHI"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"ApplicationCore",
			"AppFramework",
			"ImageWrapper",
			"InputCore",
			"Json",
			"JsonUtilities",
			"Projects",
			"Slate",
			"SlateCore"
		});

		if (Target.bBuildEditor)
		{
			PrivateDependencyModuleNames.AddRange(new string[]
			{
				"LevelEditor",
				"UnrealEd"
			});
		}

		PrivateIncludePaths.AddRange(new string[]
		{
			Path.Combine(GetModuleDirectory("Renderer"), "Private"),
			Path.Combine(GetModuleDirectory("Renderer"), "Internal")
		});
	}
}
