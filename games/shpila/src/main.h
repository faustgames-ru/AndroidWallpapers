#ifndef SHPILA_H_
#define SHPILA_H_

#include "Headers.h"
using namespace gameplay;

/**
 * This is a mesh demo game for rendering Mesh.
 */
class Shpila: public Game
{
public:
    
    /**
     * Constructor.
     */
	Shpila();

	virtual void restoreDeviceObjects();

	bool initializeNodeMaterials(Node* node);

protected:

    /**
     * @see Game::initialize
     */
    void initialize();

    /**
     * @see Game::finalize
     */
    void finalize();

    /**
     * @see Game::update
     */
    void update(float elapsedTime);

    /**
     * @see Game::render
     */
    void render(float elapsedTime);

	bool mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta);
	void keyEvent(Keyboard::KeyEvent evt, int key);
	void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);
private:
    
    void initializeMaterial(Scene* scene, Node* node, Material* material);
	void initializeAsteroids();
	void initializeSolarSystem();
	void loadCharacters();
	void initPlayers();
	void updatePlayers(float time);
    void drawSplash(void* param);
    bool drawScene(Node* node, bool transparent);
	void updateNetwork();
	void updateMenuButtons();

	static void CreateUnit(Game* game, Control* control);
	static void ShowUnitsP1(Game* game, Control* control);
	static void ShowUnitsP2(Game* game, Control* control);
	static void SwitchPlayer(Game* game, Control* control);

	static void ShowTunes(Game* game, Control* control);
	static void ShowConnection(Game* game, Control* control);
	static void CameraFoVPlus(Game* game, Control* control);
	static void CameraFoVMinus(Game* game, Control* control);
	static void SetCameraFree(Game* game, Control* control);
	static void SetCameraLocked(Game* game, Control* control);
	static void ConnectToServer(Game* game, Control* control);
	static void Upgrade(Game* game, Control* control);

    Font* _font;
	AutoRef<Scene> _scene;
    PhysicsCharacter* _character;
    Node* _characterNode;
    Node* _characterMeshNode;
    Node* _characterShadowNode;
    Node* _basketballNode;
    float _floorLevel;
    Animation* _animation;
    int _rotateX;
    MaterialParameter* _materialParameterAlpha;
    unsigned int _keyFlags;
    bool _physicsDebug;
    bool _wireframe;
    Vector3 _oldBallPosition;
    bool _hasBall;
    bool _applyKick;
    bool _kicking;
    float _kickDelay;
    bool* _buttonPressed;
    Vector2 _currentDirection;
    Gamepad* _gamepad;
	Camera* _camera;
	TargetCamera _fpCamera;
	bool _freeCamera;
	Vector3 _battleFieldDirection;
	unsigned int _moveFlags;
	int _prevX;
	int _prevY;
	Node* _particleEmitterSunNode;
	Node* _particleEmitterStarsNode;
	GameHUD _hud;
	double _totalTime;
	double _respawnTime;
	Client _client;
	GameObjectManager _manager;
	int _currentPlayerIDforUI;
public:
	int _ping;
	PLAYERID _netPlayerID;
	bool Respawn;
};

#endif //SHPILA_H_