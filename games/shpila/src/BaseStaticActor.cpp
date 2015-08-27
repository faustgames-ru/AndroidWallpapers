#include "Headers.h"

BaseStaticActor::BaseStaticActor()
: BaseGameObject()
{}

void BaseStaticActor::damageHandler(BaseGameObject* object)
{
	if (!object->Target)
		return;

	int attacksCount = object->LocalGameData.GameData->getAttacksCount(*object->Target->LocalGameData.GameData);
	for (int i = 0; i < attacksCount; i++)
	{
		if (object->LocalGameData.GameData->ImmediateAttack)
		{
			object->doDamage(object->Target);
		}
		else
		{
			((BaseStaticActor*)object)->rangeFire();
		}
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
		BaseGameObject* bullet = _manager->createObject("bullet", massCenterPosition(), Target->massCenterPosition() - massCenterPosition(), Player);
		bullet->Target = Target;
		bullet->LocalGameData.init(LocalGameData.GameData);
	}
}