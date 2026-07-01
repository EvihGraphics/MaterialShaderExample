// Copyright

#include "AVBOIT/Raster/AVBOITRasterShaders.h"

IMPLEMENT_GLOBAL_SHADER(FAVBOITRasterSplatVS, "/Plugin/MaterialShaderExample/AVBOIT/Raster/AVBOITRasterSplat.usf", "MainVS", SF_Vertex);
IMPLEMENT_GLOBAL_SHADER(FAVBOITRasterSplatPS, "/Plugin/MaterialShaderExample/AVBOIT/Raster/AVBOITRasterSplat.usf", "MainPS", SF_Pixel);

IMPLEMENT_GLOBAL_SHADER(FAVBOITRasterForwardVS, "/Plugin/MaterialShaderExample/AVBOIT/Raster/AVBOITRasterForward.usf", "MainVS", SF_Vertex);
IMPLEMENT_GLOBAL_SHADER(FAVBOITRasterForwardPS, "/Plugin/MaterialShaderExample/AVBOIT/Raster/AVBOITRasterForward.usf", "MainPS", SF_Pixel);

IMPLEMENT_GLOBAL_SHADER(FAVBOITRasterIdentityVS, "/Plugin/MaterialShaderExample/AVBOIT/Raster/AVBOITRasterIdentity.usf", "MainVS", SF_Vertex);
IMPLEMENT_GLOBAL_SHADER(FAVBOITRasterIdentityPS, "/Plugin/MaterialShaderExample/AVBOIT/Raster/AVBOITRasterIdentity.usf", "MainPS", SF_Pixel);

IMPLEMENT_GLOBAL_SHADER(FAVBOITRasterResolvedAlphaCS, "/Plugin/MaterialShaderExample/AVBOIT/Raster/AVBOITRasterResolvedAlpha.usf", "MainCS", SF_Compute);

IMPLEMENT_GLOBAL_SHADER(FAVBOITRasterCompositePS, "/Plugin/MaterialShaderExample/AVBOIT/Raster/AVBOITRasterComposite.usf", "MainPS", SF_Pixel);

IMPLEMENT_GLOBAL_SHADER(FAVBOITRasterDebugExtractCS, "/Plugin/MaterialShaderExample/AVBOIT/Raster/AVBOITRasterDebugExtract.usf", "MainCS", SF_Compute);
