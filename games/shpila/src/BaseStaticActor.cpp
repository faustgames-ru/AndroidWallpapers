#include "Headers.h"

BaseStaticActor::BaseStaticActor()
: BaseGameObject()
, _damageTimer()
{}

void BaseStaticActor::init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, Matrix transform)
{
	BaseGameObject::init(manager, gameData, node, player, transform);
	_damageTimer.start(LocalGameData.GameData->AttackDelayGround, LocalGameData.GameData->AttackDelayGround);
}

void BaseStaticActor::update(float time)
{
	BaseGameObject::update(time);
	if (_damageTimer.action(time))
	{
		doDamage(Target);
	}
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