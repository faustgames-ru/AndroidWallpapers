#include "Headers.h"

BudfoorWarrior::BudfoorWarrior()
: BaseWarrior()
, _blinkAbilityColdDownTimer()
{}

BaseGameObject* BudfoorWarrior::constructor()
{
	return new BudfoorWarrior();
}

void BudfoorWarrior::init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, Matrix transform)
{
	float scale = BUDFOOR_SCALE;
	BaseWarrior::init(manager, gameData, node, player, transform);
	_node->setScale(scale, scale, scale);
	SearchRadius = 20.0f;
	_blinkAbilityColdDownTimer.enable(false);
}

void BudfoorWarrior::update(float time)
{
	BaseWarrior::update(time);

	if (Player->getUpgrade(Upgrades::StalkerUpgrade) && !_blinkAbilityColdDownTimer.enabled())
	{
		if (Target && (Target->position().distanceSquared(position()) < SQR(STALKER_BLINK_DISTANCE)))
		{
			if (LocalGameData.Health <= ((LocalGameData.GameData->HP + LocalGameData.GameData->shield) * STALKER_BLINK_MIN_HEALTH_PERCENT))
			{
				setPosition(position() - _node->getForwardVectorWorld().normalize() * STALKER_BLINK_DISTANCE);
				_blinkAbilityColdDownTimer.enable(true);
				_blinkAbilityColdDownTimer.start(STALKER_BLINK_COLDDOWN, 0.0f);				
			}
		}
	}
	if (_blinkAbilityColdDownTimer.action(time))
		_blinkAbilityColdDownTimer.enable(false);
	/*Node* hand = _node->findNode("weapon");
	Node* weapon = _node->findNode("obj_weapon");
	weapon->set(hand->getWorldMatrix());*/
}