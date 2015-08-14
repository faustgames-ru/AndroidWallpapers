#include "Headers.h"

TowerObject::TowerObject()
: BaseStaticActor()
{}

BaseGameObject* TowerObject::constructor()
{
	return new TowerObject();
}

void TowerObject::init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, Matrix transform)
{
	BaseGameObject::init(manager, gameData, node, player, transform);
	float scale = COMMON_SCALE;
	_node->setScale(scale, scale, scale);
	addTimer(Timer(LocalGameData.GameData->AttackDelayGround, LocalGameData.GameData->AttackDelayGround, damageHandler, damageEnableHandler));
}

void TowerObject::interaction(BaseGameObject* object)
{
	float distance = object->position().distanceSquared(position());
	if ((object->Player->ID != Player->ID) && (object->LocalGameData.Health > 0.0f) && (distance <= SQR(LocalGameData.GameData->DistanceGround)))
	{
		Target = object;
	}
}

void TowerObject::update(float time)
{
	BaseGameObject::update(time);
}