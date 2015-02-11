#include "algorithms\Utils.h"

float rnd(float min, float max)
{
	if (fabs(max - min) > MATH_EPSILON)
		return (float)(rand()) / RAND_MAX*(max - min) + min;
	else
		return 0.5f * (max + min);
}

float rndSign()
{
	return (rnd() >= 0.5f) ? 1.0f : -1.0f;
}