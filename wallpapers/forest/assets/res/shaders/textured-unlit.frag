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
varying vec3 v_cameraDirection;                 // Camera direction
varying vec3 v_normalViewVector;                    // Normal vector in view space
#endif

// Varyings
varying vec2 v_texCoord0;                	// Texture coordinate(u, v)
#if defined(TEXCOORD1)
varying vec2 v_texCoord1;                   // Second tex coord for multi-texturing
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
		vec3 cameraDirection = normalize(v_cameraDirection);
		vec3 normalVector = normalize(v_normalViewVector);
		float ddot = abs(dot(normalVector, cameraDirection));
		gl_FragColor.a *= min(ddot * ddot * ddot * ddot, 1.0);
    #endif
}
