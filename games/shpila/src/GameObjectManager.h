#ifndef GAMEOBJECTMANAGER_H_
#define GAMEOBJECTMANAGER_H_

using namespace gameplay;

typedef BaseGameObject* (*GameObjectConstructorProc) ();

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

class GameObjectManager : public PlugIn
{
public:
	std::vector<PlayerObject*> Players;

	GameObjectManager();
	void setScene(Scene* scene);
	Scene* scene();
	bool initializeNodeMaterials(Node* node);
	bool initializeCharacterNodeScale(Node* node);
	void initializeMaterial(Node* node, Material* material);
	void addUnit(const char* filename, const char* name, GameObjectConstructorProc constructor);
	void initUnits();
	BaseGameObject* createObject(const char* name, Vector3 position, PlayerObject* player);
	void registerMovementController(UnitMovementBase* controller);
	void unregisterMovementController(UnitMovementBase* controller);
	void registerObject(BaseGameObject* object);
	void registerSceneNode(Node* node);

	const char* name(void) { return "dzhezuka"; }
	float selectionOrderSortKey(void) { return 0.01f; }
	void open(void);
	virtual void redraw(const float currentTime, const float elapsedTime){}
	void update(const float currentTime, const float elapsedTime);
	Itr<BaseGameObject> objects();
	void close(void)
	{
		// clear the group of all vehicles
		_all.clear();
	}
	void reset(void){}
	void handleFunctionKeys(int keyNumber){}
	const AVGroup& allVehicles(void) { return (const AVGroup&)_all; }
private:
	void interaction(BaseGameObject* object);
	AutoRef<Scene> _scene;
	std::map<std::string, GameUnit> _units;
	COwnerList<BaseGameObject> _objects;
	// a group (STL vector) of all vehicles in the PlugIn
	std::vector<UnitMovementBase*> _all;
	ProximityDatabase* _pd;
	AutoRef<Scene> _store;
};


#endif