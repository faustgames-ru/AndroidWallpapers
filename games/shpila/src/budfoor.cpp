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
	SearchRadius = 10.0f;
	ActionRadius = 2.5f;
	GeometryRadius = 0.5f;
	EnemyNexus = EnemyNexus;
	Damage = 35;
	DamageTime = 1000.0f;
}

void BudfoorWarrior::update(float time)
{
	BaseWarrior::update(time);
	Node* hand = _node->findNode("weapon");
	Node* weapon = _node->findNode("obj_weapon");
	weapon->set(hand->getWorldMatrix());
}