#include "Headers.h"

TheBaseObject::TheBaseObject()
: BaseStaticActor()
, Target()
{}

BaseGameObject* TheBaseObject::constructor()
{
	return new TheBaseObject();
}

void TheBaseObject::init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, Matrix transform)
{
	BaseStaticActor::init(manager, gameData, node, player, transform);
	float scale = COMMON_SCALE;
	_node->setScale(scale, scale, scale);
	addTimer(Timer(LocalGameData.GameData->AttackDelayGround, LocalGameData.GameData->AttackDelayGround, damageHandler, damageEnableHandler));
}

void TheBaseObject::interaction(BaseGameObject* object)
{
	float distance = object->position().distanceSquared(position());
	if ((object->Player->ID != Player->ID) && (object->LocalGameData.Health > 0.0f) && (distance <= SQR(LocalGameData.GameData->DistanceGround)))
	{
		Target = object;
	}
}

void TheBaseObject::update(float time)
{
	BaseStaticActor::update(time);
}