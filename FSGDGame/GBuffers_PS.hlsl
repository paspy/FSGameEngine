#ifndef GBUFFERS_PS_HLSL
#define GBUFFERS_PS_HLSL

#include "VertexLayouts.hlsli"
#include "TexturesAndSamplers.hlsli"
#include "GBuffersShared.hlsli"

GBufferFragOut main(GBufferVertexOut input) {
	GBufferFragOut output = (GBufferFragOut)0;

	output.diffuse = diffuseMap.Sample(linearClampSampler, input.texCoord);

	output.specular = specularMap.Sample(linearClampSampler, input.texCoord);
	output.normal = (float4(input.normalW, 1.0f) + 1) / 2;
	output.depth = input.depthDiv.x / input.depthDiv.y;
	
	if ( output.diffuse.w < MIN_ALPHA_KEEP ) {
		discard;
	}

	output.diffuse.a = ambientMap.Sample(linearClampSampler, input.texCoord).r;
	return output;
}

#endif //GBUFFERS_PS_HLSL
