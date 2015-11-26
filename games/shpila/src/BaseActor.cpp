#include "Headers.h"

BaseActor::BaseActor()
: BaseStaticActor()
, _movementController()
{}

BaseActor::~BaseActor(){}

void BaseActor::init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, const Matrix & transform)
{
	BaseStaticActor::init(manager, gameData, node, player, transform);
	manager.registerMovementController(&_movementController);
	Vector3 pos;
	transform.getTranslation(&pos);
	_movementController.setRadius(LocalGameData.GameData->GeometryRadius);
	_movementController.setPosition(pos.x, pos.y, pos.z);
	if (_node)
	{
		auto dir = _node->getForwardVectorWorld();
		_movementController.setForward(dir.x, dir.y, dir.z);
	}
	
}

void BaseActor::setPosition(const Vector3 pos)
{
	BaseGameObject::setPosition(pos);
	_movementController.setPosition(OpenSteer::Vec3(pos.x, pos.y, pos.z));
}