#ifndef ACTORDATA_H_
#define ACTORDATA_H_

using namespace gameplay;


class ActorData
{
public:
	enum DamageType
	{
		DamageLight,
		DamageArmored,
		DamageMassive,
		DamageShield,
		DamageOrganic,
		DamageBuilding,
		DamageLightAir,
		DamageArmoredAir,
		DamageMassiveAir,
		DamageShieldAir,
		DamageOrganicAir,

		DamageTypesCount
	};

	int ActorType;
	std::string Caption;
	std::string Name;
	int Price;
	int RequireUpgrade;
	std::string BattleUpgradeClass;
	float Damage[DamageTypesCount];
	float SplashDamage;
	float SplashDamageShield;
	std::string SplashType;
	std::string DistanceSplash;
	float AttackDelayAir;
	float AttackDelayGround;
	float armour;
	float shield;
	float HP;
	float mana;
	float MoveSpeed;
	float UndergroundSpeed;
	float DistanceGround;
	float DistanceAir;
	int AttackCountGround;
	int AttackCountAir;
	float Detection;
	bool TargetGround;
	bool TargetAir;
	bool MovementGround;
	bool MovementAir;
	bool LightArmor;
	bool HavyArmor;
	bool Organic;
	bool Psionic;
	bool Mechanic;
	bool Massive;
	bool Building;
	std::string effectivity;
	std::string noneffectivity;
	float GeometryRadius;

	float getDefaultDamage() const;
	float getDamage(const ActorData& targetActorData) const;
	bool isAttackToTargetAllowed(const ActorData& targetGameData) const;
};

void loadActorsData(char *filaname);
void freedActorsData();
const ActorData& getActorData(const char* actorName);
const ActorData& getActorData(int ActorType);


#endif