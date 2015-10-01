#ifndef DEFERRED_DIR_LIGHT_HLSL
#define DEFERRED_DIR_LIGHT_HLSL

#include "VertexLayouts.hlsli"
#include "TexturesAndSamplers.hlsli"
#include "LightSupport.hlsli"
#include "ShadowSupport.hlsli"
#include "../EDRendererD3D/ConstantBuffers.h"
#include "../EDRendererD3D/LightBuffers.h"

float4 main(TwoDVertexWithPositionOut inVert) : SV_TARGET0
{
	//return 0;
	float4 specular, posWorld, lightSpacePos, diffuse,
	finalAmbient, finalDiffuse, finalSpecular;
	float3 normal, reflectionVector, directionToCamera;
	float2 texCoord;
	float depth, lightMapDepth, shadow, offset, nDotL, specMod;


	return 0;
}
#endif //DEFERRED_DIR_LIGHT_HLSL
