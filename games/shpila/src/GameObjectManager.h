#ifndef GAMEOBJECTMANAGER_H_
#define GAMEOBJECTMANAGER_H_

#include "gameplay.h"
#include "Player.h"
#include "BaseGameObject.h"
#include "algorithms\Links.h"
using namespace gameplay;

typedef BaseGameObject* (*GameObjectConstructorProc) ();

class Player;

struct GameUnit
{
	Node* _node;
	GameObjectConstructorProc _constructor;
	GameUnit(Node* node, GameObjectConstructorProc constructor)
		: _node(node), _constructor(constructor)
	{}
	GameUnit()
		: _node(NULL), _constructor(NULL)
	{}
};

class GameObjectManager
{
public:
	std::vector<Player*> Players;

	GameObjectManager();
	void setScene(Scene* scene);
	bool initializeNodeMaterials(Node* node);
	void initializeMaterial(Scene* scene, Node* node, Material* material);
	void update(float time);
	void addUnit(const char* name, Node* node, GameObjectConstructorProc constructor);
	BaseGameObject* createObject(const char* name, Vector3 position, int playerID);
private:
	void interaction(BaseGameObject* object);
	AutoRef<Scene> _scene;
	std::map<std::string, GameUnit> _units;
	COwnerList<BaseGameObject> _objects;

};


#endif