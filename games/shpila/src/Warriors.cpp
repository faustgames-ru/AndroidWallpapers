#include "Headers.h"



BaseGameObject* DarkWarrior::constructor()
{
	return new DarkWarrior();
}
DarkWarrior::DarkWarrior()
: BaseWarrior()
{}

void DarkWarrior::init(GameObjectManager& manager, Node* node, int playerID, Matrix transform)
{
	float scale = COMMON_SCALE;
	BaseWarrior::init(manager, node, playerID, transform);
	_node->setScale(scale, scale, scale);
	SearchRadius = 20.0f;
}

///////////////////////////////////////////////////////////////////////////////////////////

BaseGameObject* ArchonWarrior::constructor()
{
	return new ArchonWarrior();
}
ArchonWarrior::ArchonWarrior()
: BaseWarrior()
{
}
void ArchonWarrior::init(GameObjectManager& manager, Node* node, int playerID, Matrix transform)
{
	float scale = COMMON_SCALE;
	BaseWarrior::init(manager, node, playerID, transform);
	_node->setScale(scale, scale, scale);
	SearchRadius = 20.0f;
}

///////////////////////////////////////////////////////////////////////////////////////////

BaseGameObject* ObserverWarrior::constructor()
{
	return new ObserverWarrior();
}
ObserverWarrior::ObserverWarrior()
: BaseWarrior()
{
}
void ObserverWarrior::init(GameObjectManager& manager, Node* node, int playerID, Matrix transform)
{
	BaseWarrior::init(manager, node, playerID, transform);
	float scale = COMMON_SCALE;
	_node->setScale(scale, scale, scale);
	SearchRadius = 20.0f;
}

///////////////////////////////////////////////////////////////////////////////////////////

BaseGameObject* ImmortalWarrior::constructor()
{
	return new ImmortalWarrior();
}
ImmortalWarrior::ImmortalWarrior()
: BaseWarrior()
{
}
void ImmortalWarrior::init(GameObjectManager& manager, Node* node, int playerID, Matrix transform)
{
	BaseWarrior::init(manager, node, playerID, transform);
	float scale = COMMON_SCALE;
	_node->setScale(scale, scale, scale);
	SearchRadius = 20.0f;
}

///////////////////////////////////////////////////////////////////////////////////////////

BaseGameObject* ColossusWarrior::constructor()
{
	return new ColossusWarrior();
}
ColossusWarrior::ColossusWarrior()
: BaseWarrior()
{}
void ColossusWarrior::init(GameObjectManager& manager, Node* node, int playerID, Matrix transform)
{
	BaseWarrior::init(manager, node, playerID, transform);
	float scale = COMMON_SCALE;
	_node->setScale(scale, scale, scale);
	SearchRadius = 20.0f;
}

///////////////////////////////////////////////////////////////////////////////////////////

BaseGameObject* VoidRayWarrior::constructor()
{
	return new VoidRayWarrior();
}
VoidRayWarrior::VoidRayWarrior()
: BaseWarrior()
{}
void VoidRayWarrior::init(GameObjectManager& manager, Node* node, int playerID, Matrix transform)
{
	BaseWarrior::init(manager, node, playerID, transform);
	float scale = COMMON_SCALE;
	_node->setScale(scale, scale, scale);
	SearchRadius = 20.0f;
}

///////////////////////////////////////////////////////////////////////////////////////////

BaseGameObject* CarrierWarrior::constructor()
{
	return new CarrierWarrior();
}
CarrierWarrior::CarrierWarrior()
: BaseWarrior()
{}
void CarrierWarrior::init(GameObjectManager& manager, Node* node, int playerID, Matrix transform)
{
	BaseWarrior::init(manager, node, playerID, transform);
	float scale = COMMON_SCALE;
	_node->setScale(scale, scale, scale);
	SearchRadius = 20.0f;
}

///////////////////////////////////////////////////////////////////////////////////////////

BaseGameObject* TempestWarrior::constructor()
{
	return new TempestWarrior();
}
TempestWarrior::TempestWarrior()
: BaseWarrior()
{}
void TempestWarrior::init(GameObjectManager& manager, Node* node, int playerID, Matrix transform)
{
	BaseWarrior::init(manager, node, playerID, transform);
	float scale = COMMON_SCALE;
	_node->setScale(scale, scale, scale);
	SearchRadius = 20.0f;
}

///////////////////////////////////////////////////////////////////////////////////////////

BaseGameObject* MothershipWarrior::constructor()
{
	return new MothershipWarrior();
}

MothershipWarrior::MothershipWarrior()
: BaseWarrior()
{}

void MothershipWarrior::init(GameObjectManager& manager, Node* node, int playerID, Matrix transform)
{
	BaseWarrior::init(manager, node, playerID, transform);
	float scale = COMMON_SCALE;
	_node->setScale(scale, scale, scale);
	SearchRadius = 20.0f;
}

///////////////////////////////////////////////////////////////////////////////////////////

BaseGameObject* CoreWarrior::constructor()
{
	return new CoreWarrior();
}

CoreWarrior::CoreWarrior()
: BaseWarrior()
, _altitude(0.0f)
{
}

void CoreWarrior::init(GameObjectManager& manager, Node* node, int playerID, Matrix transform)
{
	float scale = COMMON_SCALE;
	BaseWarrior::init(manager, node, playerID, transform);
	_node->setScale(scale, scale, scale);
	SearchRadius = 20.0f;
	_altitude = 3.75f;
}

void CoreWarrior::update(float time)
{
	BaseWarrior::update(time);
	if (_dead)
	{
		_altitude = (_altitude > 0.0f) ? _altitude - 0.002f * time : 0.0f;
	}
	OpenSteer::Vec3 pos = _movementController.position();
	_node->setTranslation(Vector3(pos.x, pos.y + _altitude, pos.z));
}