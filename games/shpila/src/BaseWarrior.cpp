#include "Headers.h"
#include "main.h"
#include "algorithms\StdUtils.h"

const unsigned long DEFAULT_BLENDING_TIME = 150;

ActionClips::ActionClips()
: CurrentClip()
, Clips()
{}

bool ActionClips::Playing()
{
	return CurrentClip && (CurrentClip->isPlaying());
}

void ActionClips::Play()
{
	if ((CurrentClip == NULL) || !CurrentClip->isPlaying())
	{
		AnimationClip* clip = Clips[rndIndex(0, Clips.size() - 1)];
		clip->setRepeatCount(1.0f);
		if ((CurrentClip != NULL) && (CurrentClip != clip))
			CurrentClip->crossFade(clip, DEFAULT_BLENDING_TIME);
		else
			clip->play();
		CurrentClip = clip;
	}
}

void ActionClips::addBeginListener(AnimationClip::Listener* listener)
{
	for (std::vector<AutoRef<AnimationClip>>::iterator it = Clips.begin(); it != Clips.end(); it++)
	{
		(*it)->addBeginListener(listener);
	}
}

void ActionClips::addListener(AnimationClip::Listener* listener, unsigned long eventTime)
{
	for (std::vector<AutoRef<AnimationClip>>::iterator it = Clips.begin(); it != Clips.end(); it++)
	{
		(*it)->addListener(listener, eventTime);
	}
}

unsigned long ActionClips::getDuration()
{
	if (Clips.size())
		return 0;

	int res = 0;
	for (std::vector<AutoRef<AnimationClip>>::iterator it = Clips.begin(); it != Clips.end(); it++)
	{
		res += (*it)->getDuration();
	}
	
	return (unsigned long)(res / Clips.size());
}


BaseWarrior::BaseWarrior()
: BaseActor()
, _initialized(false)
, _dead(false)
, _deadAltitude(0.0f)
, _unitActions()
, _illusionTimer()
{}

BaseWarrior::~BaseWarrior()
{
	
}

BaseGameObject* BaseWarrior::constructor()
{
	return new BaseWarrior();
}

void BaseWarrior::makeIllusion()
{
	LocalGameData.Illusion = true;
	_illusionTimer.start(JEATS_ILLUSION_LIFE_TIME, 0.0f);
}

void BaseWarrior::init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, const Matrix transform)
{
	BaseActor::init(manager, gameData, node, player, transform);
	updateAnimationState();
	_illusionTimer.enable(false);
}

bool BaseWarrior::illusionable()
{
	return !UsedForIllusion && !LocalGameData.Illusion && (LocalGameData.Health > 0.0f);
}

void BaseWarrior::interaction(BaseGameObject* object)
{
	BaseActor::interaction(object);
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
			Player->Manager.createObject(TypeName.c_str(), position() + 15.0f * Player->BattleFieldDirection, Player->BattleFieldDirection, Player);
		}
		return;
	}

	if (_dead)
	{
		if (!_unitActions.Actions[UnitActions::Death]->Playing())
		{
			switchToAction(UnitActions::Dead);
		}
		disappearing(time);
		return;
	}

	if (LocalGameData.Health <= 0.0f)
	{
		switchToAction(UnitActions::Death);
		_dead = true;
		_manager->unregisterMovementController(&_movementController);
		return;
	}

	BaseActor::update(time);
	updateMidLineState();
	updateMovementSpeed(time);
	updateAttack(time, Target);
	updatePositionFromServer(time);

	if (_illusionTimer.enabled() && _illusionTimer.action(time))
	{
		_deadAltitude = -(AIR_UNITS_ALTITUDE + 1.0f);
		_illusionTimer.enable(false);
	}
}

void BaseWarrior::updateMovementSpeed(float time)
{
	float speed = ((Target != NULL) ? LocalGameData.GameData->MoveSpeed : DEFAULT_MOVEMENT_SPEED) / TIME_SCALE * 
		(getAura(Aura::TimeWarpSlow) ? QBI_TIME_WARP_SPEED_FACTOR : 1.0f);
	_movementController.setMaxSpeed(speed);	
	_movementController.setSpeed(min(_movementController.speed(), _movementController.maxSpeed()));
	_movementController.setAcceleration(DEFAULT_UNIT_AXELERATION);
}

bool BaseWarrior::deleted()
{
	return _deadAltitude <= -(AIR_UNITS_ALTITUDE + 1.0f);
}

void BaseWarrior::disappearing(float time)
{
	_deadAltitude -= time * 0.005f;
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
		Node* node = _node->getFirstChild();
		while (node)
		{
			Animation* anim = node->getAnimation("animations");
			if (!anim)
				anim = node->getAnimation(NULL);
			if (anim)
			{
				_unitActions.Actions[UnitActions::Run] = (new ActionClips())->Auto();
				_unitActions.Actions[UnitActions::Attack] = (new ActionClips())->Auto();
				_unitActions.Actions[UnitActions::Death] = (new ActionClips())->Auto();
				_unitActions.Actions[UnitActions::Dead] = (new ActionClips())->Auto();
				for (int i = 0; i < (int)anim->getClipCount(); i++)
					if (!strcmp(anim->getClip(i)->getGroup(), "run"))
						_unitActions.Actions[UnitActions::Run]->Clips.push_back(anim->getClip(i));
				for (int i = 0; i < (int)anim->getClipCount(); i++)
					if (!strcmp(anim->getClip(i)->getGroup(), "attack"))
						_unitActions.Actions[UnitActions::Attack]->Clips.push_back(anim->getClip(i));
				for (int i = 0; i < (int)anim->getClipCount(); i++)
					if (!strcmp(anim->getClip(i)->getGroup(), "death"))
						_unitActions.Actions[UnitActions::Death]->Clips.push_back(anim->getClip(i));
				for (int i = 0; i < (int)anim->getClipCount(); i++)
					if (!strcmp(anim->getClip(i)->getGroup(), "dead"))
						_unitActions.Actions[UnitActions::Dead]->Clips.push_back(anim->getClip(i));

				if (_unitActions.Actions[UnitActions::Run]->Clips.size() == 0)
					_unitActions.Actions[UnitActions::Run]->Clips.push_back(anim->getClip("run"));
				if (_unitActions.Actions[UnitActions::Attack]->Clips.size() == 0)
					_unitActions.Actions[UnitActions::Attack]->Clips.push_back(anim->getClip("attack"));
				if (_unitActions.Actions[UnitActions::Death]->Clips.size() == 0)
					_unitActions.Actions[UnitActions::Death]->Clips.push_back(anim->getClip("death"));
				if (_unitActions.Actions[UnitActions::Dead]->Clips.size() == 0)
					_unitActions.Actions[UnitActions::Dead]->Clips.push_back(anim->getClip("dead"));
				//T* operator= (T* other){ if (_object != other) { SAFE_RELEASE(_object); _object = other; other->addRef(); } return _object; };
			}
			node = node->getNextSibling();
		}
		_initialized = true;
	}
}

void BaseWarrior::switchToAction(UnitActions::Action action)
{
	_unitActions.Actions[action]->Play();
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
		Vector3 tPos = object->position();
		updateMoveToPoint(time, tPos);

		float attackDistance = getAttackDistance(object);
		float radius = SQR(attackDistance);
		float distance = tPos.distanceSquared(position());
		if (radius < distance)
		{
			switchToAction(UnitActions::Run);
			_movementController._applyBreakingForces = false;
			attackEnable(false);
		}
		else
		{
			_movementController._applyBreakingForces = true;
			if (LocalGameData.GameData->isAttackToTargetAllowed(*object->LocalGameData.GameData))
			{
				attackEnable(true);
				switchToAction(UnitActions::Attack);
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
			switchToAction(UnitActions::Run);
			attackEnable(false);
			_movementController._applyBreakingForces = false;
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