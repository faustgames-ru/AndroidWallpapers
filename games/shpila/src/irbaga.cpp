#include "Headers.h"

BaseGameObject* IrbagaWarrior::constructor()
{
	return new IrbagaWarrior();
}

void IrbagaWarrior::init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, Matrix transform)
{
	float scale = IRBAGA_SCALE;
	BaseWarrior::init(manager, gameData, node, player, transform);
	_node->setScale(scale, scale, scale);
	SearchRadius = 20.0f;
	_chargeAbilityTimer.enable(false);
	_chargeAbilityColdDownTimer.enable(false);
}

void IrbagaWarrior::updateMovementSpeed(float time)
{
	if (Target == NULL)
	{
		_movementController.setMaxSpeed(DEFAULT_MOVEMENT_SPEED / TIME_SCALE);
		_movementController.setSpeed(min(_movementController.speed(), _movementController.maxSpeed()));
		_movementController.setAcceleration(DEFAULT_UNIT_AXELERATION);
	}
	else
	{
		if (Player->getUpgrade(Upgrades::ZealotUpgrade) && !_chargeAbilityColdDownTimer.enabled())
		{
			float sqDiet = Target->position().distanceSquared(position());
			if (sqDiet < SQR(ZEALOT_CHARGE_DISTANCE + Target->LocalGameData.GameData->GeometryRadius + LocalGameData.GameData->GeometryRadius))
			{
				_chargeAbilityTimer.start(ZEALOT_CHARGE_TIME, 0.0f);
				_chargeAbilityTimer.enable(true);
				_chargeAbilityColdDownTimer.start(ZEALOT_CHARGE_COLDDOWN, 0.0f);
				_chargeAbilityColdDownTimer.enable(true);
				_movementController.setMaxSpeed(_movementController.maxSpeed() * ZEALOT_CHARGE_SPEED_FACTOR / TIME_SCALE);
				_movementController.setSpeed(_movementController.speed() * ZEALOT_CHARGE_SPEED_FACTOR / TIME_SCALE);
				_movementController.setAcceleration(_movementController.acceleration() * ZEALOT_CHARGE_SPEED_FACTOR);
			}
		}
		if (!_chargeAbilityTimer.enabled())
		{
			_movementController.setMaxSpeed(LocalGameData.GameData->MoveSpeed / TIME_SCALE);
			_movementController.setSpeed(min(_movementController.speed(), _movementController.maxSpeed()));
			_movementController.setAcceleration(DEFAULT_UNIT_AXELERATION);
		}
	}
	

	if (_chargeAbilityTimer.action(time))
	{
		_chargeAbilityTimer.enable(false);
		_movementController.setMaxSpeed(LocalGameData.GameData->MoveSpeed / TIME_SCALE);
		_movementController.setSpeed(min(_movementController.speed(), _movementController.maxSpeed()));
		_movementController.setAcceleration(DEFAULT_UNIT_AXELERATION);
	}
	if (_chargeAbilityColdDownTimer.action(time))
		_chargeAbilityColdDownTimer.enable(false);
}