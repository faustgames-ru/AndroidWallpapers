#ifndef SHPILA_H_
#define SHPILA_H_

#include "Headers.h"
using namespace gameplay;


class Actions
{
public:
	enum Action
	{
		MOVE_FORWARD,
		MOVE_BACKWARD,
		MOVE_LEFT,
		MOVE_RIGHT,
		MOVE_UP,
		MOVE_DOWN,
		SWITCH_CAMERA,
		PLACE_UNIT,
		CANCEL_PLACE_UNIT,
		ALT,
		SHIFT,
		CTRL,
		ESC,
	};
};

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
	virtual ~Shpila();

	virtual void restoreDeviceObjects();

	bool initializeNodeMaterials(Node* node);
	bool isActivePlayer(PlayerObject* player);
	PlayerObject* getActivePlayer();
	TargetCamera* getActiveCamera();
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
	void loadCharacters();
	void initPlayers();
    void drawSplash(void* param);
    bool drawScene(Node* node, bool transparent);
	bool drawHPBars(const Matrix matrix);
	//---update parts
	void updatePlayers(float time);
	void updateNetwork();
	void updateMenuButtons();
	void updateActions(float elapsedTime);
	void updateKeyStates();
	//---
	bool PlaceUnit(int x, int y, bool continuous);
	void loadActionMap();
	void selectUnit();
	Keyboard::KeyState getActionState(Actions::Action action);
	void setKeyState(int key, bool pressed);

	static const char* format(char* fstr, ...);

	static void CreateUnit(Game* game, Control* control);
	static void SwitchPlayer(Game* game, Control* control);

	static void ShowTunes(Game* game, Control* control);
	static void ShowConnection(Game* game, Control* control);
	static void CameraFoVPlus(Game* game, Control* control);
	static void CameraFoVMinus(Game* game, Control* control);
	static void SetCameraFree(Game* game, Control* control);
	static void SetCameraLocked(Game* game, Control* control);
	static void ConnectToServer(Game* game, Control* control);
	static void Upgrade(Game* game, Control* control);
	static void AddExtractor(Game* game, Control* control);
	static void CloseGame(Game* game, Control* control);
	static void ShowUnitsPage(Game* game, Control* control);
	static void ShowUpgradesPage(Game* game, Control* control);

	static void loadSetting(Game* game, Control* control);
	static void saveSetting(Game* game, Control* control);
	static void AddEnergy(Game* game, Control* control);
	

	static void PauseClick(Game* game, Control* control);

    Font* _font;
	AutoRef<Scene> _scene;
    MaterialParameter* _materialParameterAlpha;
    bool _physicsDebug;
    bool _wireframe;
	float _cameraVelocityFactor;
	std::map<Actions::Action, Keyboard::Key> _actionsMap;
	std::map<Keyboard::Key, Keyboard::KeyState> _keyMap;
	int _mouseX;
	int _mouseY;
	int _wheelDelta;
	TargetCamera _fpCamera;
	TargetCamera _CameraPlayer[2];
	TargetCamera* _activeCamera;
	TargetCamera* _activePlayerCamera;
	bool _freeCamera;
	Vector3 _battleFieldDirection;	
	int _prevX;
	int _prevY;
	Node* _particleEmitterSunNode;
	Node* _particleEmitterStarsNode;
	GameHUD _hud;
	double _totalTime;	
	Client _client;
	GameObjectManager _manager;
	int _currentPlayerIDforUI;
	SpriteBatch* _HPBarsSpriteBatch;
public:
	int _ping;
	PLAYERID _netPlayerID;
	double _respawnTime;
	bool Respawn;
};

#endif //SHPILA_H_