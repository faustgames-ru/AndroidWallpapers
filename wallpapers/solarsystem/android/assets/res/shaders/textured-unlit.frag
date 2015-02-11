#ifdef OPENGL_ES
precision highp float;
#endif

// Uniforms
uniform sampler2D u_diffuseTexture;     	// Diffuse texture
#if defined(TEXTURE_LIGHTMAP)
uniform sampler2D u_lightmapTexture;     	// Lightmap texture
#endif
#if defined(MODULATE_COLOR)
uniform vec4 u_modulateColor;               // Modulation color
#endif
#if defined(MODULATE_ALPHA)
uniform float u_modulateAlpha;              // Modulation alpha
#endif
#if defined(SOFT_TRANSPARENT_EDGES)
	uniform vec3 u_ambientColor;                    // Ambient color
	uniform vec3 u_lightColor;                      // Light color
	uniform vec3 u_lightDirection;					// Light direction
	varying vec3 v_cameraDirection;                 // Camera direction
	varying vec3 v_normalViewVector;                // Normal vector in view space
#endif

// Varyings
varying vec2 v_texCoord0;                	// Texture coordinate(u, v)
#if defined(POINT_LIGHT)
varying vec3 v_vertexToPointLightDirection;		// Light direction w.r.t current vertex in tangent space.
varying float v_pointLightAttenuation;			// Attenuation of point light.
#elif defined(SPOT_LIGHT)
varying vec3 v_spotLightDirection;				// Direction of spot light in tangent space.
varying vec3 v_vertexToSpotLightDirection;		// Direction of the spot light w.r.t current vertex in tangent space.
varying float v_spotLightAttenuation;			// Attenuation of spot light.
#else
varying vec3 v_lightDirection;					// Direction of light in tangent space.
#endif
#if defined(TEXCOORD1)
	varying vec2 v_texCoord1;                   // Second tex coord for multi-texturing
#endif
#if defined(SOFT_TRANSPARENT_EDGES)
	varying vec3 v_normalVector;					// Normal vector in view space
#endif

// Soft transparent edges
#if defined(SOFT_TRANSPARENT_EDGES)
	#include "SoftTransparentEdges.frag"
	#include "lighting.frag"
	#if defined(POINT_LIGHT)
	#include "lighting-point.frag"
	#elif defined(SPOT_LIGHT)
	#include "lighting-spot.frag"
	#else
	#include "lighting-directional.frag"
	#endif
#endif

void main()
{
    // Sample the texture for the color
    gl_FragColor = texture2D(u_diffuseTexture, v_texCoord0);
    #if defined(TEXTURE_DISCARD_ALPHA)
    if (gl_FragColor.a < 0.5)
        discard;
    #endif
    #if defined(TEXTURE_LIGHTMAP)
    #if defined(TEXCOORD1)
    vec4 lightColor = texture2D(u_lightmapTexture, v_texCoord1);
    #else
    vec4 lightColor = texture2D(u_lightmapTexture, v_texCoord0);
    #endif
    gl_FragColor.rgb *= lightColor.rgb;
    #endif
    // Global color modulation
    #if defined(MODULATE_COLOR)
    gl_FragColor *= u_modulateColor;
    #endif
    #if defined(MODULATE_ALPHA)
    gl_FragColor.a *= u_modulateAlpha;
    #endif
	#if defined(SOFT_TRANSPARENT_EDGES)
		gl_FragColor.a *= GetSoftEdgeTransparency(getLightDirection());
    #endif
}
