float GetSoftEdgeTransparency(vec3 lightDirection)
{
	float transparency = 1.0;
	vec3 cameraDirection = normalize(v_cameraDirection);
	vec3 normalVector = normalize(v_normalViewVector);
	#if defined(SOFT_TRANSPARENT_EDGES_FILL)
		float ddot = abs(dot(normalVector, cameraDirection));
		transparency *= min(ddot * ddot * ddot * ddot, 1.0);
	#else
		float ddot = 1.0 - (abs(dot(normalVector, cameraDirection) - 0.45) * 2.0);
		transparency *= min(ddot * ddot * ddot * ddot * ddot, 1.0);
	#endif
	#if defined(SOFT_TRANSPARENT_EDGES_WITH_LIGHT)
		transparency *= clamp((dot(normalize(v_normalVector), lightDirection) + 1.0) * 0.5, 0.2, 1.0);
	#endif
	return transparency;
}