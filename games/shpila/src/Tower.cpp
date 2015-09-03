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
	BaseStaticActor::init(manager, gameData, node, player, transform);
	float scale = COMMON_SCALE;
	_node->setScale(scale, scale, scale);
}

void TowerObject::interaction(BaseGameObject* object)
{
	targeting(object);
}

void TowerObject::update(float time)
{
	BaseStaticActor::update(time);
}