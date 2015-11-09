#ifndef GBUFFERS_BUMP_PS_HLSL
#define GBUFFERS_BUMP_PS_HLSL

#include "VertexLayouts.hlsli"
#include "TexturesAndSamplers.hlsli"
#include "../EDRendererD3D/ConstantBuffers.h"
#include "GBuffersShared.hlsli"

GBufferFragOut main(GBufferBumpVertexOut input) {
	GBufferFragOut output = (GBufferFragOut)0;
	output.diffuse = diffuseMap.Sample(linearClampSampler, input.texCoord);

	output.specular = specularMap.Sample(linearClampSampler, input.texCoord);

	float3 normalT = 2.0f*normalMap.Sample(linearClampSampler, input.texCoord).rgb - 1.0f;
	//input.tbn[0] = normalize(input.tbn[0]);
	//input.tbn[1] = normalize(input.tbn[1]);
	//input.tbn[2] = normalize(input.tbn[2]);
	output.normal = (float4(normalize(mul(normalT, input.tbn)), 1.0f) + 1) / 2;
	//output.normal = (float4(input.normalW, 1.0f) + 1) / 2;

	output.depth = input.depthDiv.x / input.depthDiv.y;

	if ( output.diffuse.w < MIN_ALPHA_KEEP ) {
		discard;
	}

	output.diffuse.a = ambientMap.Sample(linearClampSampler, input.texCoord).r;
	return output;
}

#endif //GBUFFERS_BUMP_PS_HLSL
