#include "Headers.h"

BaseBullet::BaseBullet()
: BaseGameObject()
, Target()
, _deleted(false)
{}

BaseBullet::~BaseBullet()
{}

void BaseBullet::update(float time)
{
	float dist = Target->position().distance(position());
	if (dist > (0.5f * Target->GameData->GeometryRadius))
	{
		Vector3 dir = (Target->position() - position()) / dist;
		setPosition(position() + dir * min(dist, time * 0.01f));
	}
	else
		_deleted = true;
}

bool BaseBullet::deleted()
{
	return _deleted;
}