#include "Headers.h"

JeatsWarrior::JeatsWarrior()
: BaseWarrior()
, _guardianShieldTimer()
, _hallucinationTimer()
, _auranode(NULL)
, _allowCreateIllusions(false)
{}

BaseGameObject* JeatsWarrior::constructor()
{
	return new JeatsWarrior();
}
void JeatsWarrior::init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, Matrix transform)
{
	float scale = JEATS_SCALE;
	BaseWarrior::init(manager, gameData, node, player, transform);
	_node->setScale(scale, scale, scale);
	_auranode = manager.AttachUnitModel("shieldaura", this, NULL);
	_auranode->setTag("transparent");
	_auranode->setEnabled(false);
	SearchRadius = 30.0f;
	_guardianShieldTimer.enable(false);
	_hallucinationTimer.enable(false);
}

void JeatsWarrior::update(float time)
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

void JeatsWarrior::interaction(BaseGameObject* object)
{
	BaseWarrior::interaction(object);
	if (!friendly(object))
	{
		if (!_hallucinationTimer.enabled())
		{
			if (LocalGameData.Mana >= LocalGameData.GameData->mana)
			{
				if (object->position().distanceSquared(position()) <= SQR(JEATS_USE_HALLUCINATION_DISTANCE))
				{
					_hallucinationTimer.enable(true);
					_hallucinationTimer.start(JEATS_HALLUCINATION_TIME, 0.0f);
					LocalGameData.Mana -= JEATS_HALLUCINATION_MANA_COST;
					_allowCreateIllusions = true;
				}
			}
		}
		if (!_guardianShieldTimer.enabled())
		{
			if (LocalGameData.Mana >= (0.5f * LocalGameData.GameData->mana))
			{
				if (object->position().distanceSquared(position()) <= SQR(JEATS_USE_GUARDIANSHIELD_DISTANCE))
				{
					_guardianShieldTimer.enable(true);
					_guardianShieldTimer.start(JEATS_GUARDIANSHIELD_TIME, 0.0f);
					LocalGameData.Mana -= JEATS_GUARDIANSHIELD_MANA_COST;
					_auranode->setEnabled(true);
				}
			}
		}
	}

	if (friendly(object))
	{
		if (_guardianShieldTimer.enabled())
		{
			if (checkDistanceToObject(object, JEATS_GUARDIANSHIELD_AURA_DISTANCE))
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