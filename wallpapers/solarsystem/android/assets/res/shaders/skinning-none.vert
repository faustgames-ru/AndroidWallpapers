vec4 getPosition()
{
    return a_position;    
}

#if defined(LIGHTING) || defined(SOFT_TRANSPARENT_EDGES)

vec3 getNormal()
{
    return a_normal;
}

#if defined(BUMPED)

vec3 getTangent()
{
    return a_tangent;
}

vec3 getBinormal()
{
    return a_binormal;
}

#endif

#endif