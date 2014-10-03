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

AsteroidsTest::AsteroidsTest()
    : _font(NULL), _scene(NULL), _character(NULL), _characterNode(NULL), _characterMeshNode(NULL), _characterShadowNode(NULL), _basketballNode(NULL),
      _animation(NULL), _currentClip(NULL), _jumpClip(NULL), _kickClip(NULL), _rotateX(0), _materialParameterAlpha(NULL),
      _keyFlags(0), _physicsDebug(false), _wireframe(false), _hasBall(false), _applyKick(false), _gamepad(NULL)
{
    _buttonPressed = new bool[2];
}

void AsteroidsTest::initialize()
{
    // Enable multi-touch (only affects devices that support multi-touch).
    setMultiTouch(true);

    // Display the gameplay splash screen for at least 1 second.
	displayScreen(this, &AsteroidsTest::drawSplash, NULL, 1000L);

    // Load the font.
    _font = Font::create("res/ui/arial.gpb");

    // Load scene.
    _scene = Scene::load("res/common/asteroidstest.scene");

    // Update the aspect ratio for our scene's camera to match the current device resolution.
    _scene->getActiveCamera()->setAspectRatio(getAspectRatio());
    
    // Initialize the physics character.
    //initializeCharacter();
	//initializeChair();
	initializeAsteroids();

    // Create a collision object for the ceiling.
    Node* ceiling = _scene->addNode("ceiling");
    ceiling->setTranslationY(14.5f);
    PhysicsRigidBody::Parameters rbParams;
    rbParams.mass = 0.0f;
    rbParams.friction = 0.5f;
    rbParams.restitution = 0.75f;
    rbParams.linearDamping = 0.025f;
    rbParams.angularDamping = 0.16f;
    ceiling->setCollisionObject(PhysicsCollisionObject::RIGID_BODY, PhysicsCollisionShape::box(Vector3(49.5f, 1.0f, 49.5f)), &rbParams);

    // Initialize scene.
	_scene->visit(this, &AsteroidsTest::initializeScene);

    _gamepad = getGamepad(0);
}

bool AsteroidsTest::initializeScene(Node* node)
{
    Model* model = node->getModel();
    if (model && model->getMaterial())
    {
        initializeMaterial(_scene, node, model->getMaterial());
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

}

void AsteroidsTest::render(float elapsedTime)
{
    // Clear the color and depth buffers.
    clear(CLEAR_COLOR_DEPTH, Vector4(0.41f, 0.48f, 0.54f, 1.0f), 1.0f, 0);

    // Draw our scene, with separate passes for opaque and transparent objects.
	_scene->visit(this, &AsteroidsTest::drawScene, false);
	_scene->visit(this, &AsteroidsTest::drawScene, true);

    // Draw physics debug
    if (_physicsDebug)
        getPhysicsController()->drawDebug(_scene->getActiveCamera()->getViewProjectionMatrix());

    _gamepad->draw();

    // Draw FPS
    _font->start();
    char fps[32];
    sprintf(fps, "%d", getFrameRate());
    _font->drawText(fps, 5, 5, Vector4(1,1,0,1), 20);
    _font->finish();
}