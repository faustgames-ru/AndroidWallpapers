#ifndef BASEACTOR_H_
#define BASEACTOR_H_

using namespace gameplay;

class BaseGameObject;


class BaseActor : public BaseGameObject
{
public:
	BaseActor();
	~BaseActor();
	virtual void init(GameObjectManager& manager, Node* node, int playerID, Matrix transform);
	virtual void setPosition(const Vector3 pos);
protected:
	UnitMovementBase _movementController;
};

#endif