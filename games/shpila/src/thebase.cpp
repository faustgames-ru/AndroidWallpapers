#include "Headers.h"

TheBaseObject::TheBaseObject()
: BaseStaticActor()
, Target()
{}

BaseGameObject* TheBaseObject::constructor()
{
	return new TheBaseObject();
}

void TheBaseObject::init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, const Matrix transform)
{
	BaseStaticActor::init(manager, gameData, node, player, transform);
	float scale = COMMON_SCALE;
	_node->setScale(scale, scale, scale);
}

void TheBaseObject::interaction(BaseGameObject* object)
{
	targeting(object);
}

void TheBaseObject::update(float time)
{
	BaseStaticActor::update(time);
}