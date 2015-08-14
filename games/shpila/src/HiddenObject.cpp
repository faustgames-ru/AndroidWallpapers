
#include "Headers.h"

HiddenObject::HiddenObject()
: BaseGameObject()
, Position()
, Target()
{}

BaseGameObject* HiddenObject::constructor()
{
	return new HiddenObject();
}

const Vector3 HiddenObject::position()
{
	return Position;
}

void HiddenObject::init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, Matrix transform)
{
	_manager = &manager;
	Player = player;
	manager.registerObject(this);
}

void HiddenObject::interaction(BaseGameObject* object)
{
		
}

void HiddenObject::update(float time)
{

}