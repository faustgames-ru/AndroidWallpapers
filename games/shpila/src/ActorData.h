#ifndef ACTORDATA_H_
#define ACTORDATA_H_

using namespace gameplay;

class LocalActorData;

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
	float DamageUpgradeFactor[DamageTypesCount];
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
	bool ImmediateAttack;
	float DistanceGround;
	float DistanceAir;
	float DetectionDistance;//!! hardcoded
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
	float getDefaultDamageUpgradeFactor() const;
	bool isAttackToTargetAllowed(const ActorData& targetGameData) const;
	int getAttacksCount(const ActorData& targetGameData) const;
	int CellsRadius() const;
};

class Aura
{
public:
	enum Value
	{
		GuardianShield,

		Last
	};
};

class AuraState
{
public:
	enum Value
	{
		Intensify,
		Attenuate,
		Disabled,

		Last
	};
};

class LocalActorData
{
public:
	const ActorData* GameData;
	float Health;
	float Shield;
	float Mana;
	float ArmorUpgrade;
	float DamageUpgrade;
	float ShieldUpgrade;
	bool Illusion;

	LocalActorData();
	void init(const ActorData* gameData);
	void doDamage(LocalActorData& targetGameData);
	bool isAttackToTargetAllowed(const LocalActorData& targetGameData) const;
	bool getAura(Aura::Value aura);
	void setAura(Aura::Value aura, bool enabled);
private:
	std::map<Aura::Value, AuraState::Value> _auras;
};

void loadActorsData(char *filaname);
void freedActorsData();
const ActorData& getActorData(const char* actorName);
const ActorData& getActorData(int ActorType);


#endif