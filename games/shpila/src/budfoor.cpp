#include "Headers.h"

BaseGameObject* BudfoorWarrior::constructor()
{
	return new BudfoorWarrior();
}

void BudfoorWarrior::init(GameObjectManager& manager, Node* node, int playerID, Vector3 position)
{
	float scale = 0.01f;
	BaseWarrior::init(manager, node, playerID, position);
	_node->setScale(scale, scale, scale);
}

void BudfoorWarrior::update(float time)
{
	BaseWarrior::update(time);
	Node* hand = _node->findNode("Bip001 R Hand");
	Node* weapon = _node->findNode("obj_weapon");
	weapon->set(hand->getWorldMatrix());
}