#if defined(BUMPED)

void applyLight(mat3 tangentSpaceTransformMatrix, vec4 position)
{
    // Transform light direction to tangent space
    v_lightDirection = tangentSpaceTransformMatrix * u_lightDirection;
    
    #if defined(SPECULAR) || defined(SOFT_TRANSPARENT_EDGES)

    // Compute the camera direction for specular lighting
	vec4 positionWorldViewSpace = u_worldViewMatrix * position;
    v_cameraDirection = u_cameraPosition - positionWorldViewSpace.xyz;

    #endif
}

#else

void applyLight(vec4 position)
{
    
    #if defined(SPECULAR) || defined(SOFT_TRANSPARENT_EDGES)

    // Compute the camera direction for specular lighting
	vec4 positionWorldViewSpace = u_worldViewMatrix * position;
    v_cameraDirection = u_cameraPosition - positionWorldViewSpace.xyz;

    #endif
}

#endif
