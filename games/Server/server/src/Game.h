#ifndef GAME____H_
#define GAME____H_

#define RESPAWN_TIME 30000

class Game
{
public:
	Game();
	~Game();

	void update();
	double getAbsoluteTime();
	double getElapsedTime();
	double getRespawnTime();
private:
	double _respawnTime;
	int _currentUnitID;
	double _timeTicksPerMillis;
	double _timeStart;
	double _timeAbsolute;
	double _lastTimeAbsolute;
};

#endif