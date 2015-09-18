#ifndef PLAYER_H_
#define PLAYER_H_

using namespace gameplay;

class GameObjectManager;
class HiddenObject;

class WarriorsGrid
{
public:
	static const int CellsCountX = 18;
	static const int CellsCountY = 12;

	WarriorsGrid();
	~WarriorsGrid();
	Vector3 Position;
	Vector3 AxisX;
	Vector3 AxisZ;

	void init();
	void mousOver(const Vector3 mousePos);
	bool isPlaceFree();
	const Vector3 getPlacePosition();
	void markUnitPlace();
	void render(const Matrix matrix);
private:
	bool Cells[CellsCountX][CellsCountY][2];
	SpriteBatch* _spriteBatch;
	float _factor;
	Vector3 _centerPos;
	Vector3 _cornerPos;
	bool _mouseInGrid;
	int _gridPosX;
	int _gridPosX_;
	int _gridPosZ;
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
	bool CreateWarrior(const char* name, const Valuable<Vector3> position = Valuable<Vector3>::Undefined);
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
};

#endif