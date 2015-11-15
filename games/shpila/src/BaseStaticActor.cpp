#include "Headers.h"

BaseStaticActor::BaseStaticActor()
: BaseGameObject()
, _damageTimer()
, _fogOfWarTimer()
{}

void BaseStaticActor::init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, const Matrix transform)
{
	BaseGameObject::init(manager, gameData, node, player, transform);
	_damageTimer.start(LocalGameData.GameData->AttackDelayGround, LocalGameData.GameData->AttackDelayGround);
}

void BaseStaticActor::update(float time)
{
	BaseGameObject::update(time);
	if (_damageTimer.enabled() && _damageTimer.action(time))
		doDamage(Target);
}

bool BaseStaticActor::getDetected()
{
	return !_fogOfWarTimer.stoped();
}
void BaseStaticActor::setDetected(bool value)
{
	if (value)
		_fogOfWarTimer.start(0.0f, 2.0f, 1.0f, false, true);
	else
		_fogOfWarTimer.enable(false);
}

void BaseStaticActor::attackEnable(bool value)
{
	_damageTimer.enable(value);
}

void BaseStaticActor::rangeFire()
{
	if (Target != NULL)
	{
		BaseGameObject* bullet = _manager->createObject("bullet", massCenterPosition(), Target->massCenterPosition() - massCenterPosition(), Player);
		bullet->Target = Target;
		bullet->LocalGameData.init(LocalGameData.GameData, LocalGameData.Upgrades);
	}
}

void BaseStaticActor::targeting(BaseGameObject* object)
{
	float distanceToTarget = isAttackToTargetAllowed(Target) ? Target->position().distanceSquared(position()) : FLT_MAX;
	float distanceToObject = object->position().distanceSquared(position());
	float attackDistance = getAttackDistance(object);
	float targetingDistance = object->getTargetingDistance(object);
	bool detected = distanceToObject < SQR(targetingDistance);
	if (detected && (distanceToTarget > distanceToObject) && (distanceToTarget > SQR(attackDistance)) && !friendly(object))
	{
		if (isAttackToTargetAllowed(object))
		{
			Target = object;
			Target->setDetected(true);
		}

	}
}