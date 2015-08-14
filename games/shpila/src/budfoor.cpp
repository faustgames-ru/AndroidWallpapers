#include "Headers.h"

BaseGameObject* BudfoorWarrior::constructor()
{
	return new BudfoorWarrior();
}

void BudfoorWarrior::init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, Matrix transform)
{
	float scale = BUDFOOR_SCALE;
	BaseWarrior::init(manager, gameData, node, player, transform);
	_node->setScale(scale, scale, scale);
	SearchRadius = 20.0f;
}

void BudfoorWarrior::update(float time)
{
	BaseWarrior::update(time);
	/*Node* hand = _node->findNode("weapon");
	Node* weapon = _node->findNode("obj_weapon");
	weapon->set(hand->getWorldMatrix());*/
}