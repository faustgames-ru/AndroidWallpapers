#include "StarsParticles.h"
#include "algorithms\Utils.h"

void StarsParticles::GenerateParticlePosition(Vector3* dst)
{
	float r = _positionVar.length();
	float t = rnd(0.0f, MATH_PI);
	float f = rnd(0.0f, 2.0f * MATH_PI);
	dst->x = _position.x + r * sin(t) * cos(f);
	dst->y = _position.y + r * sin(t) * sin(f);
	dst->z = _position.z + r * cos(t);
}

ParticleEmitter* StarsParticles::Constructor(int maxParticlesCount)
{
	return new StarsParticles(maxParticlesCount);
}

StarsParticles::StarsParticles(unsigned int particlesCount)
: ParticleEmitter(particlesCount)
{

}