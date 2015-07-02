#include "Headers.h"

BaseGameObject* IrbagaWarrior::constructor()
{
	return new IrbagaWarrior();
}

void IrbagaWarrior::init(GameObjectManager& manager, Node* node, int playerID, Matrix transform)
{
	float scale = IRBAGA_SCALE;
	BaseWarrior::init(manager, node, playerID, transform);
	_node->setScale(scale, scale, scale);
	SearchRadius = 20.0f;
	ActionRadius = 1.5f;
	GeometryRadius = 0.5f;
	Damage = 16;
	DamageTime = 1200.0f * TIME_SCALE;
	Health = 150;
	Price = 90;
}