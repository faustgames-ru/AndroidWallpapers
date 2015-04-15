#include "Headers.h"

BaseGameObject* BararWarrior::constructor()
{
	return new BararWarrior();
}

void BararWarrior::init(GameObjectManager& manager, Node* node, int playerID, Vector3 position)
{
	//float scale = 0.01f;
	BaseWarrior::init(manager, node, playerID, position);
	//_node->setScale(scale, scale, scale);
}