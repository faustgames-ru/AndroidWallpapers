//#define LIGHTING
#ifdef OPENGL_ES
precision highp float;
#endif

// Uniforms
uniform vec4 u_diffuseColor;               		// Diffuse color
uniform vec3 u_ambientColor;                    // Ambient color
uniform vec3 u_lightColor;                      // Light color
uniform vec3 u_lightDirection;					// Light direction
#if defined(SPECULAR)
uniform float u_specularExponent;				// Specular exponent
#endif
#if defined(SPOT_LIGHT)
uniform float u_spotLightInnerAngleCos;			// The bright spot [0.0 - 1.0]
uniform float u_spotLightOuterAngleCos;			// The soft outer part [0.0 - 1.0]
uniform vec3 u_spotLightDirection;              // Direction of a spot light source
#endif
#if defined(MODULATE_COLOR)
uniform vec4 u_modulateColor;					// Modulation color
#endif
#if defined(MODULATE_ALPHA)
uniform float u_modulateAlpha;					// Modulation alpha
#endif

// Inputs
varying vec3 v_normalVector;					// Normal vector in view space
#if defined(VERTEX_COLOR)
varying vec3 v_color;							// Vertex color
#endif
#if defined(POINT_LIGHT)
varying vec3 v_vertexToPointLightDirection;		// Light direction w.r.t current vertex in tangent space
varying float v_pointLightAttenuation;			// Attenuation of point light
#elif defined(SPOT_LIGHT)
varying vec3 v_spotLightDirection;				// Direction of spot light in tangent space.
varying vec3 v_vertexToSpotLightDirection;		// Direction of the spot light w.r.t current vertex in tangent space
varying float v_spotLightAttenuation;			// Attenuation of spot light
#else
varying vec3 v_lightDirection;					// Direction of light in tangent space
#endif
#if defined(SPECULAR) || defined(SOFT_TRANSPARENT_EDGES)
varying vec3 v_cameraDirection;                 // Camera direction
varying vec3 v_normalViewVector;				// Normal vector in view space
#endif

// Soft transparent edges
#if defined(SOFT_TRANSPARENT_EDGES)
	#include "SoftTransparentEdges.frag"
#endif

// Lighting
#if defined(LIGHTING) || defined(SOFT_TRANSPARENT_EDGES_WITH_LIGHT)
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
    // Set base diffuse color
	#if defined(LIGHTING)
		#if defined(VERTEX_COLOR)
			_baseColor.rgb = v_color;
		#else
			_baseColor = u_diffuseColor;
		#endif
		gl_FragColor.a = _baseColor.a;	
		// Light the pixel
		gl_FragColor.rgb = getLitPixel();
	#else
		#if defined(VERTEX_COLOR)
			gl_FragColor.rgb = v_color;
			gl_FragColor.a = 1.0;
		#else
			gl_FragColor = u_diffuseColor;
		#endif
	#endif

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
