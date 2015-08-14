#include "Headers.h"

BaseStaticActor::BaseStaticActor()
: BaseGameObject()
{}

void BaseStaticActor::damageHandler(BaseGameObject* object)
{
	if (object->LocalGameData.GameData->DistanceGround > 1.5)
	{
		((BaseStaticActor*)object)->rangeFire();
	}
	else
	{
		object->doDamage(object->Target);
	}
}

bool BaseStaticActor::damageEnableHandler(BaseGameObject* object)
{
	return object->LocalGameData.Health > 0.0f && (object->Target != NULL) && (object->Target->LocalGameData.Health > 0.0f) && 
		object->LocalGameData.GameData->isAttackToTargetAllowed(*object->Target->LocalGameData.GameData);
}

void BaseStaticActor::rangeFire()
{
	if (Target != NULL)
	{
		BaseGameObject* bullet = _manager->createObject("bullet", massCenterPosition(), Player);
		bullet->Target = Target;
		bullet->LocalGameData.init(LocalGameData.GameData);
	}
}