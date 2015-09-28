#include "main.h"

#include "tinyxml\tinyxml.h"

// Declare our game instance
Shpila game;

// Input flags
#define NORTH 1
#define SOUTH 2
#define EAST 4
#define WEST 8
#define RUNNING 16

// Character defines
#define WALK_SPEED  5.0f
#define STRAFE_SPEED 1.5f
#define RUN_SPEED 15.0f
#define CAMERA_FOCUS_DISTANCE 16.0f

#define BUTTON_1 0
#define BUTTON_2 1

#define RESPAWN_TIME 30000.0f


static const unsigned int KEY_ALT = 1;
static const unsigned int KEY_CTRL = 2;
static const unsigned int MOVE_SHIFT = 4;


static const float MOVE_SPEED = 15.0f;
static const float UP_DOWN_SPEED = 10.0f;

static const bool EMPTY = false;

Shpila::Shpila()
: _font(NULL)
, _scene()
, _physicsDebug(false)
, _wireframe(false)
, _cameraVelocityFactor(1.0f)
, _actionsMap()
, _keyMap()
, _mouseX(0)
, _mouseY(0)
, _wheelDelta(0)
, _fpCamera()
, _activeCamera(NULL)
, _activePlayerCamera(NULL)
, _freeCamera(true)
, _battleFieldDirection()
, _particleEmitterSunNode(NULL)
, _particleEmitterStarsNode(NULL)
, _hud()
, _totalTime(0.0)
, _manager()
, _currentPlayerIDforUI(0)
, _ping(0)
, _netPlayerID(-1)
, _respawnTime(0.0)
, Respawn(false)
{
}

void Shpila::restoreDeviceObjects()
{
	if (!EMPTY)
	{
		Game::restoreDeviceObjects();
		if (isInitialized())
		{
			_scene->visit(this, &Shpila::initializeNodeMaterials);
		}
	}
}

void Shpila::initialize()
{
	if (!EMPTY)
	{
		// Enable multi-touch (only affects devices that support multi-touch).
		setMultiTouch(true);

		// Display the gameplay splash screen for at least 1 second.
		//displayScreen(this, &Shpila::drawSplash, NULL, 1000L); 

		// Load the font.
		_font = Font::create("res/ui/arial.gpb");
		_font->setCharacterAdvance(' ', 1);

		const float FoV = 60.0f;

		_hud.initialize(this, _scene);
		_hud.bind("Player1_New_Irbaga", Control::Listener::CLICK, CreateUnit);
		_hud.bind("Player1_New_Chasovoy", Control::Listener::CLICK, CreateUnit);
		_hud.bind("Player1_New_Budfoor", Control::Listener::CLICK, CreateUnit);
		_hud.bind("Player1_New_Dark", Control::Listener::CLICK, CreateUnit); 
		_hud.bind("Player1_New_Barar", Control::Listener::CLICK, CreateUnit);
		_hud.bind("Player1_New_Archon", Control::Listener::CLICK, CreateUnit); 
		_hud.bind("Player1_New_Observer", Control::Listener::CLICK, CreateUnit); 
		_hud.bind("Player1_New_Immortal", Control::Listener::CLICK, CreateUnit); 
		_hud.bind("Player1_New_Colossus", Control::Listener::CLICK, CreateUnit); 
		_hud.bind("Player1_New_Albiria", Control::Listener::CLICK, CreateUnit);
		_hud.bind("Player1_New_VoidRay", Control::Listener::CLICK, CreateUnit); 
		_hud.bind("Player1_New_Carrier", Control::Listener::CLICK, CreateUnit); 
		_hud.bind("Player1_New_Tempest", Control::Listener::CLICK, CreateUnit); 
		_hud.bind("Player1_New_Mothership", Control::Listener::CLICK, CreateUnit); 
		_hud.bind("Player1_New_MothershipCore", Control::Listener::CLICK, CreateUnit);
		//_hud.bind("Show_Units_P1", Control::Listener::CLICK, ShowUnitsP1);
		_hud.form()->getControl("Player1")->setVisible(false);
		
		_hud.form()->getControl("Tune")->setVisible(false);
		_hud.form()->getControl("Connection")->setVisible(false);
		_hud.bind("ShowTune", Control::Listener::CLICK, ShowTunes);
		_hud.bind("Connect", Control::Listener::CLICK, ShowConnection);		
		_hud.bind("CameraFoVPlus", Control::Listener::CLICK, CameraFoVPlus);
		_hud.bind("CameraFoVMinus", Control::Listener::CLICK, CameraFoVMinus);
		_hud.bind("ApplyConnection", Control::Listener::CLICK, ConnectToServer);
		_hud.bind("SwitchToPlayer1", Control::Listener::CLICK, SwitchPlayer);
		_hud.bind("SwitchToPlayer2", Control::Listener::CLICK, SwitchPlayer);

		_hud.bind("UpgradeLevel1", Control::Listener::CLICK, Upgrade);
		_hud.bind("UpgradeLevel2", Control::Listener::CLICK, Upgrade);
		_hud.bind("extractor", Control::Listener::CLICK, AddExtractor);
		

		_hud.bind("SettingsSave", Control::Listener::CLICK, saveSetting);
		_hud.bind("SettingsLoad", Control::Listener::CLICK, loadSetting);
		_hud.bind("AddEnergy", Control::Listener::CLICK, AddEnergy);

		_hud.bind("Pause", Control::Listener::CLICK, PauseClick);
			
		char buff[100];
		sprintf(buff, "FoV-%0.0f", FoV);
		((Label*)_hud.form()->getControl("CameraFoVTitle"))->setText(buff);
		_hud.bind("CameraFree", Control::Listener::CLICK, SetCameraFree);
		_hud.bind("CameraLocked", Control::Listener::CLICK, SetCameraLocked);		

		// Load scene.
		_scene = Scene::load("res/common/box.scene");

		// Update the aspect ratio for our scene's camera to match the current device resolution.
		_scene->getActiveCamera()->setAspectRatio(getAspectRatio());

		//start prepare cameras
		_fpCamera.initialize(0.01f, 1000.0f);
		_scene->addNode(_fpCamera.getRootNode());
		_fpCamera.getCamera()->setFieldOfView(FoV);

		_CameraPlayer[0].initialize(0.01f, 1000.0f);
		_scene->addNode(_CameraPlayer[0].getRootNode());
		_CameraPlayer[0].getCamera()->setFieldOfView(FoV);

		_CameraPlayer[1].initialize(0.01f, 1000.0f);
		_scene->addNode(_CameraPlayer[1].getRootNode());
		_CameraPlayer[1].getCamera()->setFieldOfView(FoV);
		if (FileSystem::fileExists("res/settings.xml"))
		{
			loadSetting(this, NULL);
		}
		else
		{
			//setup camera 1			
			_fpCamera.setPosition(Vector3(-0.960945f, 15.573791f, -17.046848f));
			Matrix rot;
			Matrix::createLookAt(0.0f, 0.0f, 0.0f,
				0.0f, -1.0f, 1.0f,
				0.0f, 1.0f, 0.5f, &rot);
			_fpCamera.setRotation(rot);
			//setup camera 2
			_CameraPlayer[0].setPosition(Vector3(42.202988f, 15.573791f, -67.127838f));
			Matrix::createLookAt(0.0f, 0.0f, 0.0f,
				0.0f, -1.0f, 1.0f,
				0.0f, 1.0f, 0.5f, &rot);
			_CameraPlayer[0].setRotation(rot);
			//setup camera 3
			_CameraPlayer[1].setPosition(Vector3(-42.202988f, 15.573791f, 47.127838f));
			Matrix::createLookAt(0.0f, 0.0f, 0.0f,
				0.0f, -1.0f, 1.0f,
				0.0f, 1.0f, 0.5f, &rot);
			_CameraPlayer[1].setRotation(rot);
			
		}
		_scene->setActiveCamera(_fpCamera.getCamera());
		_activeCamera = &_fpCamera;
		_activePlayerCamera = &_CameraPlayer[0];
		//finish prepare cameras

		_scene->setAmbientColor(0.25f, 0.25f, 0.25f);

		_manager.setScene(_scene);

		loadCharacters();
		initPlayers();
		loadActionMap();

		// Initialize scene.
		_scene->visit(this, &Shpila::initializeNodeMaterials);

		OpenSteer::OpenSteerManager::initialize();
	}
	else
	{
		Game::restoreDeviceObjects();
	}
}

bool Shpila::initializeNodeMaterials(Node* node)
{
    return _manager.initializeNodeMaterials(node);
}

bool Shpila::isActivePlayer(PlayerObject* player)
{
	return (_netPlayerID == UNASSIGNED_PLAYER_INDEX) || (_netPlayerID == player->ID);
}
PlayerObject* Shpila::getActivePlayer()
{
	int player = (_netPlayerID != UNASSIGNED_PLAYER_INDEX) ? _netPlayerID : _currentPlayerIDforUI;
	return _manager.Players[player];
}

void Shpila::initializeMaterial(Scene* scene, Node* node, Material* material)
{
	_manager.initializeMaterial(node, material);
}

void Shpila::initializeAsteroids()
{
	//gameplay-encoder.exe -g HIKCharacterNode_Boy_Reference animations -g greenchair2 chairanimations boyscene.fbx boyscene.gpb
	//gameplay-encoder.exe -g root animations asteroidstest.fbx asteroidstest.gpb
	Node* node = _scene->findNode("sphere");
	Animation* animation = node->getAnimation("animations");
	if (animation)
	{
		animation->createClips("res/common/asteroidstest.animation");
		AnimationClip* clip = animation->getClip("idle");
		clip->setSpeed(1.0f);
		clip->setRepeatCount(AnimationClip::REPEAT_INDEFINITE);
		clip->play();
	}
}

void Shpila::initializeSolarSystem()
{	
	//gameplay-encoder.exe -tb solarsystem -g root animations solarsystem.fbx solarsystem.gpb
	Node* node = _scene->findNode("solarsystem");
	Animation* animation = node->getAnimation("animations");
	if (animation)
	{
		animation->createClips("res/common/solarsystem.animation");
		AnimationClip* clip = animation->getClip("idle");
		clip->setSpeed(1.0f);
		clip->setRepeatCount(AnimationClip::REPEAT_INDEFINITE);
		clip->play();
	}
}

void Shpila::loadCharacters()
{
	loadActorsData("res/units.xml");

	_manager.addUnit("res/common/irbaga.scene", "irbaga", IrbagaWarrior::constructor);
	_manager.addUnit("res/common/chasovoy.scene", "chasovoy", ChasovoyWarrior::constructor);
	_manager.addUnit("res/common/budfoor.scene", "budfoor", BudfoorWarrior::constructor);
	_manager.addUnit("res/common/dark.scene", "dark", DarkWarrior::constructor);
	_manager.addUnit("res/common/barar.scene", "barar", BararWarrior::constructor);
	_manager.addUnit("res/common/archon.scene", "archon", ArchonWarrior::constructor);
	_manager.addUnit("res/common/observer.scene", "observer", ObserverWarrior::constructor);
	_manager.addUnit("res/common/immortal.scene", "immortal", ImmortalWarrior::constructor);
	_manager.addUnit("res/common/colossus.scene", "colossus", ColossusWarrior::constructor);
	_manager.addUnit("res/common/albiria.scene", "albiria", AlbiriaWarrior::constructor);
	_manager.addUnit("res/common/voidray.scene", "voidray", VoidRayWarrior::constructor);
	_manager.addUnit("res/common/carrier.scene", "carrier", CarrierWarrior::constructor);
	_manager.addUnit("res/common/tempest.scene", "tempest", TempestWarrior::constructor);
	_manager.addUnit("res/common/mothership.scene", "mothership", MothershipWarrior::constructor);
	_manager.addUnit("res/common/mothershipcore.scene", "mothershipcore", CoreWarrior::constructor);
	_manager.addUnit("res/common/SourceSmall.scene", "tower", TowerObject::constructor);
	_manager.addUnit("res/common/SourceBig.scene", "base", TheBaseObject::constructor);
	_manager.addUnit("res/common/bullet.scene", "bullet", BaseBullet::constructor);
	_manager.initUnits();
}

void Shpila::initPlayers()
{
	Vector3 p1 = Vector3(50.0f, 0.0f, -50.0f);
	Vector3 p2 = Vector3(-50.0f, 0.0f, 50.0f);
	_battleFieldDirection = Vector3(p2 - p1).normalize();		
	PlayerObject* pl1 = new PlayerObject(_manager, 0, p1, _battleFieldDirection);
	PlayerObject* pl2 = new PlayerObject(_manager, 1, p2, -_battleFieldDirection);
	pl1->EnemyPlayer = pl2;
	pl1->MainResource = 600;
	pl1->ID = 0;
	pl1->BattleFieldMidPoint = Vector3::zero();
	pl2->EnemyPlayer = pl1;
	pl2->MainResource = 600;	
	pl2->ID = 1;
	pl2->BattleFieldMidPoint = Vector3::zero();
	_manager.Players.push_back(pl1);
	_manager.Players.push_back(pl2);
}

void Shpila::updatePlayers(float time)
{
	for (std::vector<PlayerObject*>::iterator it = _manager.Players.begin(); it != _manager.Players.end(); ++it)
	{
		const Vector3 pos = ProjectToZeroPlane(_activeCamera->getCamera(), _mouseX, _mouseY);
		(*it)->mousOver(pos);
		(*it)->update(time);
	}
}

void Shpila::updateNetwork()
{
	_client.Update(0);
	if (_netPlayerID != UNASSIGNED_PLAYER_INDEX)
		_currentPlayerIDforUI = _netPlayerID;
}

void Shpila::finalize()
{
	_hud.finalize(this);
	SAFE_RELEASE(_scene);
    SAFE_RELEASE(_font);
	freedActorsData();
}

void Shpila::drawSplash(void* param)
{
    clear(CLEAR_COLOR_DEPTH, Vector4(0, 0, 0, 1), 1.0f, 0);
    SpriteBatch* batch = SpriteBatch::create("res/logo_powered_white.png");
    batch->start();
    batch->draw(getWidth() * 0.5f, getHeight() * 0.5f, 0.0f, 512.0f, 512.0f, 0.0f, 1.0f, 1.0f, 0.0f, Vector4::one(), true);
    batch->finish();
    SAFE_DELETE(batch);
}

bool Shpila::drawScene(Node* node, bool transparent)
{
	if (node->isEnabled() && node->getDrawable() && (transparent == node->hasTag("transparent")))
		node->getDrawable()->draw(_wireframe);
    return true;
}

void Shpila::updateMenuButtons()
{
	char buff[100];
	sprintf(buff, "Ping-%d", _ping);
	((Label*)_hud.form()->getControl("PingLabel"))->setText(buff);

	char buff2[100];
	sprintf(buff2, "Energy-%d", getActivePlayer()->MainResource);
	((Label*)_hud.form()->getControl("mainresource"))->setText(buff2);

	char buff3[100];
	int rTime = (RESPAWN_TIME - _respawnTime) / 1000.0f;
	sprintf(buff3, "Time - %d", rTime);
	((Label*)_hud.form()->getControl("timetospawn"))->setText(buff3);

	char buff4[100];
	sprintf(buff4, "2 + %d%%", getActivePlayer()->getAdditionalResourceIncreasePercent());
	((Label*)_hud.form()->getControl("mainresourceinc"))->setText(buff4);
	
	char buff5[100];
	sprintf(buff5, "extractor building: %d", getActivePlayer()->getExtractorBuildingTime());
	Label* lb = ((Label*)_hud.form()->getControl("extractorbuildtimer"));
	lb->setText(buff5);
	lb->setVisible(getActivePlayer()->isExtractorBuilding());
	

	PlayerObject * player = _manager.Players[_currentPlayerIDforUI];
	Container* container = (Container*)_hud.form()->getControl("units1_column1");
	const std::vector<Control*>& controls = container->getControls();
	int firstInvesible = -1;
	for (int i = 0; i < (int)controls.size(); i++)
	{
		const ActorData& aData = getActorData(controls[i]->getTextTag());
		bool vis = aData.RequireUpgrade <= player->UprgadeLevel;
		controls[i]->setVisible(vis);
		if ((!vis) && (firstInvesible == -1))
		{
			firstInvesible = i;
		}
	}

	Control* buttonUpgrade1 = _hud.form()->getControl("UpgradeLevel1");
	Control* buttonUpgrade2 = _hud.form()->getControl("UpgradeLevel2");
	if (firstInvesible != -1)
	{
		buttonUpgrade1->setVisible(player->UprgadeLevel == 0);
		buttonUpgrade1->setPosition(controls[firstInvesible]->getX(), controls[firstInvesible]->getY());
		buttonUpgrade2->setVisible(player->UprgadeLevel == 1);
		buttonUpgrade2->setPosition(controls[firstInvesible]->getX(), controls[firstInvesible]->getY());
	}
	else
	{
		buttonUpgrade1->setVisible(false);
		buttonUpgrade2->setVisible(false);
	}
}

void Shpila::updateActions(float elapsedTime)
{	
	Vector2 move;
	// Forward motion
	if (getActionState(Actions::MOVE_FORWARD) & Keyboard::KEY_STATE_PRESS_SET)
	{
		move.y = 1;
	}
	else if (getActionState(Actions::MOVE_BACKWARD) & Keyboard::KEY_STATE_PRESS_SET)
	{
		move.y = -1;
	}
	// Strafing
	if (getActionState(Actions::MOVE_LEFT) & Keyboard::KEY_STATE_PRESS_SET)
	{
		move.x = -1;
	}
	else if (getActionState(Actions::MOVE_RIGHT) & Keyboard::KEY_STATE_PRESS_SET)
	{
		move.x = 1;
	}
	move.normalize();
	if (!move.isZero())
	{
		_activeCamera->moveForward(_cameraVelocityFactor * elapsedTime * move.y / 20.0f);
		_activeCamera->moveLeft(_cameraVelocityFactor * elapsedTime * move.x / 20.0f);
	}

	// Up and down
	if (getActionState(Actions::MOVE_UP) & Keyboard::KEY_STATE_PRESS_SET)
	{
		_activeCamera->moveUp(_cameraVelocityFactor * elapsedTime * UP_DOWN_SPEED / 1000.0f);
	}
	else if (getActionState(Actions::MOVE_DOWN) & Keyboard::KEY_STATE_PRESS_SET)
	{
		_activeCamera->moveDown(_cameraVelocityFactor * elapsedTime * UP_DOWN_SPEED / 1000.0f);
	}

	if (getActionState(Actions::SWITCH_CAMERA) & Keyboard::KEY_STATE_CLICK_SET)
	{
		if (_activeCamera == &_fpCamera)
		{
			_scene->setActiveCamera(_activePlayerCamera->getCamera());
			_activeCamera = _activePlayerCamera;
			_hud.form()->getControl("Player1")->setVisible(true);
		}
		else
		{
			_scene->setActiveCamera(_fpCamera.getCamera());
			_activeCamera = &_fpCamera;
			_hud.form()->getControl("Player1")->setVisible(false);
		}
	}

	if (getActionState(Actions::SHIFT) & Keyboard::KEY_STATE_PRESS_SET)
	{
		_cameraVelocityFactor += (float)_wheelDelta / 100.0f;
		if (_cameraVelocityFactor < 0.05f)
			_cameraVelocityFactor = 0.05f;
		if (_cameraVelocityFactor > 5.0f)
			_cameraVelocityFactor = 5.0f;
	}
	else
	{
		float fov = _activeCamera->getCamera()->getFieldOfView();
		_activeCamera->getCamera()->setFieldOfView(fov + (float)_wheelDelta / 5.0f);
		_wheelDelta = 0;
	}

	if (getActionState(Actions::PLACE_UNIT) & Keyboard::KEY_STATE_CLICK_SET)
	{		
		PlaceUnit(_mouseX, _mouseY, getActionState(Actions::SHIFT) & Keyboard::KEY_STATE_PRESS_SET);
	}

	if ((getActionState(Actions::CANCEL_PLACE_UNIT) & Keyboard::KEY_STATE_CLICK_SET) || (getActionState(Actions::ESC) & Keyboard::KEY_STATE_CLICK_SET))
	{
		getActivePlayer()->CancelCreateWarrior();
	}
	
}

void Shpila::updateKeyStates()
{
	for (std::map<Keyboard::Key, Keyboard::KeyState>::iterator it = _keyMap.begin(); it != _keyMap.end(); it++)
	{
		switch (it->second)
		{
		case Keyboard::KEY_STATE_PRESS:
			it->second = Keyboard::KEY_STATE_PRESSED;
			break;
		case Keyboard::KEY_STATE_CLICK:
		case Keyboard::KEY_STATE_RELEASE:
			it->second = Keyboard::KEY_STATE_RELEASED;
			break;
		}
	}
}

bool Shpila::PlaceUnit(int x, int y, bool continuous)
{
	//Valuable<Vector3>(ProjectToZeroPlane(_activeCamera->getCamera(), x, y))
	return getActivePlayer()->CreateWarrior(continuous);
}

void Shpila::loadActionMap()
{
	_actionsMap[Actions::MOVE_FORWARD] = Keyboard::KEY_W;
	_actionsMap[Actions::MOVE_BACKWARD] = Keyboard::KEY_S;
	_actionsMap[Actions::MOVE_LEFT] = Keyboard::KEY_A;
	_actionsMap[Actions::MOVE_RIGHT] = Keyboard::KEY_D;
	_actionsMap[Actions::MOVE_UP] = Keyboard::KEY_E;
	_actionsMap[Actions::MOVE_DOWN] = Keyboard::KEY_Q;
	_actionsMap[Actions::SWITCH_CAMERA] = Keyboard::KEY_SPACE;	
	_actionsMap[Actions::PLACE_UNIT] = Keyboard::KEY_MOUSE_LEFT;
	_actionsMap[Actions::CANCEL_PLACE_UNIT] = Keyboard::KEY_MOUSE_RIGHT;	
	_actionsMap[Actions::ALT] = Keyboard::KEY_ALT;
	_actionsMap[Actions::SHIFT] = Keyboard::KEY_SHIFT;
	_actionsMap[Actions::CTRL] = Keyboard::KEY_CTRL;
	_actionsMap[Actions::ESC] = Keyboard::KEY_ESCAPE;

	for (std::map<Actions::Action, Keyboard::Key>::iterator it = _actionsMap.begin(); it != _actionsMap.end(); it++)
	{
		_keyMap[it->second] = Keyboard::KEY_STATE_RELEASED;
	}
}

Keyboard::KeyState Shpila::getActionState(Actions::Action action)
{
	return _keyMap[_actionsMap[action]];
}

void Shpila::setKeyState(int key, bool pressed)
{
	if (pressed)
		_keyMap[(Keyboard::Key)key] = Keyboard::KEY_STATE_PRESS;
	else
	{
		if (_keyMap[(Keyboard::Key)key] == Keyboard::KEY_STATE_PRESS)
			_keyMap[(Keyboard::Key)key] = Keyboard::KEY_STATE_CLICK;
		else
			_keyMap[(Keyboard::Key)key] = Keyboard::KEY_STATE_RELEASE;
	}
}

const Vector3 Shpila::ProjectToZeroPlane(Camera* camera, int x, int y)
{
	Vector3 pointOnPlane(0, 0, 0);
	Ray ray;
	camera->pickRay(Game::getInstance()->getViewport(), x, y, &ray);
	Vector3 normal = Vector3(0.0f, 1.0f, 0.0f);
	const float distance = Vector3::dot(pointOnPlane, normal);
	Plane plane(normal, -distance);

	Vector3 point;
	float collisionDistance = ray.intersects(plane);
	if (collisionDistance != Ray::INTERSECTS_NONE)
	{
		point.set(ray.getOrigin() + collisionDistance*ray.getDirection());
		return point;
	}
	else
		return Vector3::zero();
}


//shpila->_manager.Players[1]->CreateWarrior(names[(int)min(2.0f, rnd(0.0f, 3.0f))].c_str());

void Shpila::CreateUnit(Game* game, Control* control)
{
	((Shpila*)game)->getActivePlayer()->setCreateWarior(control->getTextTag());
}

void Shpila::SwitchPlayer(Game* game, Control* control)
{
	Shpila* shpila = (Shpila*)game;
	if (shpila->_netPlayerID == UNASSIGNED_PLAYER_INDEX)
	{
		shpila->_currentPlayerIDforUI = (!strcmp(control->getTextTag(), "player1")) ? 0 : 1;
		shpila->_activePlayerCamera = &shpila->_CameraPlayer[shpila->_currentPlayerIDforUI];
	}

}

void Shpila::ShowTunes(Game* game, Control* control)
{
	Shpila* shpila = (Shpila*)game;
	shpila->_hud.form()->getControl("Tune")->setVisible(!shpila->_hud.form()->getControl("Tune")->isVisible());
}

void Shpila::ShowConnection(Game* game, Control* control)
{
	Shpila* shpila = (Shpila*)game;
	shpila->_hud.form()->getControl("Connection")->setVisible(!shpila->_hud.form()->getControl("Connection")->isVisible());
}

void Shpila::CameraFoVPlus(Game* game, Control* control)
{
	Shpila* shpila = (Shpila*)game;
	float fov = shpila->_activeCamera->getCamera()->getFieldOfView();
	shpila->_activeCamera->getCamera()->setFieldOfView(fov + 1.0f);
	char buff[100];
	sprintf(buff, "FoV-%0.0f", fov + 1.0f);
	((Label*)shpila->_hud.form()->getControl("CameraFoVTitle"))->setText(buff);
}
void Shpila::CameraFoVMinus(Game* game, Control* control)
{
	Shpila* shpila = (Shpila*)game;
	float fov = shpila->_activeCamera->getCamera()->getFieldOfView();
	shpila->_activeCamera->getCamera()->setFieldOfView(fov - 1.0f);
	char buff[100];
	sprintf(buff, "FoV-%0.0f", fov - 1.0f);
	((Label*)shpila->_hud.form()->getControl("CameraFoVTitle"))->setText(buff);
}

void Shpila::SetCameraFree(Game* game, Control* control)
{
	Shpila* shpila = (Shpila*)game;
	shpila->_freeCamera = true;

}
void Shpila::SetCameraLocked(Game* game, Control* control)
{
	Shpila* shpila = (Shpila*)game;
	shpila->_freeCamera = false;
}

void Shpila::ConnectToServer(Game* game, Control* control)
{
	Shpila* shpila = (Shpila*)game;
	const char* host = ((TextBox*)shpila->_hud.form()->getControl("ConnectionPath"))->getText();
	shpila->_client.Startup(&shpila->_manager);
	shpila->_client.Connect(host, 7777, "Jezuka", "");
}

void Shpila::Upgrade(Game* game, Control* control)
{
	Shpila* shpila = (Shpila*)game;
	int ul = shpila->_manager.Players[shpila->_currentPlayerIDforUI]->UprgadeLevel;
	if (ul < 2)
	{
		shpila->_manager.Players[shpila->_currentPlayerIDforUI]->UprgadeLevel += 1;
	}
}

void Shpila::AddExtractor(Game* game, Control* control)
{
	Shpila* shpila = (Shpila*)game;
	shpila->getActivePlayer()->addExtractor();
}

void loadCamera(TiXmlNode *node, TargetCamera& camera)
{
	Quaternion qRotX;
	Quaternion qRotY;
	Vector3 pos;
	qRotX.x = (float)atof(node->FirstChildElement("rx0")->GetText());
	qRotX.y = (float)atof(node->FirstChildElement("rx1")->GetText());
	qRotX.z = (float)atof(node->FirstChildElement("rx2")->GetText());
	qRotX.w = (float)atof(node->FirstChildElement("rx3")->GetText());
	qRotY.x = (float)atof(node->FirstChildElement("ry0")->GetText());
	qRotY.y = (float)atof(node->FirstChildElement("ry1")->GetText());
	qRotY.z = (float)atof(node->FirstChildElement("ry2")->GetText());
	qRotY.w = (float)atof(node->FirstChildElement("ry3")->GetText());
	pos.x = (float)atof(node->FirstChildElement("posx")->GetText());
	pos.y = (float)atof(node->FirstChildElement("posy")->GetText());
	pos.z = (float)atof(node->FirstChildElement("posz")->GetText());
	camera.getRootNode()->getFirstChild()->setRotation(qRotX);
	camera.getRootNode()->setRotation(qRotY);
	camera.getRootNode()->setTranslation(pos);
	camera.getCamera()->setFieldOfView((float)atof(node->FirstChildElement("fov")->GetText()));
}

void Shpila::loadSetting(Game* game, Control* control)
{
	Shpila* shpila = (Shpila*)game;
	Quaternion qRotX;
	Quaternion qRotY;
	Vector3 pos;

	TiXmlDocument doc;
	doc.SetTabSize(8);
	doc.LoadFile("res/settings.xml");

	TiXmlNode *node = doc.FirstChild("camera");
	loadCamera(node, shpila->_fpCamera);

	node = doc.FirstChild("cameraPlayer1");
	loadCamera(node, shpila->_CameraPlayer[0]);

	node = doc.FirstChild("cameraPlayer2");
	loadCamera(node, shpila->_CameraPlayer[1]);
}

void AddFloatElement(TiXmlElement * rootElement, char* name, float value)
{
	char buff[100];
	TiXmlElement * element;
	element = new TiXmlElement(name);
	sprintf(buff, "%f", value);
	element->LinkEndChild(new TiXmlText(buff));
	rootElement->LinkEndChild(element);
}

void saveCamera(TiXmlElement * rootElement, TargetCamera& camera)
{
	Quaternion qRotX = camera.getRootNode()->getFirstChild()->getRotation();
	Quaternion qRotY = camera.getRootNode()->getRotation();
	Vector3 pos = camera.getRootNode()->getTranslation();
	AddFloatElement(rootElement, "rx0", qRotX.x);
	AddFloatElement(rootElement, "rx1", qRotX.y);
	AddFloatElement(rootElement, "rx2", qRotX.z);
	AddFloatElement(rootElement, "rx3", qRotX.w);
	AddFloatElement(rootElement, "ry0", qRotY.x);
	AddFloatElement(rootElement, "ry1", qRotY.y);
	AddFloatElement(rootElement, "ry2", qRotY.z);
	AddFloatElement(rootElement, "ry3", qRotY.w);
	AddFloatElement(rootElement, "posx", pos.x);
	AddFloatElement(rootElement, "posy", pos.y);
	AddFloatElement(rootElement, "posz", pos.z);
	AddFloatElement(rootElement, "fov", camera.getCamera()->getFieldOfView());
}

void Shpila::saveSetting(Game* game, Control* control)
{
	Shpila* shpila = (Shpila*)game;
	TiXmlDocument doc;

	doc.SetTabSize(8);
	TiXmlDeclaration * decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);

	TiXmlElement * element = new TiXmlElement("camera");
	doc.LinkEndChild(element);
	saveCamera(element, shpila->_fpCamera);

	element = new TiXmlElement("cameraPlayer1");
	doc.LinkEndChild(element);
	saveCamera(element, shpila->_CameraPlayer[0]);

	element = new TiXmlElement("cameraPlayer2");
	doc.LinkEndChild(element);
	saveCamera(element, shpila->_CameraPlayer[1]);

	doc.SaveFile("res/settings.xml");
}

void Shpila::AddEnergy(Game* game, Control* control)
{
	Shpila* shpila = (Shpila*)game;
	shpila->getActivePlayer()->MainResource += 1000;
}

void Shpila::PauseClick(Game* game, Control* control)
{
	if (game->getState() == Game::PAUSED)
		game->resume();
	else
		game->pause();
}

void Shpila::update(float elapsedTime)
{
	_totalTime += elapsedTime;
	_respawnTime += elapsedTime;
	if (!EMPTY)
	{
		if (_respawnTime > RESPAWN_TIME)
		{
			Respawn = true;
			_respawnTime = 0.0f;
		}
		OpenSteer::OpenSteerManager::update(_totalTime, elapsedTime);
		updatePlayers(elapsedTime);
		Respawn = false;

		updateNetwork();
		updateMenuButtons();
		updateActions(elapsedTime);
		updateKeyStates();
	}
}

/*ParticleEmitter* emitter = dynamic_cast<ParticleEmitter*>(_particleEmitterSunNode->getDrawable());
if (emitter)
emitter->update(elapsedTime);
ParticleEmitter* emitter = dynamic_cast<ParticleEmitter*>(_particleEmitterStarsNode->getDrawable());
if (emitter)
emitter->update(elapsedTime);*/

void Shpila::render(float elapsedTime)
{
    // Clear the color and depth buffers.	
	clear(CLEAR_COLOR_DEPTH, Vector4(0.41f, 0.48f, 0.54f, 1.0f), 1.0f, 0);
	
	if (!EMPTY)
	{
		// Draw our scene, with separate passes for opaque and transparent objects.
		_scene->visit(this, &Shpila::drawScene, false);
		_scene->visit(this, &Shpila::drawScene, true);

		for (std::vector<PlayerObject*>::iterator it = _manager.Players.begin(); it != _manager.Players.end(); ++it)
		{
			(*it)->render();
		}

		_hud.render(this, elapsedTime);
		// Draw physics debug
		//if (_physicsDebug)
		//    getPhysicsController()->drawDebug(_scene->getActiveCamera()->getViewProjectionMatrix());*/

		/*

		// Draw FPS
		_font->start();
		char fps[32];
		sprintf(fps, "%d", getFrameRate());
		_font->drawText(fps, 5, 5, Vector4(1,1,0,1), 20);
		_font->finish();*/
	}
}

void Shpila::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
	switch (evt)
	{
	case Touch::TOUCH_PRESS:
		if (x < 75 && y < 50)
		{
			// Toggle Vsync if the user touches the top left corner
			setVsync(!isVsync());
		}
		_prevX = x;
		_prevY = y;
		break;
	case Touch::TOUCH_RELEASE:
		_prevX = 0;
		_prevY = 0;
		break;
	case Touch::TOUCH_MOVE:
	{
		int deltaX = x - _prevX;
		int deltaY = y - _prevY;
		_prevX = x;
		_prevY = y;
		float pitch = -MATH_DEG_TO_RAD(deltaY * 0.5f);
		float yaw = MATH_DEG_TO_RAD(deltaX * 0.5f);
		if (_freeCamera)
			_activeCamera->rotate(yaw, pitch);
		else
		{
			_activeCamera->setPosition(_activeCamera->getPosition() - deltaX * 0.07f * _battleFieldDirection);
		}
		break;
	}
	};
}

void Shpila::keyEvent(Keyboard::KeyEvent evt, int key)
{	
	switch (evt)
	{
	case Keyboard::KEY_PRESS:
		setKeyState(key, true);
		break;
	case Keyboard::KEY_RELEASE:
		setKeyState(key, false);
		break;
	}
}

bool Shpila::mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta)
{
	switch (evt)
	{
	case Mouse::MOUSE_WHEEL:
		_wheelDelta += wheelDelta;
		return true;
	case Mouse::MOUSE_MOVE:
		_mouseX = x;
		_mouseY = y;
		return false;
	//left mouse button
	case Mouse::MOUSE_PRESS_LEFT_BUTTON:
		setKeyState(Keyboard::KEY_MOUSE_LEFT, true);
		return false;
	case Mouse::MOUSE_RELEASE_LEFT_BUTTON:
		setKeyState(Keyboard::KEY_MOUSE_LEFT, false);
		return true;
	//right mouse button
	case Mouse::MOUSE_PRESS_RIGHT_BUTTON:
		setKeyState(Keyboard::KEY_MOUSE_RIGHT, true);
		return false;
	case Mouse::MOUSE_RELEASE_RIGHT_BUTTON:
		setKeyState(Keyboard::KEY_MOUSE_RIGHT, false);
		return true;
	//middle mouse button
	case Mouse::MOUSE_PRESS_MIDDLE_BUTTON:
		setKeyState(Keyboard::KEY_MOUSE_MIDDLE, true);
		return false;
	case Mouse::MOUSE_RELEASE_MIDDLE_BUTTON:
		setKeyState(Keyboard::KEY_MOUSE_MIDDLE, false);
		return true;
	}
	return false;
}

/*
#ifndef BASEGAMEOBJECT_H_
#define BASEGAMEOBJECT_H_

#include "gameplay.h"
using namespace gameplay;



#endif


*/