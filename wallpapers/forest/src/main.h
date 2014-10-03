#ifndef CHARACTERGAME_H_
#define CHARACTERGAME_H_

#include "gameplay.h"
using namespace gameplay;

/**
 * This is a mesh demo game for rendering Mesh.
 */
class AsteroidsTest: public Game
{
public:
    
    /**
     * Constructor.
     */
	AsteroidsTest();

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

private:
    
    bool initializeScene(Node* node);
    void initializeMaterial(Scene* scene, Node* node, Material* material);
	void initializeAsteroids();
    void drawSplash(void* param);
    bool drawScene(Node* node, bool transparent);
    void play(const char* id, bool repeat, float speed = 1.0f);

    Font* _font;
    Scene* _scene;
    PhysicsCharacter* _character;
    Node* _characterNode;
    Node* _characterMeshNode;
    Node* _characterShadowNode;
    Node* _basketballNode;
    float _floorLevel;
    Animation* _animation;
    AnimationClip* _currentClip;
    AnimationClip* _jumpClip;
    AnimationClip* _kickClip;
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

};

#endif 