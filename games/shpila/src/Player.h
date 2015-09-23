#ifndef PLAYER_H_
#define PLAYER_H_

using namespace gameplay;

class GameObjectManager;
class HiddenObject;

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
	static const int CellsCountX = 18;
	static const int CellsCountZ = 12 * 2;

	WarriorsGrid();
	~WarriorsGrid();
	Vector3 Position;
	Vector3 AxisX;
	Vector3 AxisZ;

	void init();
	void mousOver(const Vector3 mousePos);
	bool isPlaceFree(int radius);
	const Vector3 getPlacePosition(const int radius);
	void markUnitPlace(const int radius);
	void render(const Matrix matrix, int radius);
private:
	const GridCell worldToGrid(const Vector3 point);
	bool getCell(GridCell cell);
	void setCell(GridCell cell, bool value);
	bool Cells[CellsCountX][CellsCountZ];
	SpriteBatch* _spriteBatch;
	float _factor;
	Vector3 _centerPos;
	Vector3 _cornerPos;
	bool _mouseInGrid;
	GridCell _cell;
	Vector3 _mousePos;
};

class PlayerObject
{
public:
	GameObjectManager& Manager;
	PlayerObject* EnemyPlayer;
	bool AutoPlay;
	int ID;
	int UprgadeLevel;
	Vector3 BattleFieldDirection;
	int MainResource;
	PlayerObject(GameObjectManager& manager, int id, Vector3 position, Vector3 battleFieldDirection);
	void update(float time);
	void setCreateWarior(const char* name);
	bool CreateWarrior(const Valuable<Vector3> position = Valuable<Vector3>::Undefined);
	int getNewObjectID();
	BaseStaticActor* getDefence();
	void mousOver(const Vector3 mousePos);
	void render();
private:
	CLink<TowerObject> _defenceTower;
	CLink<TheBaseObject> _defenceBase;
	Vector3 _position;
	int _warriorsSpawnedCount;
	int _newObjectID;
	float _mainResourceIncreacetimer;
	WarriorsGrid _grid;
	std::string _CurrentCharacterName;
};

#endif