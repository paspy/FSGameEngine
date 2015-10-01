#ifndef DEFERRED_POINT_LIGHT_HLSL
#define DEFERRED_POINT_LIGHT_HLSL

#include "VertexLayouts.hlsli"
#include "TexturesAndSamplers.hlsli"
#include "LightSupport.hlsli"
#include "../EDRendererD3D/ConstantBuffers.h"
#include "../EDRendererD3D/LightBuffers.h"

float4 main(VertexWithPositionOut inVert) : SV_TARGET0 
{
	//return float4(1, 1, 1, 1);
	float4 diffuse, specular, posWorld, 
		finalAmbient, finalDiffuse, finalSpecular;
	float3 toLight, normal;
	float2 texCoord;
	float toLightLength, attenuation, nDotL, specMod, depth;

	return 0;
}
#endif //DEFERRED_POINT_LIGHT_HLSL
