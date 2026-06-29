//// Copyright Epic Games, Inc. All Rights Reserved.

//using System.IO;
//using UnrealBuildTool;

//public class CustomSSGI : ModuleRules
//{
//	public CustomSSGI(ReadOnlyTargetRules Target) : base(Target)
//	{
//		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

//        // ?? ?? ???? ??? Private ?? ??? ???? ??? ??!
//        string RendererPrivatePath = Path.Combine(GetModuleDirectory("Renderer"), "Private");
//        PrivateIncludePaths.Add(RendererPrivatePath);

//        PublicIncludePaths.AddRange(
//			new string[] {
//				// ... add public include paths required here ...
//				//System.IO.Path.Combine(EngineDirectory, "Source/Runtime/Renderer/Private")
//            }
//			);
				
		
//		PrivateIncludePaths.AddRange(
//			new string[] { 
//				// ... add other private include paths required here ...
//				//System.IO.Path.Combine(EngineDirectory, "Source/Runtime/Renderer/Private")
//            }
//			);

//        PublicDependencyModuleNames.AddRange(
//			new string[]
//			{
//				"Core",
//				// ... add other public dependencies that you statically link with here ...
//			}
//			);

//		PublicDependencyModuleNames.AddRange(    
//			new string[] 
//			{        
//				"Core",        
//				"CoreUObject",      
//				"Engine",
//				"RenderCore",		// ??? ????? ??? ?? ??!
//				"RHI",				// GPU ???? ?? ??? ?? ??!
//                "Projects",			// <-- ???? ??? ?? ?? ??!
//                "Renderer"			// ?? ? ??? ????? G-Buffer? ??? ? ????!
//			}
//			);
			
		
//		PrivateDependencyModuleNames.AddRange(
//			new string[]
//			{
//				"CoreUObject",
//				"Engine",
//				"Slate",
//				"SlateCore",
//				// ... add private dependencies that you statically link with here ...	
//			}
//			);
		
		
//		DynamicallyLoadedModuleNames.AddRange(
//			new string[]
//			{
//				// ... add any modules that your module loads dynamically here ...
//			}
//			);
//	}
//}

// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class CustomSSGI : ModuleRules
{
    public CustomSSGI(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        // ?? ?? ???? ??? Private ?? ??? ???? ??? ??! (??? ????)
        string RendererPrivatePath = Path.Combine(GetModuleDirectory("Renderer"), "Private");
        PrivateIncludePaths.Add(RendererPrivatePath);

        // ??? ???? ??? ??? ???? ??????.
        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "RenderCore",		// ??? ????? ??? ?? ??!
				"RHI",				// GPU ???? ?? ??? ?? ??!
				"Projects",			// ???? ??? ?? ?? ??!
				"Renderer"			// ?? G-Buffer ? ??? ?? ??? ?? ??!
			}
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Slate",
                "SlateCore"
            }
        );
    }
}
