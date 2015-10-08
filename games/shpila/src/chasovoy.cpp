#include "Headers.h"

ChasovoyWarrior::ChasovoyWarrior()
: BaseWarrior()
, _guardianShieldTimer()
, _hallucinationTimer()
, _auranode(NULL)
, _allowCreateIllusions(false)
{}

BaseGameObject* ChasovoyWarrior::constructor()
{
	return new ChasovoyWarrior();
}
void ChasovoyWarrior::init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, Matrix transform)
{
	float scale = CHASOVOY_SCALE;
	BaseWarrior::init(manager, gameData, node, player, transform);
	_node->setScale(scale, scale, scale);
	manager.AttachUnitModel("shieldaura", this, NULL);
	_auranode = _node->getFirstChild()->getNextSibling();
	_auranode->setTag("transparent");
	_auranode->setEnabled(false);
	SearchRadius = 30.0f;
	_guardianShieldTimer.enable(false);
	_hallucinationTimer.enable(false);
}

void ChasovoyWarrior::update(float time)
{
	BaseWarrior::update(time);
	if (_hallucinationTimer.action(time))
	{
		_hallucinationTimer.enable(false);
	}

	if (_guardianShieldTimer.action(time))
	{
		_guardianShieldTimer.enable(false);
		_auranode->setEnabled(false);
	}
}

void ChasovoyWarrior::interaction(BaseGameObject* object)
{
	BaseWarrior::interaction(object);
	if (!friendly(object))
	{
		if (!_hallucinationTimer.enabled())
		{
			if (LocalGameData.Mana >= LocalGameData.GameData->mana)
			{
				if (object->position().distanceSquared(position()) <= SQR(CHASOVOY_USE_HALLUCINATION_DISTANCE))
				{
					_hallucinationTimer.enable(true);
					_hallucinationTimer.start(CHASOVOY_HALLUCINATION_TIME, 0.0f);
					LocalGameData.Mana -= CHASOVOY_HALLUCINATION_MANA_COST;
					_allowCreateIllusions = true;
				}
			}
		}
		if (!_guardianShieldTimer.enabled())
		{
			if (LocalGameData.Mana >= (0.5f * LocalGameData.GameData->mana))
			{
				if (object->position().distanceSquared(position()) <= SQR(CHASOVOY_USE_GUARDIANSHIELD_DISTANCE))
				{
					_guardianShieldTimer.enable(true);
					_guardianShieldTimer.start(CHASOVOY_GUARDIANSHIELD_TIME, 0.0f);
					LocalGameData.Mana -= CHASOVOY_GUARDIANSHIELD_MANA_COST;
					_auranode->setEnabled(true);
				}
			}
		}
	}

	if (friendly(object))
	{
		if (_guardianShieldTimer.enabled())
		{
			if (checkDistanceToObject(object, CHASOVOY_GUARDIANSHIELD_AURA_DISTANCE))
			{
				object->setAura(Aura::GuardianShield, true);
			}				
		}
		if (object->illusionable() && _allowCreateIllusions)
		{
			object->UsedForIllusion = true;
			BaseWarrior* obj = (BaseWarrior*)Player->Manager.createObject(object->TypeName.c_str(),
				object->position() + 5.0f * Player->BattleFieldDirection + 3.0f * Vector3(-Player->BattleFieldDirection.z, Player->BattleFieldDirection.y, Player->BattleFieldDirection.x),
				Player->BattleFieldDirection, Player);
			obj->makeIllusion();
			obj->setTransparency(0.4f);

			obj = (BaseWarrior*)Player->Manager.createObject(object->TypeName.c_str(),
				object->position() + 5.0f * Player->BattleFieldDirection - 3.0f * Vector3(-Player->BattleFieldDirection.z, Player->BattleFieldDirection.y, Player->BattleFieldDirection.x),
				Player->BattleFieldDirection, Player);
			obj->makeIllusion();
			obj->setTransparency(0.4f);

			_allowCreateIllusions = false;
		}
	}	
}