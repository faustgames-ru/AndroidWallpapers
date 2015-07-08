#include "Headers.h"

BaseGameObject* BudfoorWarrior::constructor()
{
	return new BudfoorWarrior();
}

void BudfoorWarrior::init(GameObjectManager& manager, Node* node, int playerID, Matrix transform)
{
	float scale = BUDFOOR_SCALE;
	BaseWarrior::init(manager, node, playerID, transform);
	_node->setScale(scale, scale, scale);
	SearchRadius = 20.0f;
	GeometryRadius = 2.5f;
}

void BudfoorWarrior::update(float time)
{
	BaseWarrior::update(time);
	/*Node* hand = _node->findNode("weapon");
	Node* weapon = _node->findNode("obj_weapon");
	weapon->set(hand->getWorldMatrix());*/
}