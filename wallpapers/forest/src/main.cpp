#include "main.h"

// Declare our game instance
AsteroidsTest game;

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

AsteroidsTest::AsteroidsTest()
    : _font(NULL), _scene(NULL), _character(NULL), _characterNode(NULL), _characterMeshNode(NULL), _characterShadowNode(NULL), _basketballNode(NULL),
      _animation(NULL), _currentClip(NULL), _jumpClip(NULL), _kickClip(NULL), _rotateX(0), _materialParameterAlpha(NULL),
      _keyFlags(0), _physicsDebug(false), _wireframe(false), _hasBall(false), _applyKick(false), _gamepad(NULL)
{
    _buttonPressed = new bool[2];
}

void AsteroidsTest::restoreDeviceObjects()
{
	Game::restoreDeviceObjects();
	if (isInitialized())
	{
		_scene->visit(this, &AsteroidsTest::initializeScene);
	}
}

void AsteroidsTest::initialize()
{
    // Enable multi-touch (only affects devices that support multi-touch).
    setMultiTouch(true);

    // Display the gameplay splash screen for at least 1 second.
	//displayScreen(this, &AsteroidsTest::drawSplash, NULL, 1000L); 

    // Load the font.
    _font = Font::create("res/ui/arial.gpb");

    // Load scene.
    _scene = Scene::load("res/common/solarsystem.scene");

    // Update the aspect ratio for our scene's camera to match the current device resolution.
    _scene->getActiveCamera()->setAspectRatio(getAspectRatio());

	Vector3 cameraPosition(0.0f, 0.35f, 0.0f);
	/*if (Camera* camera = _scene->getActiveCamera())
	{
		camera->getNode()->getTranslation(&cameraPosition);
	}*/

	_fpCamera.initialize(0.01f, 50.0f);
	_fpCamera.setPosition(cameraPosition);
	//_scene->addNode(_fpCamera.getRootNode());
	Matrix rot; 
	Matrix::createLookAt(0.0f, 0.0f, 0.0f,   
						 0.0f, 1.0f, 0.0f,   
						 0.0f, 0.0f, 1.0f, &rot);
	_fpCamera.setRotation(rot);
	_fpCamera.rotate(MATH_PI, 0.0f);
	_scene->findNode("earth")->addChild(Node::create("earthcamerahelper"));
	_fpCamera.setTargetNode(_scene->findNode("earthcamerahelper"));
	_scene->setActiveCamera(_fpCamera.getCamera());
	_scene->setAmbientColor(0.1f, 0.1f, 0.1f);
    
    // Initialize the physics character.
    //initializeCharacter();
	//initializeChair();
	//initializeAsteroids();
	initializeSolarSystem();

    // Initialize scene.
	_scene->visit(this, &AsteroidsTest::initializeScene);

    _gamepad = getGamepad(0);
}

bool AsteroidsTest::initializeScene(Node* node)
{
    Model* model = node->getModel();
    if (model)
    {
		if (model->getMeshPartCount() > 1)
		{
			for (int i = 0; i < model->getMeshPartCount(); i++)
			{
				initializeMaterial(_scene, node, model->getMaterial(i));
			}
		}
		else
		{
			if (model->getMaterial())
			{
				initializeMaterial(_scene, node, model->getMaterial());
			}
		}
    }

    return true;
}

void AsteroidsTest::initializeMaterial(Scene* scene, Node* node, Material* material)
{
    // Bind light shader parameters to dynamic objects only
    if (node->hasTag("dynamic"))
    {
        material->getParameter("u_ambientColor")->bindValue(scene, &Scene::getAmbientColor);
        Node* lightNode = scene->findNode("sun");
        if (lightNode)
        {
            material->getParameter("u_directionalLightColor[0]")->bindValue(lightNode->getLight(), &Light::getColor);
            material->getParameter("u_directionalLightDirection[0]")->bindValue(lightNode, &Node::getForwardVectorView);
			material->getParameter("u_lightColor")->bindValue(lightNode->getLight(), &Light::getColor);
			material->getParameter("u_lightDirection")->bindValue(lightNode, &Node::getForwardVectorView);
			material->getParameter("u_modulateColor")->setVector4(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
			material->getParameter("u_diffuseColor")->setVector4(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
			material->getParameter("u_modulateAlpha")->setFloat(1.0f);
        }
    }
}

void AsteroidsTest::initializeAsteroids()
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

void AsteroidsTest::initializeSolarSystem()
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

void AsteroidsTest::finalize()
{
    SAFE_RELEASE(_scene);
    SAFE_RELEASE(_font);
    SAFE_DELETE_ARRAY(_buttonPressed);
}

void AsteroidsTest::drawSplash(void* param)
{
    clear(CLEAR_COLOR_DEPTH, Vector4(0, 0, 0, 1), 1.0f, 0);
    SpriteBatch* batch = SpriteBatch::create("res/logo_powered_white.png");
    batch->start();
    batch->draw(getWidth() * 0.5f, getHeight() * 0.5f, 0.0f, 512.0f, 512.0f, 0.0f, 1.0f, 1.0f, 0.0f, Vector4::one(), true);
    batch->finish();
    SAFE_DELETE(batch);
}

bool AsteroidsTest::drawScene(Node* node, bool transparent)
{
    if (node->getModel() && (transparent == node->hasTag("transparent")))
        node->getModel()->draw(_wireframe);

    return true;
}

void AsteroidsTest::play(const char* id, bool repeat, float speed)
{
    AnimationClip* clip = _animation->getClip(id);

    // Set clip properties
    clip->setSpeed(speed);
    clip->setRepeatCount(repeat ? AnimationClip::REPEAT_INDEFINITE : 1);

    // Is the clip already playing?
    if (clip == _currentClip && clip->isPlaying())
        return;

    if (_jumpClip->isPlaying() || _kickClip->isPlaying())
    {
        _currentClip = clip;
        return;
    }

    // If a current clip is playing, crossfade into the new one
    if (_currentClip && _currentClip->isPlaying())
    {
        _currentClip->crossFade(clip, 150);
    }
    else
    {
        clip->play();
    }
    _currentClip = clip;
}

void AsteroidsTest::update(float elapsedTime)
{
	float slowFactor = 0.1f;
	_scene->findNode("sunroot")->rotateZ(0.01f * elapsedTime * slowFactor);
	_scene->findNode("rootmercury")->rotateZ(0.001f * elapsedTime * slowFactor);
	_scene->findNode("rootvenus")->rotateZ(0.0015f * elapsedTime * slowFactor);
	_scene->findNode("rootearth")->rotateZ(0.0012f * elapsedTime * slowFactor);
	_scene->findNode("earth")->rotateZ(0.001f * elapsedTime * slowFactor);
	_scene->findNode("earthcamerahelper")->rotateZ(0.0006f * elapsedTime * slowFactor);
	_scene->findNode("rootmars")->rotateZ(0.0016f * elapsedTime * slowFactor);
	_scene->findNode("rootjupiter")->rotateZ(0.0018f * elapsedTime * slowFactor);
	_scene->findNode("rootsaturn")->rotateZ(0.002f * elapsedTime * slowFactor);
	_scene->findNode("rooturanus")->rotateZ(0.003f * elapsedTime * slowFactor);
	_scene->findNode("rootneptune")->rotateZ(0.0018f * elapsedTime * slowFactor);
	_scene->findNode("rootpluto")->rotateZ(0.0019f * elapsedTime * slowFactor);
}

void AsteroidsTest::render(float elapsedTime)
{
    // Clear the color and depth buffers.
    clear(CLEAR_COLOR_DEPTH, Vector4(0.41f, 0.48f, 0.54f, 1.0f), 1.0f, 0);

    // Draw our scene, with separate passes for opaque and transparent objects.
	_scene->visit(this, &AsteroidsTest::drawScene, false);
	_scene->visit(this, &AsteroidsTest::drawScene, true);

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

void AsteroidsTest::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
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
							  _fpCamera.rotate(yaw, pitch);
							  break;
	}
	};
}

void AsteroidsTest::keyEvent(Keyboard::KeyEvent evt, int key)
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

bool AsteroidsTest::mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta)
{
	switch (evt)
	{
	case Mouse::MOUSE_WHEEL:
		_fpCamera.moveForward(wheelDelta * MOVE_SPEED / 2.0f);
		return true;
	}
	return false;
}
