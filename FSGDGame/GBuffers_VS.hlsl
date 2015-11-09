#ifndef GBUFFERS_VS_HLSL
#define GBUFFERS_VS_HLSL

#include "VertexLayouts.hlsli"
#include "../EDRendererD3D/ConstantBuffers.h"

GBufferVertexOut main(VERTEX_POSNORMTEX input) {
	GBufferVertexOut output = (GBufferVertexOut)0;
	output.position = mul(float4(input.position, 1.0f), gMVP);
	output.normalW = normalize(mul(float4(input.normal, 1.0f), gWorld)).rgb;
	output.texCoord = input.texCoord;
	output.depthDiv = output.position.zw;
	return output;
}

#endif //GBUFFERS_VS_HLSL
