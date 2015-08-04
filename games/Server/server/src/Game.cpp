#include "Game.h"
#ifdef WIN32
#include <windows.h>
#endif

Game::Game()
: _respawnTime(0.0)
, _currentUnitID(0)
, _timeTicksPerMillis(0.0)
, _timeStart(0.0)
, _timeAbsolute(0.0)
, _lastTimeAbsolute(0.0)
{
	LARGE_INTEGER tps;
	QueryPerformanceFrequency(&tps);
	_timeTicksPerMillis = (double)(tps.QuadPart / 1000L);
	LARGE_INTEGER queryTime;
	QueryPerformanceCounter(&queryTime);
	if (_timeTicksPerMillis > 0.0)
	_timeStart = queryTime.QuadPart / _timeTicksPerMillis;
	_lastTimeAbsolute = getAbsoluteTime();
}

Game::~Game()
{
}

void Game::update()
{
	double elapsedTime = getElapsedTime();

	_respawnTime += elapsedTime;
	if (_respawnTime > RESPAWN_TIME)
	{
		_respawnTime = 0.0;
	}
}

double Game::getAbsoluteTime()
{
	LARGE_INTEGER queryTime;
	QueryPerformanceCounter(&queryTime);
	if (_timeTicksPerMillis > 0.0)
		_timeAbsolute = queryTime.QuadPart / _timeTicksPerMillis;

	return _timeAbsolute - _timeStart;
}

double Game::getElapsedTime()
{
	double absoluteTime = getAbsoluteTime();
	float elapsedTime = absoluteTime - _lastTimeAbsolute;
	_lastTimeAbsolute = absoluteTime;
	return elapsedTime;
}

double Game::getRespawnTime()
{
	return _respawnTime;
}