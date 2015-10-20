#include "Headers.h"


BaseGameObject* BoomerangBullet::constructor()
{
	return new BoomerangBullet();
}

void BoomerangBullet::update(float time)
{
	BaseBullet::update(time);
	_node->rotateY(time * 0.02f);
}