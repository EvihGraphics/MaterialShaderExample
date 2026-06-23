#include "AVBOIT/Backend/AVBOITBackendShaders.h"
#include "ShaderParameterUtils.h"

IMPLEMENT_GLOBAL_SHADER(FAVBOITClearCS, "/Plugin/MaterialShaderExample/AVBOIT/Backend/AVBOITClear.usf", "ClearCS", SF_Compute);
IMPLEMENT_GLOBAL_SHADER(FAVBOITBuildCS, "/Plugin/MaterialShaderExample/AVBOIT/Backend/AVBOITBuild.usf", "BuildCS", SF_Compute);
IMPLEMENT_GLOBAL_SHADER(FAVBOITIntegrateCS, "/Plugin/MaterialShaderExample/AVBOIT/Backend/AVBOITIntegrate.usf", "IntegrateCS", SF_Compute);
IMPLEMENT_GLOBAL_SHADER(FAVBOITCompositeCS, "/Plugin/MaterialShaderExample/AVBOIT/Backend/AVBOITComposite.usf", "CompositeCS", SF_Compute);
IMPLEMENT_GLOBAL_SHADER(FAVBOITShadeCS, "/Plugin/MaterialShaderExample/AVBOIT/Backend/AVBOITShade.usf", "ShadeCS", SF_Compute);
