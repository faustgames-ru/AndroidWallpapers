#include "Headers.h"



BaseGameObject* DarkWarrior::constructor()
{
	return new DarkWarrior();
}
DarkWarrior::DarkWarrior()
: BaseWarrior()
{}

void DarkWarrior::init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, Matrix transform)
{
	float scale = COMMON_SCALE;
	BaseWarrior::init(manager, gameData, node, player, transform);
	_node->setScale(scale, scale, scale);
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
void ArchonWarrior::init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, Matrix transform)
{
	float scale = COMMON_SCALE;
	BaseWarrior::init(manager, gameData, node, player, transform);
	_node->setScale(scale, scale, scale);
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
void ObserverWarrior::init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, Matrix transform)
{
	BaseWarrior::init(manager, gameData, node, player, transform);
	float scale = COMMON_SCALE;
	_node->setScale(scale, scale, scale);
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
void ImmortalWarrior::init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, Matrix transform)
{
	BaseWarrior::init(manager, gameData, node, player, transform);
	float scale = COMMON_SCALE;
	_node->setScale(scale, scale, scale);
}

///////////////////////////////////////////////////////////////////////////////////////////

BaseGameObject* ColossusWarrior::constructor()
{
	return new ColossusWarrior();
}
ColossusWarrior::ColossusWarrior()
: BaseWarrior()
{}
void ColossusWarrior::init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, Matrix transform)
{
	BaseWarrior::init(manager, gameData, node, player, transform);
	float scale = COMMON_SCALE;
	_node->setScale(scale, scale, scale);
}

///////////////////////////////////////////////////////////////////////////////////////////

BaseGameObject* VoidRayWarrior::constructor()
{
	return new VoidRayWarrior();
}
VoidRayWarrior::VoidRayWarrior()
: BaseWarrior()
{}
void VoidRayWarrior::init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, Matrix transform)
{
	BaseWarrior::init(manager, gameData, node, player, transform);
	float scale = COMMON_SCALE;
	_node->setScale(scale, scale, scale);
}

///////////////////////////////////////////////////////////////////////////////////////////

BaseGameObject* CarrierWarrior::constructor()
{
	return new CarrierWarrior();
}
CarrierWarrior::CarrierWarrior()
: BaseWarrior()
{}
void CarrierWarrior::init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, Matrix transform)
{
	BaseWarrior::init(manager, gameData, node, player, transform);
	float scale = COMMON_SCALE;
	_node->setScale(scale, scale, scale);
}

///////////////////////////////////////////////////////////////////////////////////////////

BaseGameObject* TempestWarrior::constructor()
{
	return new TempestWarrior();
}
TempestWarrior::TempestWarrior()
: BaseWarrior()
{}
void TempestWarrior::init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, Matrix transform)
{
	BaseWarrior::init(manager, gameData, node, player, transform);
	float scale = COMMON_SCALE;
	_node->setScale(scale, scale, scale);
}

///////////////////////////////////////////////////////////////////////////////////////////

BaseGameObject* MothershipWarrior::constructor()
{
	return new MothershipWarrior();
}

MothershipWarrior::MothershipWarrior()
: BaseWarrior()
{}

void MothershipWarrior::init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, Matrix transform)
{
	BaseWarrior::init(manager, gameData, node, player, transform);
	float scale = COMMON_SCALE;
	_node->setScale(scale, scale, scale);
}

///////////////////////////////////////////////////////////////////////////////////////////

BaseGameObject* QbiWarrior::constructor()
{
	return new QbiWarrior();
}

QbiWarrior::QbiWarrior()
: BaseWarrior()
, _altitude(0.0f)
{
}

void QbiWarrior::init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, Matrix transform)
{
	float scale = COMMON_SCALE;
	BaseWarrior::init(manager, gameData, node, player, transform);
	_node->setScale(scale, scale, scale);
	_altitude = AIR_UNITS_ALTITUDE;
	_abilityTimer.enable(false);
}

void QbiWarrior::interaction(BaseGameObject* object)
{
	BaseWarrior::interaction(object);
	
	if (!friendly(object) && checkDistanceToObject(object, QBI_TIME_WARP_ABILITY_RANGE))
	{
		if (!_abilityTimer.enabled())
		{
			Player->Manager.createObject("timewarp", object->position(), Player->BattleFieldDirection, Player);
			_abilityTimer.start(QBI_TIME_WARP_ABILITY_COLDDOWN, 0.0f);
			_abilityTimer.enable(true);
		}
	}
}

void QbiWarrior::update(float time)
{
	BaseWarrior::update(time);
	if (_dead)
	{
		_altitude = (_altitude > 0.0f) ? _altitude - 0.002f * time : 0.0f;
	}
	OpenSteer::Vec3 pos = _movementController.position();
	_node->setTranslation(Vector3(pos.x, pos.y + _altitude, pos.z));

	if (_abilityTimer.action(time))
	{
		_abilityTimer.enable(false);
	}

}