
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
	if (GameData->Damage > 0.0f)
	{
		float distance = object->position().distanceSquared(position());
		if ((object->PlayerID != PlayerID) && (object->Health > 0.0f) && (distance <= (GameData->AttackDistance * GameData->AttackDistance)))
		{
			Target = object;
		}
	}
}

void HiddenObject::update(float time)
{
	_damageTimer += time;
	if (Target && (GameData->Damage > 0.0f))
	{
		if (_damageTimer > GameData->AttackDelay)
		{
			Target->Health -= GameData->Damage;
			_damageTimer = 0.0f;
		}
	}
}