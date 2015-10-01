#ifndef SHADOW_SUPPORT_HLSLI
#define SHADOW_SUPPORT_HLSLI

#include "LightSupport.hlsli"
#include "TexturesAndSamplers.hlsli"

#define ShadowNormOffset 0.025f

float CalculatePointLightShadow(float3 notNormalizedToLight, float toLightLength )
{
	return 1;
}

// Tests if a given location is in shadow
// shadowMap - the shadow map to test against
// texCoord - The central coordinate in the shadow map representing this location
// depth - The depth of the fragment we are testing whether is in shadow
// (optional) texCoordOffset - An offset, in pixels, to test around texCoord
// returns A value between 0 and 1, where 0 represents a location completely in shadow.  
// Shadow comparisons use the SampleCmpLevelZero method which will blur multiple samples, 
// resulting in softer edges on shadows
float CalculateShadow(Texture2D shadowMap, float2 texCoord, 
					  float depth, int2 texCoordOffset = float2(0, 0))
{
	float shadow = 1;
	return shadow;
}

// Tests if a given location is in shadow, using a 3x3 kernal to average neighboring shadow 
// values resulting in softer smoother shadow penumbra
// shadowMap - the shadow map to test against
// texCoord - The central coordinate in the shadow map representing this location
// depth - The depth of the fragment we are testing whether is in shadow
float CalculateShadowPCF(Texture2D shadowMap, float2 texCoord, float depth)
{
	//return CalculateShadow(shadowMap, texCoord, depth);
	return 0;
}

#endif //SHADOW_SUPPORT_HLSLI
