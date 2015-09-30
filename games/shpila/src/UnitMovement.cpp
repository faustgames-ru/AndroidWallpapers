
#include "Headers.h"

AVGroup UnitMovementBase::neighbors;

void UnitMovementBase::reset(void)
{
	// reset the vehicle 
	SimpleVehicle::reset();
	//setForward(-1.0f, 0.0f, 0.0f);

	// size of bounding sphere, for obstacle avoidance, etc.
	setRadius(1.5); // width = 0.7, add 0.3 margin, take half

	setSpeed(0.0f);             // speed along Forward direction.
	setMaxForce(20.0f);        // steering force is clipped to this magnitude
	setAcceleration(DEFAULT_UNIT_AXELERATION);
	setMaxSpeed(DEFAULT_MOVEMENT_SPEED / TIME_SCALE);        // velocity is clipped to this magnitude

	// notify proximity database that our position has changed
	proximityToken->updateForNewPosition(position());
}

// ----------------------------------------------------------------------------
// globals
// (perhaps these should be member variables of a Vehicle or PlugIn class)


/*const int UnitMovementBase::maxObstacleCount = 100;

const Vec3 gHomeBaseCenter(0, 0, 0);
const float gHomeBaseRadius = 1.5;

const float gMinStartRadius = 30;
const float gMaxStartRadius = 40;

const float gBrakingRate = 0.75;

const Vec3 evadeColor(0.6f, 0.6f, 0.3f); // annotation
const Vec3 seekColor(0.3f, 0.6f, 0.6f); // annotation
const Vec3 clearPathColor(0.3f, 0.6f, 0.3f); // annotation

const float gAvoidancePredictTimeMin = 0.9f;
const float gAvoidancePredictTimeMax = 2;
float gAvoidancePredictTime = gAvoidancePredictTimeMin;

bool enableAttackSeek = true; // for testing (perhaps retain for UI control?)
bool enableAttackEvade = true; // for testing (perhaps retain for UI control?)

UnitMovementSeeker* gSeeker = NULL;


// count the number of times the simulation has reset (e.g. for overnight runs)
int resetCount = 0;


// ----------------------------------------------------------------------------
// state for OpenSteerDemo PlugIn
//
// XXX consider moving this inside UnitMovementPlugIn
// XXX consider using STL (any advantage? consistency?)


UnitMovementSeeker* unitMovementSeeker;
const int UnitMovementEnemyCount = 4;
UnitMovementEnemy* UnitMovementEnemies[UnitMovementEnemyCount];


// ----------------------------------------------------------------------------
// reset state


void UnitMovementBase::reset(void)
{
	SimpleVehicle::reset();  // reset the vehicle 

	setSpeed(3);             // speed along Forward direction.
	setMaxForce(3.0);        // steering force is clipped to this magnitude
	setMaxSpeed(3.0);        // velocity is clipped to this magnitude

	avoiding = false;         // not actively avoiding

	randomizeStartingPositionAndHeading();  // new starting position
}


void UnitMovementSeeker::reset(void)
{
	UnitMovementBase::reset();
	bodyColor.set(0.4f, 0.4f, 0.6f); // blueish
	gSeeker = this;
	state = running;
	evading = false;
}


void UnitMovementEnemy::reset(void)
{
	UnitMovementBase::reset();
	bodyColor.set(0.6f, 0.4f, 0.4f); // redish
}

void UnitMovementBase::randomizeStartingPositionAndHeading(void)
{
	// randomize position on a ring between inner and outer radii
	// centered around the home base
	const float rRadius = frandom2(gMinStartRadius, gMaxStartRadius);
	const Vec3 randomOnRing = RandomUnitVectorOnXZPlane() * rRadius;
	setPosition(gHomeBaseCenter + randomOnRing);

	// are we are too close to an obstacle?
	if (minDistanceToObstacle(position()) < radius() * 5)
	{
		// if so, retry the randomization (this recursive call may not return
		// if there is too little free space)
		randomizeStartingPositionAndHeading();
	}
	else
	{
		// otherwise, if the position is OK, randomize 2D heading
		randomizeHeadingOnXZPlane();
	}
}


// ----------------------------------------------------------------------------


void UnitMovementEnemy::update(const float currentTime, const float elapsedTime)
{
	// determine upper bound for pursuit prediction time
	const float seekerToGoalDist = Vec3::distance(gHomeBaseCenter,
		gSeeker->position());
	const float adjustedDistance = seekerToGoalDist - radius() - gHomeBaseRadius;
	const float seekerToGoalTime = ((adjustedDistance < 0) ?
		0 :
		(adjustedDistance / gSeeker->speed()));
	const float maxPredictionTime = seekerToGoalTime * 0.9f;

	// determine steering (pursuit, obstacle avoidance, or braking)
	Vec3 steer(0, 0, 0);
	if (gSeeker->state == running)
	{
		const Vec3 avoidance =
			steerToAvoidObstacles(gAvoidancePredictTimeMin,
			(ObstacleGroup&)allObstacles);

		// saved for annotation
		avoiding = (avoidance == Vec3::zero);

		if (avoiding)
			steer = steerForPursuit(*gSeeker, maxPredictionTime);
		else
			steer = avoidance;
	}
	else
	{
		applyBrakingForce(gBrakingRate, elapsedTime);
	}
	applySteeringForce(steer, elapsedTime);

	// detect and record interceptions ("tags") of seeker
	const float seekerToMeDist = Vec3::distance(position(),
		gSeeker->position());
	const float sumOfRadii = radius() + gSeeker->radius();
	if (seekerToMeDist < sumOfRadii)
	{
		if (gSeeker->state == running) gSeeker->state = tagged;
	}
}


// ----------------------------------------------------------------------------
// are there any enemies along the corridor between us and the goal?


bool UnitMovementSeeker::clearPathToGoal(void)
{
	const float sideThreshold = radius() * 8.0f;
	const float behindThreshold = radius() * 2.0f;

	const Vec3 goalOffset = gHomeBaseCenter - position();
	const float goalDistance = goalOffset.length();
	const Vec3 goalDirection = goalOffset / goalDistance;

	const bool goalIsAside = isAside(gHomeBaseCenter, 0.5);

	// for annotation: loop over all and save result, instead of early return 
	bool xxxReturn = true;

	// loop over enemies
	for (int i = 0; i < UnitMovementEnemyCount; i++)
	{
		// short name for this enemy
		const UnitMovementEnemy& e = *UnitMovementEnemies[i];
		const float eDistance = Vec3::distance(position(), e.position());
		const float timeEstimate = 0.3f * eDistance / e.speed(); //xxx
		const Vec3 eFuture = e.predictFuturePosition(timeEstimate);
		const Vec3 eOffset = eFuture - position();
		const float alongCorridor = goalDirection.dot(eOffset);
		const bool inCorridor = ((alongCorridor > -behindThreshold) &&
			(alongCorridor < goalDistance));
		const float eForwardDistance = forward().dot(eOffset);

		// consider as potential blocker if within the corridor
		if (inCorridor)
		{
			const Vec3 perp = eOffset - (goalDirection * alongCorridor);
			const float acrossCorridor = perp.length();
			if (acrossCorridor < sideThreshold)
			{
				// not a blocker if behind us and we are perp to corridor
				const float eFront = eForwardDistance + e.radius();

				//annotationLine (position, forward*eFront, gGreen); // xxx
				//annotationLine (e.position, forward*eFront, gGreen); // xxx

				// xxx
				// std::ostringstream message;
				// message << "eFront = " << std::setprecision(2)
				//         << std::setiosflags(std::ios::fixed) << eFront << std::ends;
				// draw2dTextAt3dLocation (*message.str(), eFuture, gWhite);

				const bool eIsBehind = eFront < -behindThreshold;
				const bool eIsWayBehind = eFront < (-2 * behindThreshold);
				const bool safeToTurnTowardsGoal =
					((eIsBehind && goalIsAside) || eIsWayBehind);

				if (!safeToTurnTowardsGoal)
				{
					// return false;
					xxxReturn = false;
				}
			}
		}
	}

	return xxxReturn;
}

Vec3 UnitMovementSeeker::steerToEvadeAllDefenders(void)
{
	Vec3 evade(0, 0, 0);
	const float goalDistance = Vec3::distance(gHomeBaseCenter, position());

	// sum up weighted evasion
	for (int i = 0; i < UnitMovementEnemyCount; i++)
	{
		const UnitMovementEnemy& e = *UnitMovementEnemies[i];
		const Vec3 eOffset = e.position() - position();
		const float eDistance = eOffset.length();

		const float eForwardDistance = forward().dot(eOffset);
		const float behindThreshold = radius() * 2;
		const bool behind = eForwardDistance < behindThreshold;
		if ((!behind) || (eDistance < 5))
		{
			if (eDistance < (goalDistance * 1.2)) //xxx
			{
				// const float timeEstimate = 0.5f * eDistance / e.speed;//xxx
				const float timeEstimate = 0.15f * eDistance / e.speed();//xxx
				const Vec3 future =
					e.predictFuturePosition(timeEstimate);

				const Vec3 offset = future - position();
				const Vec3 lateral = offset.perpendicularComponent(forward());
				const float d = lateral.length();
				const float weight = -1000 / (d * d);
				evade += (lateral / d) * weight;
			}
		}
	}
	return evade;
}


Vec3 UnitMovementSeeker::XXXsteerToEvadeAllDefenders(void)
{
	// sum up weighted evasion
	Vec3 evade(0, 0, 0);
	for (int i = 0; i < UnitMovementEnemyCount; i++)
	{
		const UnitMovementEnemy& e = *UnitMovementEnemies[i];
		const Vec3 eOffset = e.position() - position();
		const float eDistance = eOffset.length();

		// xxx maybe this should take into account e's heading? xxx
		const float timeEstimate = 0.5f * eDistance / e.speed(); //xxx
		const Vec3 eFuture = e.predictFuturePosition(timeEstimate);

		// steering to flee from eFuture (enemy's future position)
		const Vec3 flee = xxxsteerForFlee(eFuture);

		const float eForwardDistance = forward().dot(eOffset);
		const float behindThreshold = radius() * -2;

		const float distanceWeight = 4 / eDistance;
		const float forwardWeight = ((eForwardDistance > behindThreshold) ?
			1.0f : 0.5f);

		const Vec3 adjustedFlee = flee * distanceWeight * forwardWeight;

		evade += adjustedFlee;
	}
	return evade;
}


// ----------------------------------------------------------------------------


Vec3 UnitMovementSeeker::steeringForSeeker(void)
{
	// determine if obstacle avodiance is needed
	const bool clearPath = clearPathToGoal();
	adjustObstacleAvoidanceLookAhead(clearPath);
	const Vec3 obstacleAvoidance =
		steerToAvoidObstacles(gAvoidancePredictTime,
		(ObstacleGroup&)allObstacles);

	// saved for annotation
	avoiding = (obstacleAvoidance != Vec3::zero);

	if (avoiding)
	{
		// use pure obstacle avoidance if needed
		return obstacleAvoidance;
	}
	else
	{
		// otherwise seek home base and perhaps evade defenders
		const Vec3 seek = xxxsteerForSeek(gHomeBaseCenter);
		if (clearPath)
		{
			// we have a clear path (defender-free corridor), use pure seek

			// xxx experiment 9-16-02
			Vec3 s = limitMaxDeviationAngle(seek, 0.707f, forward());
			return s;
		}
		else
		{
			if (0) // xxx testing new evade code xxx
			{
				// combine seek and (forward facing portion of) evasion
				const Vec3 evade = steerToEvadeAllDefenders();
				const Vec3 steer =
					seek + limitMaxDeviationAngle(evade, 0.5f, forward());

				return steer;
			}
			else

			{
				const Vec3 evade = XXXsteerToEvadeAllDefenders();
				const Vec3 steer = limitMaxDeviationAngle(seek + evade,
					0.707f, forward());

				return steer;
			}
		}
	}
}


// ----------------------------------------------------------------------------
// adjust obstacle avoidance look ahead time: make it large when we are far
// from the goal and heading directly towards it, make it small otherwise.


void UnitMovementSeeker::adjustObstacleAvoidanceLookAhead(const bool clearPath)
{
	if (clearPath)
	{
		evading = false;
		const float goalDistance = Vec3::distance(gHomeBaseCenter, position());
		const bool headingTowardGoal = isAhead(gHomeBaseCenter, 0.98f);
		const bool isNear = (goalDistance / speed()) < gAvoidancePredictTimeMax;
		const bool useMax = headingTowardGoal && !isNear;
		gAvoidancePredictTime =
			(useMax ? gAvoidancePredictTimeMax : gAvoidancePredictTimeMin);
	}
	else
	{
		evading = true;
		gAvoidancePredictTime = gAvoidancePredictTimeMin;
	}
}


// ----------------------------------------------------------------------------


void UnitMovementSeeker::updateState(const float currentTime)
{
	// if we reach the goal before being tagged, switch to atGoal state
	if (state == running)
	{
		const float baseDistance = Vec3::distance(position(), gHomeBaseCenter);
		if (baseDistance < (radius() + gHomeBaseRadius)) state = atGoal;
	}

	// update lastRunningTime (holds off reset time)
	if (state == running)
	{
		lastRunningTime = currentTime;
	}
	else
	{
		const float resetDelay = 4;
		const float resetTime = lastRunningTime + resetDelay;
		if (currentTime > resetTime)
		{
			// xxx a royal hack (should do this internal to UnitMovement):
			OpenSteer::OpenSteerManager::queueDelayedResetPlugInXXX();
		}
	}
}

// ----------------------------------------------------------------------------
// update method for goal seeker


void UnitMovementSeeker::update(const float currentTime, const float elapsedTime)
{
	// do behavioral state transitions, as needed
	updateState(currentTime);

	// determine and apply steering/braking forces
	Vec3 steer(0, 0, 0);
	if (state == running)
	{
		steer = steeringForSeeker();
	}
	else
	{
		applyBrakingForce(gBrakingRate, elapsedTime);
	}
	applySteeringForce(steer, elapsedTime);
}


// ----------------------------------------------------------------------------
// dynamic obstacle registry
//
// xxx need to combine guts of addOneObstacle and minDistanceToObstacle,
// xxx perhaps by having the former call the latter, or change the latter to
// xxx be "nearestObstacle": give it a position, it finds the nearest obstacle
// xxx (but remember: obstacles a not necessarilty spheres!)


int UnitMovementBase::obstacleCount = -1; // this value means "uninitialized"
SOG UnitMovementBase::allObstacles;


#define testOneObstacleOverlap(radius, center)               \
{                                                            \
	float d = Vec3::distance(c, center);                    \
	float clearance = d - (r + (radius));                    \
if (minClearance > clearance) minClearance = clearance;  \
}


void UnitMovementBase::initializeObstacles(void)
{
	// start with 40% of possible obstacles
	if (obstacleCount == -1)
	{
		obstacleCount = 0;
		for (int i = 0; i < (maxObstacleCount * 0.4); i++) addOneObstacle();
	}
}


void UnitMovementBase::addOneObstacle(void)
{
	if (obstacleCount < maxObstacleCount)
	{
		// pick a random center and radius,
		// loop until no overlap with other obstacles and the home base
		float r;
		Vec3 c;
		float minClearance;
		const float requiredClearance = gSeeker->radius() * 4; // 2 x diameter
		do
		{
			r = frandom2(1.5, 4);
			c = randomVectorOnUnitRadiusXZDisk() * gMaxStartRadius * 1.1f;
			minClearance = FLT_MAX;

			for (SOI so = allObstacles.begin(); so != allObstacles.end(); so++)
			{
				testOneObstacleOverlap((**so).radius, (**so).center);
			}

			testOneObstacleOverlap(gHomeBaseRadius - requiredClearance,
				gHomeBaseCenter);
		} while (minClearance < requiredClearance);

		// add new non-overlapping obstacle to registry
		allObstacles.push_back(new SphericalObstacle(r, c));
		obstacleCount++;
	}
}


float UnitMovementBase::minDistanceToObstacle(const Vec3 point)
{
	float r = 0;
	Vec3 c = point;
	float minClearance = FLT_MAX;
	for (SOI so = allObstacles.begin(); so != allObstacles.end(); so++)
	{
		testOneObstacleOverlap((**so).radius, (**so).center);
	}
	return minClearance;
}


void UnitMovementBase::removeOneObstacle(void)
{
	if (obstacleCount > 0)
	{
		obstacleCount--;
		allObstacles.pop_back();
	}
}*/