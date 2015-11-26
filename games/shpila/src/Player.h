#ifndef PLAYER_H_
#define PLAYER_H_

using namespace gameplay;

class GameObjectManager;
class HiddenObject;

class Upgrades
{
public:
	enum Values
	{
		BaseLevel,
		Shield,
		GroundAttack,
		AirAttack,
		GroundArmor,
		AirArmor,
		WukaUpgrade,
		LamakiUpgrade,

		Last
	};
};

class UpgradePrice
{
public:
	UpgradePrice(){ Value[0] = Value[1] = Value[2] = 0; }
	UpgradePrice(int v1){ Value[0] = Value[1] = Value[2] = v1;}
	UpgradePrice(int v1, int v2){ Value[0] = v1; Value[1] = Value[2] = v2; }
	UpgradePrice(int v1, int v2, int v3){ Value[0] = v1; Value[1] = v2; Value[2] = v3; }
	int Value[3];
};

class PlayerObject;

class UpgradesData
{
public:
	UpgradesData(PlayerObject &player);

	int getUpgrade(Upgrades::Values upgrade) const;
	bool setUpgrade(Upgrades::Values upgrade, int value);
	bool incUpgrade(Upgrades::Values upgrade);
private:
	mutable std::map<Upgrades::Values, int> _upgrade;
	PlayerObject &_player;
};

class GridCell
{
public:
	GridCell(int posX, int posZ)
	: PosX(posX)
	, PosZ(posZ)
	{}
	int PosX;
	int PosZ;
};

class WarriorsGrid
{
public:
	enum CellState
	{
		Free,
		Selected,
		Occupied
	};
	static const int CellsCountX = 18;
	static const int CellsCountZ = 12 * 2;

	WarriorsGrid();
	~WarriorsGrid();
	Vector3 Position;
	Vector3 AxisX;
	Vector3 AxisZ;
	bool Air;

	void init();
	void mousOver(const Vector3 mousePos);
	bool isPlaceFree(const Vector3& position, int radius);
	const Vector3 getPlacePosition(const int radius);
	void markUnitPlace(const Vector3& position, const int radius);
	void render(const Matrix matrix, int radius);
	Vector3& mousePos() const;
private:
	GridCell worldToGrid(const Vector3& point) const;
	bool getCell(GridCell cell);
	void setCell(GridCell cell, bool value);
	void drawCell(Vector3 position, CellState state, bool air);
	bool Cells[CellsCountX][CellsCountZ];
	SpriteBatch* _spriteBatch;
	float _factor;
	Vector3 _centerPos;
	Vector3 _cornerPos;
	bool _mouseInGrid;
	GridCell _cell;
	mutable Vector3 _mousePos;
};

typedef bool(*OnCreateWarrior) (const char* name, const Vector3& position);

//players union
class PlayerObject : public CLinkObject
{
public:
	GameObjectManager& Manager;
	PlayerObject* EnemyPlayer;
	bool AutoPlay;
	int ID;
	//int UprgadeLevel;
	Vector3 BattleFieldDirection;
	Vector3 BattleFieldMidPoint;
	int UnitsOverMidLineCount;
	int MainResource;
	OnCreateWarrior OnCreateWarriorHandler;

	PlayerObject(GameObjectManager& manager, int id, Vector3 position, Vector3 battleFieldDirection);
	void update(float time);
	void setCreateWariorName(const char* name);
	std::string getCreateWariorName() const;
	bool CreateWarrior(bool continuous, const Valuable<Vector3> position = Valuable<Vector3>::Undefined);
	void CancelCreateWarrior();
	void addExtractor();
	bool defited();
	int getNewObjectID();
	UpgradesData* upgrades();
	BaseStaticActor* getDefence();
	Vector3 getPosition();
	int getAdditionalResourceIncreasePercent();
	int getExtractorBuildingTime();
	bool isExtractorBuilding();
	void mousOver(int x, int y);
	void render();
private:
	CLink<TowerObject> _defenceTower;
	CLink<TheBaseObject> _defenceBase;
	Vector3 _position;
	int _warriorsSpawnedCount;
	int _newObjectID;
	SimpleTimer _mainResourceIncreaceTimer;
	SimpleTimer _ExtractorBuildTimer;
	WarriorsGrid _gridGround;
	WarriorsGrid _gridAir;
	std::string _CurrentCharacterName;
	bool _controlMid;
	int _extractorsCount;
	UpgradesData _upgradesData;
};

void initUpgradeParams();

#endif