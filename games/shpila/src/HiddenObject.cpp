
#include "Headers.h"

HiddenObject::HiddenObject()
: BaseGameObject()
, Position()
, Target()
{}

BaseGameObject* HiddenObject::constructor()
{
	return new HiddenObject();
}

const Vector3 HiddenObject::position()
{
	return Position;
}

void HiddenObject::init(GameObjectManager& manager, Node* node, int playerID, Matrix transform)
{
	_manager = &manager;
	PlayerID = playerID;
	manager.registerObject(this);
}

void HiddenObject::interaction(BaseGameObject* object)
{
	if (GameData->getDamage(*object->GameData) > 0.0f)
	{
		float distance = object->position().distanceSquared(position());
		if ((object->PlayerID != PlayerID) && (object->Health > 0.0f) && (distance <= (GameData->DistanceGround * GameData->DistanceGround)))
		{
			Target = object;
		}
	}
}

void HiddenObject::update(float time)
{
	_damageTimer += time;
	float damage = GameData->getDamage(*Target->GameData);
	if (Target && (damage > 0.0f))
	{
		if (_damageTimer > GameData->AttackDelayGround)
		{
			Target->Health -= damage;
			_damageTimer = 0.0f;
		}
	}
}