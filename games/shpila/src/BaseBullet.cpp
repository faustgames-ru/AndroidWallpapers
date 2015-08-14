#include "Headers.h"

BaseBullet::BaseBullet()
: BaseGameObject()
, _deleted(false)
{}

BaseBullet::~BaseBullet()
{}

BaseGameObject* BaseBullet::constructor()
{
	return new BaseBullet();
}

void BaseBullet::init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, Matrix transform)
{
	float scale = COMMON_SCALE;
	BaseGameObject::init(manager, gameData, node, player, transform);
	_node->setScale(scale, scale, scale);
}

void BaseBullet::update(float time)
{
	if (!Target)
	{
		_deleted = true;
		return;
	}
	
	float dist = Target->massCenterPosition().distance(position());
	if (dist > (0.5f * Target->LocalGameData.GameData->GeometryRadius))
	{
		Vector3 dir = (Target->massCenterPosition() - position()) / dist;
		setPosition(position() + dir * min(dist, time * 0.01f));
	}
	else
	{
		_deleted = true;
		doDamage(Target);
	}
		
}

bool BaseBullet::deleted()
{
	return _deleted;
}