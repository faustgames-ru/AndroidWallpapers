#include "Headers.h"

BaseGameObject* WukaWarrior::constructor()
{
	return new WukaWarrior();
}

void WukaWarrior::init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, const Matrix & transform)
{
	float scale = WUKA_SCALE;
	BaseWarrior::init(manager, gameData, node, player, transform);
	_node->setScale(scale, scale, scale);
	_chargeAbilityTimer.enable(false);
	_chargeAbilityColdDownTimer.enable(false);
	Target.subscribe(this, &WukaWarrior::targetChange);
}

void WukaWarrior::updateMovementSpeed(float time)
{
	if ((Target != NULL) && Player->upgrades()->getUpgrade(Upgrades::WukaUpgrade) &&
		!_chargeAbilityColdDownTimer.enabled() && checkDistanceToObject(Target, WUKA_CHARGE_DISTANCE))
	{
		_chargeAbilityTimer.start(WUKA_CHARGE_TIME, 0.0f);
		_chargeAbilityTimer.enable(true);
		_chargeAbilityColdDownTimer.start(WUKA_CHARGE_COLDDOWN, 0.0f);
		_chargeAbilityColdDownTimer.enable(true);
		_movementController.setMaxSpeed(_movementController.maxSpeed() * WUKA_CHARGE_SPEED_FACTOR / TIME_SCALE);
		_movementController.setSpeed(_movementController.speed() * WUKA_CHARGE_SPEED_FACTOR / TIME_SCALE);
		_movementController.setAcceleration(_movementController.acceleration() * WUKA_CHARGE_SPEED_FACTOR);
	}
	if (_chargeAbilityTimer.action(time))
		_chargeAbilityTimer.enable(false);
	if (_chargeAbilityColdDownTimer.action(time))
		_chargeAbilityColdDownTimer.enable(false);

	if (!_chargeAbilityTimer.enabled())
	{
		BaseWarrior::updateMovementSpeed(time);
	}
}

void WukaWarrior::doDamage(BaseGameObject* object)
{
	_chargeAbilityTimer.enable(false);
	BaseWarrior::doDamage(object);
}

void WukaWarrior::targetChange()
{
	_chargeAbilityTimer.enable(false);
}