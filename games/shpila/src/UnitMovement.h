#ifndef UNITMOVEMENT_H_
#define UNITMOVEMENT_H_

using namespace gameplay;
using namespace OpenSteer;

//typedef std::vector<SphericalObstacle*> SOG;  // spherical obstacle group
//typedef SOG::const_iterator SOI;              // spherical obstacle iterator

typedef AbstractProximityDatabase<AbstractVehicle*> ProximityDatabase;
typedef AbstractTokenForProximityDatabase<AbstractVehicle*> ProximityToken;
typedef LQProximityDatabase<AbstractVehicle*> LQPDAV;

class UnitMovementBase : public SimpleVehicle
{
public:

	// type for a group of UnitMovementBases
	typedef std::vector<UnitMovementBase*> groupType;

	// constructor
	UnitMovementBase()
		: _target(0.0f, 0.0f, 0.0f)
		, _applyBreakingForces(false)
		, proximityToken(NULL)
	{
	}

	void init(ProximityDatabase& pd)
	{
		// allocate a token for this boid in the proximity database
		newPD(pd);
		// reset UnitMovementBase state
		reset();
	}

	// destructor
	virtual ~UnitMovementBase()
	{
		// delete this boid's token in the proximity database
		SAFE_DELETE(proximityToken);
	}


	bool rightHanded(void) const 
	{ 
		return true; 
	}

	// reset all instance state
	void reset(void);

	// per frame simulation update
	void update(const float currentTime, const float elapsedTime)
	{
		// apply steering force to our momentum
		if (_applyBreakingForces)
			applyBrakingForce(0.8f, elapsedTime);
		else
			applySteeringForce(determineCombinedSteering(elapsedTime), elapsedTime);

		// notify proximity database that our position has changed
		proximityToken->updateForNewPosition(position());
	}

	// compute combined steering force: move forward, avoid obstacles
	// or neighbors if needed, otherwise follow the path and wander
	Vec3 determineCombinedSteering(const float elapsedTime)
	{
		// move forward
		Vec3 steeringForce = forward();

		// probability that a lower priority behavior will be given a
		// chance to "drive" even if a higher priority behavior might
		// otherwise be triggered.
		const float leakThrough = 0.1f;
		{
			// otherwise consider avoiding collisions with others
			Vec3 collisionAvoidance;
			const float caLeadTime = 3.0f;
			float distToTarget = Vec3(position() - _target).length() - 0.01f;

			// find all neighbors within maxRadius using proximity database
			// (radius is largest distance between vehicles traveling head-on
			// where a collision is possible within caLeadTime seconds.)
			const float maxRadius = min((maxSpeed() * caLeadTime * 2.0f), distToTarget);
			neighbors.clear();
			proximityToken->findNeighbors(position(), maxRadius, neighbors);

			//if (leakThrough < frandom01())
			collisionAvoidance = steerToAvoidNeighbors(caLeadTime, neighbors) * 10.0f;

			// if collision avoidance is needed, do it
			if (collisionAvoidance != Vec3::zero)
			{
				steeringForce += collisionAvoidance; 
			}
			else
			{
				steeringForce += steerForSeek(_target) * 10.0f;
			}
		}

		// return steering constrained to global XZ "ground" plane
		return steeringForce.setYtoZero();
	}

	// switch to new proximity database -- just for demo purposes
	void newPD(ProximityDatabase& pd)
	{
		// delete this boid's token in the old proximity database
		SAFE_DELETE(proximityToken);

		// allocate a token for this boid in the proximity database
		proximityToken = pd.allocateToken(this);
	}

	// a pointer to this boid's interface object for the proximity database
	ProximityToken* proximityToken;

	// allocate one and share amoung instances just to save memory usage
	// (change to per-instance allocation to be more MP-safe)
	static AVGroup neighbors;

	Vec3 _target;

	bool _applyBreakingForces;

	// path to be followed by this pedestrian
	// XXX Ideally this should be a generic Pathway, but we use the
	// XXX getTotalPathLength and radius methods (currently defined only
	// XXX on PolylinePathway) to set random initial positions.  Could
	// XXX there be a "random position inside path" method on Pathway?
	//PolylinePathway* path;

	// direction for path following (upstream or downstream)
	//int pathDirection;
};

/*class UnitMovementBase : public SimpleVehicle
{
public:
	// constructor
	UnitMovementBase() { reset(); }

	// reset state
	void reset(void);

	void randomizeStartingPositionAndHeading(void);
	enum seekerState { running, tagged, atGoal };

	// for draw method
	Vec3 bodyColor;

	// xxx store steer sub-state for anotation
	bool avoiding;

	// dynamic obstacle registry
	static void initializeObstacles(void);
	static void addOneObstacle(void);
	static void removeOneObstacle(void);
	float minDistanceToObstacle(const Vec3 point);
	static int obstacleCount;
	static const int maxObstacleCount;
	static SOG allObstacles;
};


class UnitMovementSeeker : public UnitMovementBase
{
public:

	// constructor
	UnitMovementSeeker() { reset(); }

	// reset state
	void reset(void);

	// per frame simulation update
	void update(const float currentTime, const float elapsedTime);

	// is there a clear path to the goal?
	bool clearPathToGoal(void);

	Vec3 steeringForSeeker(void);
	void updateState(const float currentTime);
	Vec3 steerToEvadeAllDefenders(void);
	Vec3 XXXsteerToEvadeAllDefenders(void);
	void adjustObstacleAvoidanceLookAhead(const bool clearPath);
	void clearPathAnnotation(const float threshold,
		const float behindcThreshold,
		const Vec3& goalDirection);

	seekerState state;
	bool evading; // xxx store steer sub-state for anotation
	float lastRunningTime; // for auto-reset
};


class UnitMovementEnemy : public UnitMovementBase
{
public:

	// constructor
	UnitMovementEnemy() { reset(); }

	// reset state
	void reset(void);

	// per frame simulation update
	void update(const float currentTime, const float elapsedTime);
};*/

#endif