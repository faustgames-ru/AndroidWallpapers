#include "Headers.h"



BaseGameObject* CheidaWarrior::constructor()
{
	return new CheidaWarrior();
}
CheidaWarrior::CheidaWarrior()
: BaseWarrior()
{}

void CheidaWarrior::init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, const Matrix transform)
{
	float scale = COMMON_SCALE;
	BaseWarrior::init(manager, gameData, node, player, transform);
	_node->setScale(scale, scale, scale);
}

///////////////////////////////////////////////////////////////////////////////////////////

BaseGameObject* HantîWarrior::constructor()
{
	return new HantîWarrior();
}
HantîWarrior::HantîWarrior()
: BaseWarrior()
{
}
void HantîWarrior::init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, const Matrix transform)
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
void ObserverWarrior::init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, const Matrix transform)
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
void ImmortalWarrior::init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, const Matrix transform)
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
void ColossusWarrior::init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, const Matrix transform)
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
void VoidRayWarrior::init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, const Matrix transform)
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
void CarrierWarrior::init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, const Matrix transform)
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
void TempestWarrior::init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, const Matrix transform)
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

void MothershipWarrior::init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, const Matrix transform)
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
{
}

void QbiWarrior::init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, const Matrix transform)
{
	float scale = COMMON_SCALE;
	BaseWarrior::init(manager, gameData, node, player, transform);
	_node->setScale(scale, scale, scale);
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

	if (_abilityTimer.action(time))
	{
		_abilityTimer.enable(false);
	}

}