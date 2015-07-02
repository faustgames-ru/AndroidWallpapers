#include "Headers.h"

TheBaseObject::TheBaseObject()
: BaseGameObject()
, Target()
{}

BaseGameObject* TheBaseObject::constructor()
{
	return new TheBaseObject();
}

void TheBaseObject::init(GameObjectManager& manager, Node* node, int playerID, Matrix transform)
{
	BaseGameObject::init(manager, node, playerID, transform);
	float scale = COMMON_SCALE;
	_node->setScale(scale, scale, scale);
}

void TheBaseObject::interaction(BaseGameObject* object)
{
	if (Damage > 0.0f)
	{
		float distance = object->position().distanceSquared(position());
		if ((object->PlayerID != PlayerID) && (object->Health > 0.0f) && (distance <= (ActionRadius * ActionRadius)))
		{
			Target = object;
		}
	}
}

void TheBaseObject::update(float time)
{
	_damageTimer += time;
	if (Target && (Damage > 0.0f))
	{
		if (_damageTimer > DamageTime)
		{
			Target->Health -= Damage;
			_damageTimer = 0.0f;
		}
	}
}