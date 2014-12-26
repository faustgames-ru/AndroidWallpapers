// Attributes
attribute vec4 a_position;									// Vertex Position							(x, y, z, w)
#if defined(SOFT_TRANSPARENT_EDGES)
attribute vec3 a_normal;									// Vertex normal							(x, y, z)
#endif
attribute vec2 a_texCoord0;									// Vertex Texture Coordinate				(u, v)
#if defined(TEXCOORD1)
attribute vec2 a_texCoord1;                                 // Second tex coord for multi-texturing
#endif
#if defined(SKINNING)
attribute vec4 a_blendWeights;								// Vertex blend weight, up to 4				(0, 1, 2, 3) 
attribute vec4 a_blendIndices;								// Vertex blend index int u_matrixPalette	(0, 1, 2, 3)
#endif

// Uniforms
uniform mat4 u_worldViewProjectionMatrix;					// Matrix to transform a position to clip space
#if defined(SOFT_TRANSPARENT_EDGES)
uniform mat4 u_worldViewMatrix;								// Matrix to tranform a position to view space
uniform mat4 u_inverseTransposeWorldViewMatrix;				// Matrix to transform a normal to view space
#endif
#if defined(SKINNING)
uniform vec4 u_matrixPalette[SKINNING_JOINT_COUNT * 3];		// Array of 4x3 matrices
#endif
#if defined(SOFT_TRANSPARENT_EDGES)
uniform vec3 u_cameraPosition;                 				// Position of the camera in view space
#endif
#if defined(TEXTURE_REPEAT)
uniform vec2 u_textureRepeat;								// Texture repeat for tiling
#endif
#if defined(TEXTURE_OFFSET)
uniform vec2 u_textureOffset;								// Texture offset
#endif

// Varyings
varying vec2 v_texCoord0;									// Texture Coordinate
#if defined(TEXCOORD1)
varying vec2 v_texCoord1;                                   // Second tex coord for multi-texturing
#endif
#if defined(SOFT_TRANSPARENT_EDGES)
varying vec3 v_cameraDirection;								// Direction the camera is looking at in tangent space
varying vec3 v_normalVector;								// Normal vector in view space
#endif

// Skinning 
#if defined(SKINNING)
#include "skinning.vert"
#else
#include "skinning-none.vert" 
#endif


void main()
{
    // Get the vertex position
    vec4 position = getPosition();

    // Transform position to clip space.
    gl_Position = u_worldViewProjectionMatrix * position;

    // Texture transformation.
    v_texCoord0 = a_texCoord0;
    #if defined(TEXCOORD1)
    v_texCoord1 = a_texCoord1;
    #endif
    #if defined(TEXTURE_REPEAT)
    v_texCoord0 *= u_textureRepeat;
    #endif
    #if defined(TEXTURE_OFFSET)
    v_texCoord0 += u_textureOffset;
    #endif
	#if defined(SOFT_TRANSPARENT_EDGES)
	vec4 positionWorldViewSpace = u_worldViewMatrix * position;
	v_cameraDirection = u_cameraPosition - positionWorldViewSpace.xyz;
	// Transform normal to view space.
	mat3 normalMatrix = mat3(u_inverseTransposeWorldViewMatrix[0].xyz, u_inverseTransposeWorldViewMatrix[1].xyz, u_inverseTransposeWorldViewMatrix[2].xyz);
	vec3 normal = getNormal();
    v_normalVector = normalMatrix * normal;
	#endif
}
