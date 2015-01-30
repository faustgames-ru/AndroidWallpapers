vec3 getLightDirection()
{
	#if defined(BUMPED)
		return normalize(v_lightDirection);
	#else
		return normalize(u_lightDirection);
	#endif
}

#if defined(BUMPED)
	vec3 getLitPixel()
	{
		// Fetch normals from the normal map
		vec3 normalVector = normalize(texture2D(u_normalmapTexture, v_texCoord).rgb * 2.0 - 1.0);
		vec3 lightDirection = getLightDirection();
		
		#if defined(SPECULAR)    
			vec3 cameraDirection = v_cameraDirection;
			return computeLighting(normalVector, -lightDirection, 1.0, cameraDirection);    
		#else    
			return computeLighting(normalVector, -lightDirection, 1.0);    
		#endif
	}
#else
	vec3 getLitPixel()
	{
		// Normalize the vectors.
		vec3 normalVector = normalize(v_normalVector);
		vec3 lightDirection = getLightDirection();

		#if defined(SPECULAR)    
			vec3 cameraDirection = v_cameraDirection;
			return computeLighting(normalVector, -lightDirection, 1.0, cameraDirection);    
		#else    
			return computeLighting(normalVector, -lightDirection, 1.0);    
		#endif
	}
#endif