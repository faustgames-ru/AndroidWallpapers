#ifndef STARSPARTICLES_H_
#define STARSPARTICLES_H_

#include "gameplay.h"

using namespace gameplay;

class StarsParticles : public ParticleEmitter
{
	public:
		virtual void GenerateParticlePosition(Vector3* dst);
		static ParticleEmitter* Constructor(int maxParticlesCount);
	protected:
		StarsParticles(unsigned int particlesCount);
};

#endif