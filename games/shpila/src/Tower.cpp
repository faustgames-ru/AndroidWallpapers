#include "Headers.h"

TowerObject::TowerObject()
: BaseGameObject()
, Target()
{}

BaseGameObject* TowerObject::constructor()
{
	return new TowerObject();
}

void TowerObject::init(GameObjectManager& manager, Node* node, int playerID, Matrix transform)
{
	BaseGameObject::init(manager, node, playerID, transform);
	float scale = COMMON_SCALE;
	_node->setScale(scale, scale, scale);
}

void TowerObject::interaction(BaseGameObject* object)
{
	if (GameData->DamageLight > 0.0f)
	{
		float distance = object->position().distanceSquared(position());
		if ((object->PlayerID != PlayerID) && (object->Health > 0.0f) && (distance <= (GameData->DistanceGround * GameData->DistanceGround)))
		{
			Target = object;
		}
	}
}

void TowerObject::update(float time)
{
	_damageTimer += time;
	if (Target && (GameData->DamageLight > 0.0f))
	{
		if (_damageTimer > GameData->AttackDelayGround)
		{
			Target->Health -= GameData->DamageLight;
			_damageTimer = 0.0f;
		}
	}
}