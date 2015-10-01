#ifndef GBUFFERS_BUMP_PS_HLSL
#define GBUFFERS_BUMP_PS_HLSL

#include "VertexLayouts.hlsli"
#include "TexturesAndSamplers.hlsli"
#include "../EDRendererD3D/ConstantBuffers.h"
#include "GBuffersShared.hlsli"

GBufferFragOut main(GBufferBumpVertexOut input)
{
	GBufferFragOut output = (GBufferFragOut)0;
	return output;
}

#endif //GBUFFERS_BUMP_PS_HLSL
