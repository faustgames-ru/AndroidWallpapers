
#include "HiddenObject.h"

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

void HiddenObject::interaction(BaseGameObject* object)
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

void HiddenObject::update(float time)
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