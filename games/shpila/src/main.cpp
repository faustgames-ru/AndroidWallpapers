#include "main.h"

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

static const unsigned int MOVE_FORWARD = 1;
static const unsigned int MOVE_BACKWARD = 2;
static const unsigned int MOVE_LEFT = 4;
static const unsigned int MOVE_RIGHT = 8;
static const unsigned int MOVE_UP = 16;
static const unsigned int MOVE_DOWN = 32;

static const float MOVE_SPEED = 15.0f;
static const float UP_DOWN_SPEED = 10.0f;

static const bool EMPTY = false;

std::string ACTOR_TYPE[] = { "irbaga", "budfoor", "barar", "albiria", "chasovoy" };

Shpila::Shpila()
    : _font(NULL), _scene(), _character(NULL), _characterNode(NULL), _characterMeshNode(NULL), _characterShadowNode(NULL), _basketballNode(NULL),
      _animation(NULL), _rotateX(0), _materialParameterAlpha(NULL),
	  _keyFlags(0), _physicsDebug(false), _wireframe(false), _hasBall(false), _applyKick(false), _gamepad(NULL), _camera(), _fpCamera(), _freeCamera(true), _battleFieldDirection(), _particleEmitterSunNode(NULL), _particleEmitterStarsNode(NULL),
	  _hud(), _totalTime(0.0), _manager(), _ping(0), _netPlayerID(-1)
{
    _buttonPressed = new bool[2];
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

		const float FoV = 60.0f;

		_hud.initialize(this, _scene);
		_hud.bind("Player1_Auto", Control::Listener::CLICK, Player1_Auto_Click);
		_hud.bind("Player2_Auto", Control::Listener::CLICK, Player2_Auto_Click);
		_hud.bind("Player1_New_Irbaga", Control::Listener::CLICK, Player1_New_Irbaga);
		_hud.bind("Player2_New_Irbaga", Control::Listener::CLICK, Player2_New_Irbaga);
		_hud.bind("Player1_New_Budfoor", Control::Listener::CLICK, Player1_New_Budfoor);
		_hud.bind("Player2_New_Budfoor", Control::Listener::CLICK, Player2_New_Budfoor);
		_hud.bind("Player1_New_Barar", Control::Listener::CLICK, Player1_New_Barar);
		_hud.bind("Player2_New_Barar", Control::Listener::CLICK, Player2_New_Barar);
		_hud.bind("Player1_New_Albiria", Control::Listener::CLICK, Player1_New_Albiria);
		_hud.bind("Player2_New_Albiria", Control::Listener::CLICK, Player2_New_Albiria);
		_hud.bind("Player1_New_Chasovoy", Control::Listener::CLICK, Player1_New_Chasovoy);
		_hud.bind("Player2_New_Chasovoy", Control::Listener::CLICK, Player2_New_Chasovoy);

		_hud.bind("CameraFoVPlus", Control::Listener::CLICK, CameraFoVPlus);
		_hud.bind("CameraFoVMinus", Control::Listener::CLICK, CameraFoVMinus);
		char buff[100];
		sprintf(buff, "FoV-%0.0f", FoV);
		((Label*)_hud.form()->getControl("CameraFoVTitle"))->setText(buff);
		_hud.bind("CameraFree", Control::Listener::CLICK, SetCameraFree);
		_hud.bind("CameraLocked", Control::Listener::CLICK, SetCameraLocked);
		

		// Load scene.
		_scene = Scene::load("res/common/box.scene");

		// Update the aspect ratio for our scene's camera to match the current device resolution.
		_scene->getActiveCamera()->setAspectRatio(getAspectRatio());

		//static camera
		Vector3 cameraPosition(0.0f, 1.8f, -4.4f);
		_fpCamera.initialize(0.01f, 1000.0f);
		_fpCamera.setPosition(cameraPosition);
		Matrix rot;
		Matrix::createLookAt(0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,
			0.0f, 1.0f, 0.5f, &rot);
		_fpCamera.setRotation(rot);
		_scene->addNode(_fpCamera.getRootNode());

		//setup camera
		_fpCamera.getCamera()->setFieldOfView(FoV);
		_scene->setActiveCamera(_fpCamera.getCamera());
		_scene->setAmbientColor(0.25f, 0.25f, 0.25f);

		_manager.setScene(_scene);
		loadCharacters();
		initPlayers();

		// Initialize scene.
		_scene->visit(this, &Shpila::initializeNodeMaterials);

		_gamepad = getGamepad(0);

		OpenSteer::OpenSteerManager::initialize();

		_client.Startup(&_manager);
		_client.Connect("localhost", 7777, "Jezuka", "");
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
	_manager.addUnit("res/common/irbaga.scene", "irbaga", IrbagaWarrior::constructor);
	_manager.addUnit(NULL, "tower", HiddenObject::constructor);
	_manager.addUnit("res/common/budfoor.scene", "budfoor", BudfoorWarrior::constructor);
	_manager.addUnit("res/common/barar.scene", "barar", BararWarrior::constructor);
	_manager.addUnit("res/common/albiria.scene", "albiria", AlbiriaWarrior::constructor);
	_manager.addUnit("res/common/chasovoy.scene", "chasovoy", ChasovoyWarrior::constructor);
	_manager.initUnits();
}

void Shpila::initPlayers()
{
	Vector3 p1 = Vector3(9.0f, 0.0f, -9.0f);
	Vector3 p2 = Vector3(-9.0f, 0.0f, 9.0f);
	_battleFieldDirection = Vector3(p2 - p1).normalize();		
	_manager.Players.push_back(new Player(_manager, 0, p1));
	_manager.Players.push_back(new Player(_manager, 1, p2));
}

void Shpila::updatePlayers(float time)
{
	for (std::vector<Player*>::iterator it = _manager.Players.begin(); it != _manager.Players.end(); ++it)
	{
		(*it)->update(time);
	}
}

void Shpila::finalize()
{
	_hud.finalize(this);
	SAFE_RELEASE(_scene);
    SAFE_RELEASE(_font);
    SAFE_DELETE_ARRAY(_buttonPressed);
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


//shpila->_manager.Players[1]->CreateWarrior(names[(int)min(2.0f, rnd(0.0f, 3.0f))].c_str());

void Shpila::Player1_Auto_Click(Game* game)
{
	//Shpila* shpila = (Shpila*)game;
	//shpila->_manager.Players[0]->AutoPlay = !shpila->_manager.Players[0]->AutoPlay;
}

void Shpila::Player2_Auto_Click(Game* game)
{
	//Shpila* shpila = (Shpila*)game;
	//shpila->_manager.Players[1]->AutoPlay = !shpila->_manager.Players[1]->AutoPlay;
}

void Shpila::Player1_New_Irbaga(Game* game)
{
	Shpila* shpila = (Shpila*)game;
	if ((shpila->_netPlayerID >= 0) && (shpila->_netPlayerID != 0))
		return;
	shpila->_manager.Players[0]->CreateWarrior(ACTOR_TYPE[0].c_str());
}

void Shpila::Player2_New_Irbaga(Game* game)
{
	Shpila* shpila = (Shpila*)game;
	if ((shpila->_netPlayerID >= 0) && (shpila->_netPlayerID != 1))
		return;
	shpila->_manager.Players[1]->CreateWarrior(ACTOR_TYPE[0].c_str());
}

void Shpila::Player1_New_Budfoor(Game* game)
{
	Shpila* shpila = (Shpila*)game;
	if ((shpila->_netPlayerID >= 0) && (shpila->_netPlayerID != 0))
		return;
	shpila->_manager.Players[0]->CreateWarrior(ACTOR_TYPE[1].c_str());
}
void Shpila::Player2_New_Budfoor(Game* game)
{
	Shpila* shpila = (Shpila*)game;
	if ((shpila->_netPlayerID >= 0) && (shpila->_netPlayerID != 1))
		return;
	shpila->_manager.Players[1]->CreateWarrior(ACTOR_TYPE[1].c_str());
}
void Shpila::Player1_New_Barar(Game* game)
{
	Shpila* shpila = (Shpila*)game;
	if ((shpila->_netPlayerID >= 0) && (shpila->_netPlayerID != 0))
		return;
	shpila->_manager.Players[0]->CreateWarrior(ACTOR_TYPE[2].c_str());
}
void Shpila::Player2_New_Barar(Game* game)
{
	Shpila* shpila = (Shpila*)game;
	if ((shpila->_netPlayerID >= 0) && (shpila->_netPlayerID != 1))
		return;
	shpila->_manager.Players[1]->CreateWarrior(ACTOR_TYPE[2].c_str());
}

void Shpila::Player1_New_Albiria(Game* game)
{
	Shpila* shpila = (Shpila*)game;
	if ((shpila->_netPlayerID >= 0) && (shpila->_netPlayerID != 0))
		return;
	shpila->_manager.Players[0]->CreateWarrior(ACTOR_TYPE[3].c_str());
}

void Shpila::Player2_New_Albiria(Game* game)
{
	Shpila* shpila = (Shpila*)game;
	if ((shpila->_netPlayerID >= 0) && (shpila->_netPlayerID != 1))
		return;
	shpila->_manager.Players[1]->CreateWarrior(ACTOR_TYPE[3].c_str());
}

void Shpila::Player1_New_Chasovoy(Game* game)
{
	Shpila* shpila = (Shpila*)game;
	if ((shpila->_netPlayerID >= 0) && (shpila->_netPlayerID != 0))
		return;
	shpila->_manager.Players[0]->CreateWarrior(ACTOR_TYPE[4].c_str());
}
void Shpila::Player2_New_Chasovoy(Game* game)
{
	Shpila* shpila = (Shpila*)game;
	shpila->_manager.Players[1]->CreateWarrior(ACTOR_TYPE[4].c_str());
}

void Shpila::CameraFoVPlus(Game* game)
{
	Shpila* shpila = (Shpila*)game;
	float fov = shpila->_fpCamera.getCamera()->getFieldOfView();
	shpila->_fpCamera.getCamera()->setFieldOfView(fov + 1.0f);
	char buff[100];
	sprintf(buff, "FoV-%0.0f", fov + 1.0f);
	((Label*)shpila->_hud.form()->getControl("CameraFoVTitle"))->setText(buff);
}
void Shpila::CameraFoVMinus(Game* game)
{
	Shpila* shpila = (Shpila*)game;
	float fov = shpila->_fpCamera.getCamera()->getFieldOfView();
	shpila->_fpCamera.getCamera()->setFieldOfView(fov - 1.0f);
	char buff[100];
	sprintf(buff, "FoV-%0.0f", fov - 1.0f);
	((Label*)shpila->_hud.form()->getControl("CameraFoVTitle"))->setText(buff);
}

void Shpila::SetCameraFree(Game* game)
{
	Shpila* shpila = (Shpila*)game;
	shpila->_freeCamera = true;

}
void Shpila::SetCameraLocked(Game* game)
{
	Shpila* shpila = (Shpila*)game;
	shpila->_freeCamera = false;
}

void Shpila::update(float elapsedTime)
{
	_totalTime += elapsedTime;
	if (!EMPTY)
	{
		OpenSteer::OpenSteerManager::update(_totalTime, elapsedTime);
		updatePlayers(elapsedTime);

		/*ParticleEmitter* emitter = dynamic_cast<ParticleEmitter*>(_particleEmitterSunNode->getDrawable());
		if (emitter)
			emitter->update(elapsedTime);
		ParticleEmitter* emitter = dynamic_cast<ParticleEmitter*>(_particleEmitterStarsNode->getDrawable());
		if (emitter)
			emitter->update(elapsedTime);*/
		_client.Update(0);

		char buff[100];
		sprintf(buff, "Ping-%d", _ping);
		((Label*)_hud.form()->getControl("PingLabel"))->setText(buff);

		Vector2 move;
		if (_moveFlags != 0)
		{
			// Forward motion
			if (_moveFlags & MOVE_FORWARD)
			{
				move.y = 1;
			}
			else if (_moveFlags & MOVE_BACKWARD)
			{
				move.y = -1;
			}
			// Strafing
			if (_moveFlags & MOVE_LEFT)
			{
				move.x = -1;
			}
			else if (_moveFlags & MOVE_RIGHT)
			{
				move.x = 1;
			}
			move.normalize();

			// Up and down
			if (_moveFlags & MOVE_UP)
			{
				_fpCamera.moveUp(elapsedTime * UP_DOWN_SPEED / 1000.0f);
			}
			else if (_moveFlags & MOVE_DOWN)
			{
				_fpCamera.moveDown(elapsedTime * UP_DOWN_SPEED / 1000.0f);
			}

			if (!move.isZero())
			{
				_fpCamera.moveForward(elapsedTime * move.y / 100.0f);
				_fpCamera.moveLeft(elapsedTime * move.x / 100.0f);
			}

		}

	}
}

void Shpila::render(float elapsedTime)
{
    // Clear the color and depth buffers.	
	clear(CLEAR_COLOR_DEPTH, Vector4(0.41f, 0.48f, 0.54f, 1.0f), 1.0f, 0);
	
	if (!EMPTY)
	{
		// Draw our scene, with separate passes for opaque and transparent objects.
		_scene->visit(this, &Shpila::drawScene, false);
		_scene->visit(this, &Shpila::drawScene, true);

		_hud.render(this, elapsedTime);
		// Draw physics debug
		//if (_physicsDebug)
		//    getPhysicsController()->drawDebug(_scene->getActiveCamera()->getViewProjectionMatrix());*/

		/*_gamepad->draw();

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
								_fpCamera.rotate(yaw, pitch);
							  else
							  {
								  _fpCamera.setPosition(_fpCamera.getPosition() - deltaX * 0.01f * _battleFieldDirection);
							  }
							  break;
	}
	};
}

void Shpila::keyEvent(Keyboard::KeyEvent evt, int key)
{
	if (evt == Keyboard::KEY_PRESS)
	{
		switch (key)
		{
		case Keyboard::KEY_W:
			_moveFlags |= MOVE_FORWARD;
			break;
		case Keyboard::KEY_S:
			_moveFlags |= MOVE_BACKWARD;
			break;
		case Keyboard::KEY_A:
			_moveFlags |= MOVE_LEFT;
			break;
		case Keyboard::KEY_D:
			_moveFlags |= MOVE_RIGHT;
			break;

		case Keyboard::KEY_Q:
			_moveFlags |= MOVE_DOWN;
			break;
		case Keyboard::KEY_E:
			_moveFlags |= MOVE_UP;
			break;
		case Keyboard::KEY_PG_UP:
			_fpCamera.rotate(0, MATH_PIOVER4);
			break;
		case Keyboard::KEY_PG_DOWN:
			_fpCamera.rotate(0, -MATH_PIOVER4);
			break;
		}
	}
	else if (evt == Keyboard::KEY_RELEASE)
	{
		switch (key)
		{
		case Keyboard::KEY_W:
			_moveFlags &= ~MOVE_FORWARD;
			break;
		case Keyboard::KEY_S:
			_moveFlags &= ~MOVE_BACKWARD;
			break;
		case Keyboard::KEY_A:
			_moveFlags &= ~MOVE_LEFT;
			break;
		case Keyboard::KEY_D:
			_moveFlags &= ~MOVE_RIGHT;
			break;
		case Keyboard::KEY_Q:
			_moveFlags &= ~MOVE_DOWN;
			break;
		case Keyboard::KEY_E:
			_moveFlags &= ~MOVE_UP;
			break;
		}
	}
}

bool Shpila::mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta)
{
	switch (evt)
	{
	case Mouse::MOUSE_WHEEL:
		_fpCamera.moveForward(wheelDelta * MOVE_SPEED / 100.0f);
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