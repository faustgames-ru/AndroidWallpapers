#ifndef BASEACTOR_H_
#define BASEACTOR_H_

using namespace gameplay;

class BaseActor : public BaseStaticActor
{
public:
	BaseActor();
	~BaseActor();
	virtual void init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, Matrix transform);
	virtual void setPosition(const Vector3 pos);
protected:
	UnitMovementBase _movementController;
};

#endif