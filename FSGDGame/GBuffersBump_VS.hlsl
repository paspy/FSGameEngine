#ifndef GBUFFERS_BUMP_VS_HLSL
#define GBUFFERS_BUMP_VS_HLSL

#include "VertexLayouts.hlsli"
#include "../EDRendererD3D/ConstantBuffers.h"

GBufferBumpVertexOut main(VERTEX_POSNORMTANTEX input) {
	GBufferBumpVertexOut output = (GBufferBumpVertexOut)0;
	output.position = mul(float4(input.position, 1.0f), gMVP);
	//output.normalW = normalize(mul(float4(input.normal, 1.0f), gWorld)).rgb;

	output.tbn[2] = normalize(mul(float4(input.normal, 0.0f), gWorld)).rgb;
	output.tbn[0] = normalize(mul(float4(input.tangent, 0.0f), gWorld)).rgb;
	output.tbn[1] = cross(output.tbn[2], output.tbn[0]) * input.determinant;
	output.texCoord = input.texCoord;
	output.depthDiv = output.position.zw;
	return output;
}

#endif //GBUFFERS_BUMP_VS_HLSL
