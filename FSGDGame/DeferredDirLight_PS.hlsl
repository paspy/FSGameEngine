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

	specular = specularGBuffer.Sample(linearClampSampler, inVert.texCoord);
	depth = depthGBuffer.Sample(linearClampSampler, inVert.texCoord).x;
	posWorld = CalculateWorldSpacePosition(inVert.pixelPosition.xy, depth, gInvViewProj);
	lightSpacePos;
	diffuse = diffuseGBuffer.Sample(linearClampSampler, inVert.texCoord);
	normal = 2.0f * normalGBuffer.Sample(linearClampSampler, inVert.texCoord).rgb - 1.0f;

	reflectionVector = reflect(DirLight.direction, normal);
	directionToCamera = normalize(gCameraPos - posWorld.xyz);
	specMod = DirLight.specularIntensity * pow(saturate(dot(reflectionVector, directionToCamera)), DirLight.specularPower);
	finalSpecular = specular * specMod * float4(DirLight.specularColor, 1.0f);

	nDotL = saturate(dot(normal, -DirLight.direction));
	finalDiffuse = nDotL * (diffuse * float4(DirLight.diffuseColor, 1.0f));

	finalAmbient = float4((diffuse.rgb * DirLight.ambientColor), 1.0f) * diffuse.a;

	return finalDiffuse + finalAmbient + finalSpecular;

}
#endif //DEFERRED_DIR_LIGHT_HLSL
