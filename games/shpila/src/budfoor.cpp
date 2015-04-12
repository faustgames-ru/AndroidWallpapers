#include "Headers.h"

BaseGameObject* BudfoorWarrior::constructor()
{
	return new BudfoorWarrior();
}

void BudfoorWarrior::init(GameObjectManager& manager, Node* node, int playerID, Vector3 position)
{
	float scale = 1.5f;
	BaseWarrior::init(manager, node, playerID, position);
	_node->setScale(scale, scale, scale);
	//Node* weapon = _node->findNode("obj_weapon");	
	//weapon->setTranslation(0.0f, 0.0f, 0.0f);
	//weapon->setRotation(0.0f, 0.0f, 0.0f, 1.0f);
	//_node->findNode("Bip001 R Hand")->addChild(weapon);
}