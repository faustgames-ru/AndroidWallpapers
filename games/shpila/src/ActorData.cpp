#include "Headers.h"
#include "tinyxml\tinyxml.h"

std::vector<ActorData*> ActorsData;

float ActorData::getDefaultDamage() const
{
	float min = 1e20f;
	for (int i = 0; i < DamageTypesCount; i++)
	{
		if (min > Damage[i])
		{
			min = Damage[i];
		}
	}
	return min;
}

float ActorData::getDefaultDamageUpgradeFactor() const
{
	float min = 1e20f;
	for (int i = 0; i < DamageTypesCount; i++)
	{
		if (min > DamageUpgradeFactor[i])
		{
			min = DamageUpgradeFactor[i];
		}
	}
	return min;
}

bool ActorData::isAttackToTargetAllowed(const ActorData& targetGameData) const
{
	return (TargetGround && targetGameData.MovementGround) || (TargetAir && targetGameData.MovementAir);
}

int ActorData::getAttacksCount(const ActorData& targetGameData) const
{
	int res = 1;
	if (targetGameData.MovementGround)
	{
		res = max(res, AttackCountGround);
	}
	if (targetGameData.MovementAir)
	{
		res = max(res, AttackCountAir);
	}
	return res;
}

LocalActorData::LocalActorData()
: GameData(NULL)
, Health(0.0f)
, Shield(0.0f)
, ArmorUpgrade(0.0f)
, DamageUpgrade(0.0f)
, ShieldUpgrade(0.0f)
{
}

void LocalActorData::init(const ActorData* gameData)
{
	if (gameData)
	{
		GameData = gameData;
		Health = GameData->HP;
		Shield = GameData->shield;
	}
}

void LocalActorData::doDamage(LocalActorData& targetGameData)
{
	float defaultDamage = GameData->getDefaultDamage();
	float defaultDamageUpgradeFactor = GameData->getDefaultDamageUpgradeFactor();
	
	float result = defaultDamage;
	if (targetGameData.GameData->MovementGround)
	{
		if (targetGameData.GameData->LightArmor)
			result = max(result, GameData->Damage[ActorData::DamageLight] + DamageUpgrade * GameData->DamageUpgradeFactor[ActorData::DamageLight]);
		if (targetGameData.GameData->HavyArmor)
			result = max(result, GameData->Damage[ActorData::DamageArmored] + DamageUpgrade * GameData->DamageUpgradeFactor[ActorData::DamageArmored]);
		if (targetGameData.GameData->Organic)
			result = max(result, GameData->Damage[ActorData::DamageOrganic] + DamageUpgrade * GameData->DamageUpgradeFactor[ActorData::DamageOrganic]);
		if (targetGameData.GameData->Psionic)
			result = max(result, defaultDamage + DamageUpgrade * defaultDamageUpgradeFactor);
		if (targetGameData.GameData->Mechanic)
			result = max(result, defaultDamage + DamageUpgrade * defaultDamageUpgradeFactor);
		if (targetGameData.GameData->Massive)
			result = max(result, GameData->Damage[ActorData::DamageMassive] + DamageUpgrade * GameData->DamageUpgradeFactor[ActorData::DamageMassive]);
		if (targetGameData.GameData->Building)
			result = max(result, GameData->Damage[ActorData::DamageBuilding] + DamageUpgrade * GameData->DamageUpgradeFactor[ActorData::DamageBuilding]);
	}
	else if (targetGameData.GameData->MovementAir)
	{
		if (targetGameData.GameData->LightArmor)
			result = max(result, GameData->Damage[ActorData::DamageLightAir] + DamageUpgrade * GameData->DamageUpgradeFactor[ActorData::DamageLightAir]);
		if (targetGameData.GameData->HavyArmor)
			result = max(result, GameData->Damage[ActorData::DamageArmoredAir] + DamageUpgrade * GameData->DamageUpgradeFactor[ActorData::DamageArmoredAir]);
		if (targetGameData.GameData->Organic)
			result = max(result, GameData->Damage[ActorData::DamageOrganicAir] + DamageUpgrade * GameData->DamageUpgradeFactor[ActorData::DamageOrganicAir]);
		if (targetGameData.GameData->Psionic)
			result = max(result, defaultDamage + DamageUpgrade * defaultDamageUpgradeFactor);
		if (targetGameData.GameData->Mechanic)
			result = max(result, defaultDamage + DamageUpgrade * defaultDamageUpgradeFactor);
		if (targetGameData.GameData->Massive)
			result = max(result, GameData->Damage[ActorData::DamageMassiveAir] + DamageUpgrade * GameData->DamageUpgradeFactor[ActorData::DamageMassiveAir]);
	}
	if (targetGameData.Shield > 0.0f)
	{
		result = max(result, GameData->Damage[ActorData::DamageShield]);
		float shieldDamage = min(targetGameData.Shield, result);
		targetGameData.Shield -= shieldDamage;
		result -= shieldDamage;
	}
	result = max(0.0f, result - (targetGameData.GameData->armour + targetGameData.ArmorUpgrade));
	targetGameData.Health -= result;
}

void loadActorsData(char *filaname)
{
	TiXmlDocument doc;
	doc.SetTabSize(8);
	doc.LoadFile(filaname);
	TiXmlNode *unit = doc.FirstChild("units");
	unit = unit->FirstChild("unit");
	while (unit)
	{
		ActorData* actorData = new ActorData();
		actorData->ActorType = ActorsData.size();
		ActorsData.push_back(actorData);
		actorData->Caption = unit->FirstChildElement("name")->GetText();
		actorData->Name = unit->FirstChildElement("id")->GetText();
		actorData->Price = atoi(unit->FirstChildElement("price")->GetText());
		actorData->RequireUpgrade = atoi(unit->FirstChildElement("RequireUpgrade")->GetText());
		actorData->BattleUpgradeClass = unit->FirstChildElement("BattleUpgradeClass")->GetText();
		actorData->Damage[ActorData::DamageLight] = (float)atof(unit->FirstChildElement("DamageLight")->GetText());
		actorData->Damage[ActorData::DamageArmored] = (float)atof(unit->FirstChildElement("DamageArmored")->GetText());
		actorData->Damage[ActorData::DamageMassive] = (float)atof(unit->FirstChildElement("DamageMassive")->GetText());
		actorData->Damage[ActorData::DamageShield] = (float)atof(unit->FirstChildElement("DamageShield")->GetText());
		actorData->Damage[ActorData::DamageOrganic] = (float)atof(unit->FirstChildElement("DamageOrganic")->GetText());
		actorData->Damage[ActorData::DamageBuilding] = (float)atof(unit->FirstChildElement("DamageBuilding")->GetText());
		actorData->Damage[ActorData::DamageLightAir] = (float)atof(unit->FirstChildElement("DamageLightAir")->GetText());
		actorData->Damage[ActorData::DamageArmoredAir] = (float)atof(unit->FirstChildElement("DamageArmoredAir")->GetText());
		actorData->Damage[ActorData::DamageMassiveAir] = (float)atof(unit->FirstChildElement("DamageMassiveAir")->GetText());
		actorData->Damage[ActorData::DamageShieldAir] = (float)atof(unit->FirstChildElement("DamageShieldAir")->GetText());
		actorData->Damage[ActorData::DamageOrganicAir] = (float)atof(unit->FirstChildElement("DamageOrganicAir")->GetText());
		actorData->DamageUpgradeFactor[ActorData::DamageLight] = (float)atof(unit->FirstChildElement("DamageLightUpgradeFactor")->GetText());
		actorData->DamageUpgradeFactor[ActorData::DamageArmored] = (float)atof(unit->FirstChildElement("DamageArmoredUpgradeFactor")->GetText());
		actorData->DamageUpgradeFactor[ActorData::DamageMassive] = (float)atof(unit->FirstChildElement("DamageMassiveUpgradeFactor")->GetText());
		actorData->DamageUpgradeFactor[ActorData::DamageShield] = (float)atof(unit->FirstChildElement("DamageShieldUpgradeFactor")->GetText());
		actorData->DamageUpgradeFactor[ActorData::DamageOrganic] = (float)atof(unit->FirstChildElement("DamageOrganicUpgradeFactor")->GetText());
		actorData->DamageUpgradeFactor[ActorData::DamageBuilding] = (float)atof(unit->FirstChildElement("DamageBuildingUpgradeFactor")->GetText());
		actorData->DamageUpgradeFactor[ActorData::DamageLightAir] = (float)atof(unit->FirstChildElement("DamageLightAirUpgradeFactor")->GetText());
		actorData->DamageUpgradeFactor[ActorData::DamageArmoredAir] = (float)atof(unit->FirstChildElement("DamageArmoredAirUpgradeFactor")->GetText());
		actorData->DamageUpgradeFactor[ActorData::DamageMassiveAir] = (float)atof(unit->FirstChildElement("DamageMassiveAirUpgradeFactor")->GetText());
		actorData->DamageUpgradeFactor[ActorData::DamageShieldAir] = (float)atof(unit->FirstChildElement("DamageShieldAirUpgradeFactor")->GetText());
		actorData->DamageUpgradeFactor[ActorData::DamageOrganicAir] = (float)atof(unit->FirstChildElement("DamageOrganicAirUpgradeFactor")->GetText());
		actorData->SplashDamage = (float)atof(unit->FirstChildElement("SplashDamage")->GetText());
		actorData->SplashDamageShield = (float)atof(unit->FirstChildElement("SplashDamageShield")->GetText());
		actorData->SplashType = unit->FirstChildElement("SplashType")->GetText();
		actorData->DistanceSplash = (float)atof(unit->FirstChildElement("DistanceSplash")->GetText());
		actorData->AttackDelayAir = 1000.0f * TIME_SCALE * (float)atof(unit->FirstChildElement("AttackDelayAir")->GetText());
		actorData->AttackDelayGround = 1000.0f * TIME_SCALE * (float)atof(unit->FirstChildElement("AttackDelayGround")->GetText());
		actorData->armour = (float)atof(unit->FirstChildElement("armour")->GetText());
		actorData->shield = (float)atof(unit->FirstChildElement("shield")->GetText());
		actorData->HP = (float)atof(unit->FirstChildElement("HP")->GetText());
		actorData->mana = (float)atof(unit->FirstChildElement("mana")->GetText());
		actorData->MoveSpeed = (float)atof(unit->FirstChildElement("MoveSpeed")->GetText());
		actorData->UndergroundSpeed = (float)atof(unit->FirstChildElement("UndergroundSpeed")->GetText());
		actorData->ImmediateAttack = (bool)atoi(unit->FirstChildElement("ImmediateAttack")->GetText());
		actorData->DistanceGround = (float)atof(unit->FirstChildElement("DistanceGround")->GetText());
		actorData->DistanceAir = (float)atof(unit->FirstChildElement("DistanceAir")->GetText());
		actorData->AttackCountGround = atoi(unit->FirstChildElement("AttackCountGround")->GetText());
		actorData->AttackCountAir = atoi(unit->FirstChildElement("AttackCountAir")->GetText());
		actorData->Detection = (float)atof(unit->FirstChildElement("Detection")->GetText());
		actorData->TargetGround = (bool)atoi(unit->FirstChildElement("TargetGround")->GetText());
		actorData->TargetAir = (bool)atoi(unit->FirstChildElement("TargetAir")->GetText());
		actorData->MovementGround = (bool)atoi(unit->FirstChildElement("MovementGround")->GetText());
		actorData->MovementAir = (bool)atoi(unit->FirstChildElement("MovementAir")->GetText());
		actorData->LightArmor = (bool)atoi(unit->FirstChildElement("LightArmor")->GetText());
		actorData->HavyArmor = (bool)atoi(unit->FirstChildElement("HavyArmor")->GetText());
		actorData->Organic = (bool)atoi(unit->FirstChildElement("Organic")->GetText());
		actorData->Psionic = (bool)atoi(unit->FirstChildElement("Psionic")->GetText());
		actorData->Mechanic = (bool)atoi(unit->FirstChildElement("Mechanic")->GetText());
		actorData->Massive = (bool)atoi(unit->FirstChildElement("Massive")->GetText());
		actorData->Building = (bool)atoi(unit->FirstChildElement("Building")->GetText());
		actorData->effectivity = unit->FirstChildElement("effectivity")->GetText();
		actorData->noneffectivity = unit->FirstChildElement("noneffectivity")->GetText();
		actorData->GeometryRadius = (float)atof(unit->FirstChildElement("GeometryRadius")->GetText());

		unit = unit->NextSibling("unit");
	}

	//empty data
	ActorData* actorData = new ActorData();
	ActorsData.push_back(actorData);
}

void freedActorsData()
{
	for (std::vector<ActorData*>::iterator it = ActorsData.begin(); it != ActorsData.end(); it++)
	{
		SAFE_DELETE(*it);
	}
}

const ActorData& getActorData(const char* actorName)
{
	for (std::vector<ActorData*>::iterator it = ActorsData.begin(); it != ActorsData.end(); it++)
	{
		if (!strcmp(actorName, (*it)->Name.c_str()))
			return **it;
	}
	return *(ActorsData.back());
}

const ActorData& getActorData(int ActorType)
{
	return *(ActorsData[ActorType]);
}