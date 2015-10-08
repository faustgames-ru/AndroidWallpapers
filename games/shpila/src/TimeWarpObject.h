#ifndef TIME_WARP_OBJECT_H_
#define TIME_WARP_OBJECT_H_

class TimeWarpObject : public BaseGameObject
{
public:
	TimeWarpObject();
	static BaseGameObject* constructor();
	virtual void init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, Matrix transform);
	virtual bool interactive();
	virtual void update(float time);
};

#endif