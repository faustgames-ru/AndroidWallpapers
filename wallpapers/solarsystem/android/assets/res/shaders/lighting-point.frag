vec3 getLightDirection()
{
	return normalize(v_vertexToPointLightDirection);
}

#if defined(BUMPED)

vec3 getLitPixel()
{
	vec3 vertexToPointLightDirection = getLightDirection();
    // Fetch normals from the normal map and normalize the vectors
    vec3 normalVector = normalize(texture2D(u_normalmapTexture, v_texCoord).rgb * 2.0 - 1.0);
    
    
	float pointLightAttenuation = v_pointLightAttenuation;
    
    #if defined(SPECULAR)    
		vec3 cameraDirection = v_cameraDirection;
		return computeLighting(normalVector, vertexToPointLightDirection, pointLightAttenuation, cameraDirection);    
    #else    
		return computeLighting(normalVector, vertexToPointLightDirection, pointLightAttenuation);    
    #endif
}

#else

vec3 getLitPixel()
{
    // Normalize the vectors.
    vec3 normalVector = normalize(v_normalVector);    
    vec3 vertexToPointLightDirection = getLightDirection();
    
    // Fetch point light attenuation.
    float pointLightAttenuation = v_pointLightAttenuation;
    
    #if defined (SPECULAR)    
		vec3 cameraDirection = v_cameraDirection;
		return computeLighting(normalVector, vertexToPointLightDirection, pointLightAttenuation, cameraDirection);    
    #else    
		return computeLighting(normalVector, vertexToPointLightDirection, pointLightAttenuation);    
    #endif
}

#endif