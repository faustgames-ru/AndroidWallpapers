#ifndef TIME_WARP_OBJECT_H_
#define TIME_WARP_OBJECT_H_

class TimeWarpObject : public BaseGameObject
{
public:
	TimeWarpObject();
	static BaseGameObject* constructor();
	virtual void init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, const Matrix transform);
	virtual bool interactive();
	virtual void interaction(BaseGameObject* object);
	virtual void update(float time);
	virtual bool deleted();
private:
	SimpleTimer _lifeTimer;
};

#endif