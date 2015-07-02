#include "Headers.h"

BaseGameObject* BararWarrior::constructor()
{
	return new BararWarrior();
}

void BararWarrior::init(GameObjectManager& manager, Node* node, int playerID, Matrix transform)
{
	float scale = BARAR_SCALE;
	BaseWarrior::init(manager, node, playerID, transform);
	_node->setScale(scale, scale, scale);
	SearchRadius = 20.0f;
	ActionRadius = 1.5f;
	GeometryRadius = 0.5f;
	Damage = 10;
	DamageTime = 1440.0f * TIME_SCALE;
	Health = 120;
	Price = 275;
}