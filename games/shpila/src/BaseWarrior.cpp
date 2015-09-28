#include "Headers.h"
#include "main.h"
#include "algorithms\StdUtils.h"

const unsigned long DEFAULT_BLENDING_TIME = 150;

BaseWarrior::BaseWarrior()
: BaseActor()
, _initialized(false)
, _dead(false)
, _deadAltitude(0.0f)
, _unitAnimation()
{}

BaseWarrior::~BaseWarrior()
{
	DeleteVectorPointers(_unitAnimation);
}

BaseGameObject* BaseWarrior::constructor()
{
	return new BaseWarrior();
}

void BaseWarrior::init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, Matrix transform)
{
	BaseActor::init(manager, gameData, node, player, transform);
	updateAnimationState();
}

void BaseWarrior::interaction(BaseGameObject* object)
{
	//collision
	if ((LocalGameData.Health > 0.0f) && (object->LocalGameData.Health > 0.0f) && (position().distanceSquared(object->position()) < SQR(LocalGameData.GameData->GeometryRadius)))
	{
		Vector3 offset = (position() - object->position()) * 0.5f;
		_node->setTranslation(_node->getTranslation() + offset);
	}
	targeting(object);
}

void BaseWarrior::update(float time)
{
	if (Holder)
	{
		bool resp = ((Shpila*)Game::getInstance())->Respawn;
		if (((Shpila*)Game::getInstance())->isActivePlayer(Player) && resp)
		{
			Player->Manager.createObject(HolderWarriorName.c_str(), position() + 15.0f * Player->BattleFieldDirection, Player->BattleFieldDirection, Player);
		}
		return;
	}

	if (_dead)
	{
		if (_unitAnimation.size() > 0)
		{
			if (!(*_unitAnimation[0]->_clips)[UnitAnimation::Death]->isPlaying())
			{
				switchToAnimation(UnitAnimation::Dead, AnimationClip::REPEAT_INDEFINITE, 0);
			}
		}
		disappearing(time);
		return;
	}

	if (LocalGameData.Health <= 0.0f)
	{
		switchToAnimation(UnitAnimation::Death, 1.0f, DEFAULT_BLENDING_TIME);
		_dead = true;
		_manager->unregisterMovementController(&_movementController);
		return;
	}

	BaseActor::update(time);
	updateMidLineState();
	updateAttack(time, Target);
	updatePositionFromServer(time);
}

bool BaseWarrior::deleted()
{
	return _deadAltitude < -3.0f;
}

void BaseWarrior::disappearing(float time)
{
	_deadAltitude -= time * 0.001f;
	_node->setTranslation(_node->getTranslation() + Vector3(0.0, -time * 0.001f, 0.0f));
}


//const float ROTATION_FACTOR = 0.002f;
//const float MOVE_FACTOR = 0.0005f;

//Vector3 targetDir = Vector3(Target->position() - position()).normalize();
//Vector3 objectDir = _node->getForwardVectorWorld().normalize();
//float targetAngle = Vector3::angle(objectDir, targetDir);

//float rotationAngle = (targetAngle > (time * ROTATION_FACTOR)) ? time * ROTATION_FACTOR : targetAngle;
//Vector3 rightVector = _node->getRightVectorWorld().normalize();
//rotationAngle = copysignf(1.0f, Vector3::dot(rightVector, targetDir)) * rotationAngle;
//Vector3 vAxis;
//float currentAngle = _node->getRotation(&vAxis) + rotationAngle;
//while (currentAngle > 2.0f * MATH_PI) currentAngle -= 2.0f * MATH_PI;
//while (currentAngle < 0.0f) currentAngle += 2.0f * MATH_PI;
//_node->setRotation(Vector3::unitY(), currentAngle);
//_movementController.setForward(OpenSteer::Vec3(-objectDir.x, -objectDir.y, -objectDir.z));

//_node->setTranslation(_node->getTranslation() + targetDir * MOVE_FACTOR * time);
//_movementController.applySteeringForce(OpenSteer::Vec3(targetDir.x, targetDir.y, targetDir.z) * MOVE_FACTOR, time * 0.001f);

void BaseWarrior::updateMidLineState()
{
	float dot = Vector3(Player->getPosition() - Player->BattleFieldMidPoint).dot(position() - Player->BattleFieldMidPoint);
	if (dot < 0.0f)
		Player->UnitsOverMidLineCount++;
}

void BaseWarrior::updateAnimationState()
{
	//initialization
	if (!_initialized)
	{
		_unitAnimation.clear();
		Node* node = _node->getFirstChild();
		while (node)
		{
			Animation* anim = node->getAnimation("animations");
			if (!anim)
				anim = node->getAnimation(NULL);
			if (anim)
			{
				UnitAnimation::ActionsMap* am = new UnitAnimation::ActionsMap();
				(*am)[UnitAnimation::Run] = anim->getClip("run");
				(*am)[UnitAnimation::Attack] = anim->getClip("attack");
				(*am)[UnitAnimation::Death] = anim->getClip("death");
				(*am)[UnitAnimation::Dead] = anim->getClip("dead");
				_unitAnimation.push_back(new UnitAnimation(am));
			}
			node = node->getNextSibling();
		}
		_initialized = true;
	}
}

void BaseWarrior::switchToAnimation(UnitAnimation::Actions action, float repeatCount, unsigned long blendingTime)
{
	for (std::vector<UnitAnimation*>::iterator it = _unitAnimation.begin(); it != _unitAnimation.end(); it++)
	{
		AnimationClip* clip = (*(*it)->_clips)[action];
		if (((*it)->_clipCurrent != clip) || ((*it)->_clipCurrent == NULL) || (!(*it)->_clipCurrent->isPlaying()))
		{
			//clip->setSpeed(1.0f);
			clip->setRepeatCount(repeatCount);
			if ((*it)->_clipCurrent && (blendingTime > 0))
				(*it)->_clipCurrent->crossFade(clip, blendingTime);
			else
				clip->play();
			(*it)->_clipCurrent = clip;
		}
	}
}

void BaseWarrior::updatePositionFromServer(float time)
{
	if (_positionOnServer.defined())
	{
		float minStep = time * 0.01f;
		float sqDist = position().distanceSquared(_positionOnServer);
		if (sqDist > 1.0f)
			_synkPositionMode = true;
		if (_synkPositionMode)
		{
			if (sqDist <= (minStep * minStep))
			{
				setPosition(_positionOnServer);
				_synkPositionMode = false;
			}
			else
			{
				Vector3 dir = (Vector3(_positionOnServer) - position()) / sqrt(sqDist);
				setPosition(position() + dir * minStep);
			}
		}
	}
}

void BaseWarrior::updateAttack(float time, BaseGameObject* object)
{
	if (!isAttackToTargetAllowed(object))
		object = NULL;

	if (object != NULL)
	{
		_movementController.setMaxSpeed(LocalGameData.GameData->MoveSpeed / TIME_SCALE);
		Vector3 tPos = object->position();
		updateMoveToPoint(time, tPos);

		float attackDistance = getAttackDistance(object);
		float radius = SQR(attackDistance);
		float distance = tPos.distanceSquared(position());
		if (radius < distance)
		{
			switchToAnimation(UnitAnimation::Run, AnimationClip::REPEAT_INDEFINITE, DEFAULT_BLENDING_TIME);
			_movementController._applyBreakingForces = false;
			_damageTimer.enable(false);
		}
		else
		{
			_movementController._applyBreakingForces = true;
			if (LocalGameData.GameData->isAttackToTargetAllowed(*object->LocalGameData.GameData))
			{
				_damageTimer.enable(true);
				switchToAnimation(UnitAnimation::Attack, 1, DEFAULT_BLENDING_TIME);
			}
		}
	}
	else
	{
		//!! move forvard
		Vector3 dir = position() - Player->EnemyPlayer->getDefence()->position();
		if (Player->BattleFieldDirection.dot(dir) <= 0.0f)
		{
			updateMoveToPoint(time, position() + Player->BattleFieldDirection * 10.0f);
			switchToAnimation(UnitAnimation::Run, AnimationClip::REPEAT_INDEFINITE, DEFAULT_BLENDING_TIME);
			_damageTimer.enable(false);
			_movementController._applyBreakingForces = false;
			_movementController.setMaxSpeed(2.25f / TIME_SCALE);
		}
	}
}

void BaseWarrior::updateMoveToPoint(float time, Vector3 point)
{
	_movementController._target = OpenSteer::Vec3(point.x, point.y, point.z);
	_movementController.update(0.0f, time * 0.001f);
	OpenSteer::Vec3 forward = _movementController.forward();
	Matrix rot;
	createCharacterRotationMatrix(Vector3(forward.x, forward.y, forward.z), &rot);
	_node->setRotation(rot);
	OpenSteer::Vec3 pos = _movementController.position();
	_node->setTranslation(Vector3(pos.x, pos.y, pos.z));
}