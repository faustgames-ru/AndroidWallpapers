#include "Headers.h"

#define SPRITE_VSH "res/shaders/sprite.vert"
#define SPRITE_FSH "res/shaders/sprite.frag"

std::map<Upgrades::Values, UpgradePrice> __upgradePrice;
std::map<Upgrades::Values, int> __upgradeMaxLevel;

void initUpgradeParams()
{
	__upgradePrice[Upgrades::BaseLevel] = BASE_LEVEL_UPGRADE_PRICE;
	__upgradePrice[Upgrades::WukaUpgrade] = WUKA_UPGRADE_PRICE;
	__upgradePrice[Upgrades::LamakiUpgrade] = LAMAKI_UPGRADE_PRICE;
	__upgradePrice[Upgrades::Shield] = SHIELD_UPGRADE_PRICE;
	__upgradePrice[Upgrades::GroundAttack] = GROUNDATTACK_UPGRADE_PRICE;
	__upgradePrice[Upgrades::AirAttack] = AIRATTACK_UPGRADE_PRICE;
	__upgradePrice[Upgrades::GroundArmor] = GROUNDARMOR_UPGRADE_PRICE;
	__upgradePrice[Upgrades::AirArmor] = AIRARMOR_UPGRADE_PRICE;
	
	__upgradeMaxLevel[Upgrades::BaseLevel] = 2;
	__upgradeMaxLevel[Upgrades::WukaUpgrade] = 1;
	__upgradeMaxLevel[Upgrades::LamakiUpgrade] = 1;
	__upgradeMaxLevel[Upgrades::Shield] = 3;
	__upgradeMaxLevel[Upgrades::GroundAttack] = 3;
	__upgradeMaxLevel[Upgrades::AirAttack] = 3;
	__upgradeMaxLevel[Upgrades::GroundArmor] = 3;
	__upgradeMaxLevel[Upgrades::AirArmor] = 3;
}

UpgradesData::UpgradesData(PlayerObject &player)
: _upgrade()
, _player(player)
{
	for (int i = Upgrades::BaseLevel; i < Upgrades::Last; i++)
		_upgrade[(Upgrades::Values)i] = 0;
}

int UpgradesData::getUpgrade(Upgrades::Values upgrade) const
{
	return _upgrade[upgrade];
}
bool UpgradesData::setUpgrade(Upgrades::Values upgrade, int value)
{
	bool res = (_upgrade[upgrade] < __upgradeMaxLevel[upgrade]) && (__upgradePrice[upgrade].Value[_upgrade[upgrade]] < _player.MainResource);
	if (res)
	{
		_player.MainResource -= __upgradePrice[upgrade].Value[_upgrade[upgrade]];
		_upgrade[upgrade] = value;
	}
	return res;
}

bool UpgradesData::incUpgrade(Upgrades::Values upgrade)
{	
	return setUpgrade(upgrade, getUpgrade(upgrade) + 1);
}

WarriorsGrid::WarriorsGrid()
: Position()
, AxisX()
, AxisZ()
, Air(false)
, _spriteBatch(NULL)
, _factor(1.0f)
, _centerPos()
, _cornerPos()
, _mouseInGrid(false)
, _cell(0, 0)
, _mousePos()
{
	for (int i = 0; i < CellsCountX; i++)
	{
		for (int j = 0; j < CellsCountZ; j++)
		{
			Cells[i][j] = false;
			Cells[i][j] = false;
		}
	}
}

WarriorsGrid::~WarriorsGrid()
{
	SAFE_DELETE(_spriteBatch);
}

void WarriorsGrid::init()
{
	_spriteBatch = SpriteBatch::create("res\\png\\warriorsgrid.png");
	_spriteBatch->getStateBlock()->setDepthWrite(false);
	_spriteBatch->getStateBlock()->setDepthTest(true);

	_factor = sqrt(2.0f);
	_centerPos = Position + Vector3(0.0f, 0.1f, 0.0f);
	_cornerPos = _centerPos - AxisX * _factor * 0.5f * (float)CellsCountX - AxisZ * _factor * 0.25f * (float)CellsCountZ;
}

void WarriorsGrid::mousOver(const Vector3 mousePos)
{
	_mousePos = mousePos;
	Vector3 dir = mousePos - _cornerPos;
	Vector3 localdir = Vector3(dir.dot(AxisX), 0.0, dir.dot(AxisZ));
	_mouseInGrid = (localdir.x >= 0.0f) && (localdir.x <= _factor * (float)CellsCountX) && (localdir.z >= 0.0f) && (localdir.z <= 0.5f * _factor * (float)CellsCountZ);
	_cell = worldToGrid(mousePos);
}

bool WarriorsGrid::isPlaceFree(int radius)
{
	bool res = true;
	switch (radius)//!!
	{
	case 3:
		res = res && !getCell(worldToGrid(_mousePos + Vector3::unitX()));
		res = res && !getCell(worldToGrid(_mousePos + Vector3::unitZ()));
		res = res && !getCell(worldToGrid(_mousePos + Vector3::unitX() + Vector3::unitZ()));
		res = res && !getCell(worldToGrid(_mousePos - Vector3::unitX() + Vector3::unitZ()));
		res = res && !getCell(worldToGrid(_mousePos + Vector3::unitX() - Vector3::unitZ()));
	case 2:
		res = res && !getCell(worldToGrid(_mousePos - Vector3::unitX()));
		res = res && !getCell(worldToGrid(_mousePos - Vector3::unitZ()));
		res = res && !getCell(worldToGrid(_mousePos - Vector3::unitX() - Vector3::unitZ()));
	case 1:
		res = res && !getCell(_cell);
	}
	return res;
}

const Vector3 WarriorsGrid::getPlacePosition(const int radius)
{
	Vector3 position = _cornerPos + AxisX * _factor * ((float)(_cell.PosZ % 2) * 0.5f + (float)_cell.PosX) + 0.5f * AxisZ * _factor * (float)_cell.PosZ;
	if (radius == 2)//!!
	{
		position = position - _factor * 0.5f * AxisX;
	}
	return position;
}

void WarriorsGrid::markUnitPlace(const int radius)
{
	GridCell cell(0, 0);
	switch (radius)//!!
	{
	case 3:
		setCell(worldToGrid(_mousePos + Vector3::unitX()), true);
		setCell(worldToGrid(_mousePos + Vector3::unitZ()), true);
		setCell(worldToGrid(_mousePos + Vector3::unitX() + Vector3::unitZ()), true);
		setCell(worldToGrid(_mousePos - Vector3::unitX() + Vector3::unitZ()), true);
		setCell(worldToGrid(_mousePos + Vector3::unitX() - Vector3::unitZ()), true);
	case 2:
		setCell(worldToGrid(_mousePos - Vector3::unitX()), true);
		setCell(worldToGrid(_mousePos - Vector3::unitZ()), true);
		setCell(worldToGrid(_mousePos - Vector3::unitX() - Vector3::unitZ()), true);
	case 1:
		setCell(_cell, true);
	}
	
}

void WarriorsGrid::render(const Matrix matrix, int radius)
{
	if (radius != 0)
	{
	_spriteBatch->start();
	_spriteBatch->setProjectionMatrix(matrix);
	
	//_spriteBatch->draw(_centerPos, AxisX, AxisZ, _factor * (float)CellsCountX, 0.5f * _factor * (float)CellsCountZ, 0, 0, 1, 1, Vector4(1, 1, 1, 1), Vector2::zero(), 0);
	
		Vector3 quadPos = _cornerPos;
		Vector4 color;
		for (int i = 0; i < CellsCountX; i++)
		{
			for (int j = 0; j < CellsCountZ; j++)
			{
				if ((j % 2) != 0)
					quadPos = _cornerPos + AxisX * _factor * (0.5f + (float)i) + 0.5f * AxisZ * _factor * (float)j;
				else
					quadPos = _cornerPos + AxisX * _factor * (float)i + 0.5f * AxisZ * _factor * (float)j;
				drawCell(quadPos, Cells[i][j] ? Selected : Free, Air);
			}
		}
		if (_mouseInGrid)
		{
			switch (radius)//!!
			{
			case 3:
				drawCell(getPlacePosition(1) + Vector3::unitX(), getCell(worldToGrid(_mousePos + Vector3::unitX())) ? Occupied : Selected, Air);
				drawCell(getPlacePosition(1) + Vector3::unitZ(), getCell(worldToGrid(_mousePos + Vector3::unitZ())) ? Occupied : Selected, Air);
				drawCell(getPlacePosition(1) + Vector3::unitX() + Vector3::unitZ(), getCell(worldToGrid(_mousePos + Vector3::unitX() + Vector3::unitZ())) ? Occupied : Selected, Air);
				drawCell(getPlacePosition(1) - Vector3::unitX() + Vector3::unitZ(), getCell(worldToGrid(_mousePos - Vector3::unitX() + Vector3::unitZ())) ? Occupied : Selected, Air);
				drawCell(getPlacePosition(1) + Vector3::unitX() - Vector3::unitZ(), getCell(worldToGrid(_mousePos + Vector3::unitX() - Vector3::unitZ())) ? Occupied : Selected, Air);
			case 2:
				drawCell(getPlacePosition(1) - Vector3::unitX(), getCell(worldToGrid(_mousePos - Vector3::unitX())) ? Occupied : Selected, Air);
				drawCell(getPlacePosition(1) - Vector3::unitZ(), getCell(worldToGrid(_mousePos - Vector3::unitZ())) ? Occupied : Selected, Air);
				drawCell(getPlacePosition(1) - Vector3::unitX() - Vector3::unitZ(), getCell(worldToGrid(_mousePos - Vector3::unitX() - Vector3::unitZ())) ? Occupied : Selected, Air);
			case 1:
				drawCell(getPlacePosition(1), getCell(_cell) ? Occupied : Selected, Air);
			}
		}	
	_spriteBatch->finish();
	}
}

const GridCell WarriorsGrid::worldToGrid(const Vector3 point)
{
	GridCell res(0, 0);
	Vector3 dir = point - _cornerPos;
	Vector3 localdir = Vector3(dir.dot(AxisX), 0.0, dir.dot(AxisZ));
	res.PosZ = (localdir.z + 0.25f * _factor) / (0.5f *_factor);
	res.PosX = (localdir.x + 0.5f * _factor - (res.PosZ % 2) * 0.5f * _factor) / (_factor);
	return res;
}

bool WarriorsGrid::getCell(GridCell cell)
{
	if ((cell.PosX < 0) || (cell.PosX >= CellsCountX) || (cell.PosZ < 0) || (cell.PosZ >= CellsCountZ))
		return true;
	else
		return Cells[cell.PosX][cell.PosZ];
}

void WarriorsGrid::setCell(GridCell cell, bool value)
{
	Cells[cell.PosX][cell.PosZ] = value;
}

void WarriorsGrid::drawCell(Vector3 position, CellState state, bool air)
{
	Vector4 color = Vector4(1, 1, 1, 1);
	Vector4 tex;
	tex.y = air ? 0.5f : 0.0f;
	tex.w = air ? 1.0f : 0.5f;
	switch (state)
	{
	case Free:
		tex.x = 0.0f;
		tex.z = 0.25f;
		break;
	case Selected:
		tex.x = 0.25f;
		tex.z = 0.5f;
		break;
	case Occupied:
		tex.x = 0.5f;
		tex.z = 0.75f;
		break;
	}
	_spriteBatch->draw(position, Vector3::unitX(), Vector3::unitZ(), 1.0f, 1.0f, tex.x, tex.y, tex.z, tex.w, color, Vector2::zero(), 0);
}

PlayerObject::PlayerObject(GameObjectManager& manager, int id, Vector3 position, Vector3 battleFieldDirection)
: Manager(manager)
, EnemyPlayer(NULL)
, AutoPlay(false)
, ID(id)
//, UprgadeLevel(0)
, BattleFieldDirection(battleFieldDirection)
, BattleFieldMidPoint()
, UnitsOverMidLineCount(0)
, MainResource(0)
, _upgradesData(*this)
, _defenceTower()
, _defenceBase()
, _position(position)
, _warriorsSpawnedCount(0)
, _newObjectID(0)
, _mainResourceIncreaceTimer()
, _ExtractorBuildTimer()
, _gridGround()
, _gridAir()
, _CurrentCharacterName()
, _controlMid(false)
, _extractorsCount(0)
{
	_gridGround.AxisX = Vector3(BattleFieldDirection.z, BattleFieldDirection.y, -BattleFieldDirection.x);
	_gridGround.AxisZ = BattleFieldDirection;
	_gridGround.Position = _position;
	_gridGround.init();

	_gridAir.AxisX = Vector3(BattleFieldDirection.z, BattleFieldDirection.y, -BattleFieldDirection.x);
	_gridAir.AxisZ = BattleFieldDirection;
	_gridAir.Position = _position + Vector3(0.0f, AIR_UNITS_ALTITUDE, 0.0f);
	_gridAir.Air = true;
	_gridAir.init();
	
	_mainResourceIncreaceTimer.start(1000.0f, 0.0f);
	_ExtractorBuildTimer.enable(false);
}


void PlayerObject::update(float time)
{
	//create own nexus
	if (_defenceBase == NULL)
	{
		_defenceBase = (TheBaseObject*)Manager.createObject("base", _position + 25.45f * BattleFieldDirection, BattleFieldDirection, this);
	}

	if (_defenceTower == NULL)
	{
		_defenceTower = (TowerObject*)Manager.createObject("tower", _position + 45.25f * BattleFieldDirection, BattleFieldDirection, this);
	}
	if (_mainResourceIncreaceTimer.action(time))
		MainResource += 2;

	if (_ExtractorBuildTimer.action(time))
	{
		_extractorsCount++;
		_mainResourceIncreaceTimer.enable(true);
		_ExtractorBuildTimer.enable(false);
	}
		
	_controlMid = UnitsOverMidLineCount > EnemyPlayer->UnitsOverMidLineCount;
	UnitsOverMidLineCount = 0;
}

void PlayerObject::setCreateWariorName(const char* name)
{
	_CurrentCharacterName = name;
}

const std::string PlayerObject::getCreateWariorName()
{
	return _CurrentCharacterName;
}

bool PlayerObject::CreateWarrior(bool continuous, const Valuable<Vector3> position)
{
	if (_CurrentCharacterName.empty())
		return false;
	
	bool res = false;
	const ActorData& ad = getActorData(_CurrentCharacterName.c_str());
	if (ad.Price < MainResource)
	{
		WarriorsGrid &_grid = ad.MovementGround ? _gridGround : _gridAir;
		res = _grid.isPlaceFree(ad.CellsRadius());
		if (res)
		{
			const Vector3 warriorPosition = position.defined() ? position : _grid.getPlacePosition(ad.CellsRadius());
			_grid.markUnitPlace(ad.CellsRadius());
			BaseWarrior* warrior = (BaseWarrior*)Manager.createObject(_CurrentCharacterName.c_str(), warriorPosition, BattleFieldDirection, this);
			warrior->Holder = true;
			_warriorsSpawnedCount++;
			MainResource -= ad.Price;
			if (!continuous)
			{
				_CurrentCharacterName = "";
			}
				
		}
	}
	return res;
}

void PlayerObject::CancelCreateWarrior()
{
	_CurrentCharacterName = "";
}

void PlayerObject::addExtractor()
{
	if ((EXTRACTOR_PROCE < MainResource) && (_mainResourceIncreaceTimer.enabled()))
	{
		MainResource -= EXTRACTOR_PROCE;
		_ExtractorBuildTimer.start(EXTRACTOR_BUILD_TIME, 0.0f);
		_ExtractorBuildTimer.enable(true);
		_mainResourceIncreaceTimer.enable(false);
	}
}

bool PlayerObject::defited()
{
	return getDefence()->LocalGameData.Health <= 0.0f;
}

int PlayerObject::getNewObjectID()
{
	return _newObjectID++;
}

UpgradesData* PlayerObject::upgrades()
{
	return &_upgradesData;
}

BaseStaticActor* PlayerObject::getDefence()
{
	return _defenceTower->LocalGameData.Health > 0.0 ? (BaseStaticActor*)_defenceTower : (BaseStaticActor*)_defenceBase;
}

Vector3 PlayerObject::getPosition()
{
	return _position;
}

int PlayerObject::getAdditionalResourceIncreasePercent()
{
	int res = 0;
	if (_controlMid)
		res += 15;
	res += _extractorsCount * 10;
	return res;
}

int PlayerObject::getExtractorBuildingTime()
{
	return (int)(EXTRACTOR_BUILD_TIME - _ExtractorBuildTimer.getTime()) / 1000;
}

bool PlayerObject::isExtractorBuilding()
{
	return _ExtractorBuildTimer.enabled();
}

void PlayerObject::mousOver(const Vector3 mousePos)
{
	_gridGround.mousOver(mousePos);
	_gridAir.mousOver(mousePos);
}

void PlayerObject::render()
{
	const ActorData& ad = getActorData(_CurrentCharacterName.c_str());
	int radius = !_CurrentCharacterName.empty() ? ad.CellsRadius() : 0;	
	if (ad.MovementGround)
		_gridGround.render(Manager.scene()->getActiveCamera()->getViewProjectionMatrix(), radius);
	else
		_gridAir.render(Manager.scene()->getActiveCamera()->getViewProjectionMatrix(), radius);
}