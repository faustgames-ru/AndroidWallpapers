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

float ActorData::getDamage(const ActorData& targetActorData) const
{
	float defaultDamage = getDefaultDamage();
	float result = defaultDamage;
	if (targetActorData.MovementGround)
	{
		if (targetActorData.LightArmor)
			result = max(result, Damage[DamageLight]);
		if (targetActorData.HavyArmor)
			result = max(result, Damage[DamageArmored]);
		if (targetActorData.Organic)
			result = max(result, Damage[DamageOrganic]);
		if (targetActorData.Psionic)
			result = max(result, defaultDamage);
		if (targetActorData.Mechanic)
			result = max(result, defaultDamage);
		if (targetActorData.Massive)
			result = max(result, Damage[DamageMassive]);
		if (targetActorData.Building)
			result = max(result, Damage[DamageBuilding]);
	}
	else if (targetActorData.MovementAir)
	{
		if (targetActorData.LightArmor)
			result = max(result, Damage[DamageLightAir]);
		if (targetActorData.HavyArmor)
			result = max(result, Damage[DamageArmoredAir]);
		if (targetActorData.Organic)
			result = max(result, Damage[DamageOrganicAir]);
		if (targetActorData.Psionic)
			result = max(result, defaultDamage);
		if (targetActorData.Mechanic)
			result = max(result, defaultDamage);
		if (targetActorData.Massive)
			result = max(result, Damage[DamageMassiveAir]);
	}
	return result;
}

bool ActorData::isAttackToTargetAllowed(const ActorData& targetGameData) const
{
	return (TargetGround && targetGameData.MovementGround) || (TargetAir && targetGameData.MovementAir);
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