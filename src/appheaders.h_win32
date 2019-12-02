#ifndef Appheaders_h_
#define Appheaders_h_

#include <Windows.h>
#include <cstdlib>
#include <ctime>
#include <string>

static const char* DATA_SOURCE_FILENAME = "C:/temp/sensordata.txt";
static const char* TRANSITION_MANAGER_FILENAME = "C:/temp/transitionmanager.txt";
static const char* LAYER_FILENAME = "C:/temp/layer.txt";

static double unifRand( double a, double b )
{
	return (b-a) * ( rand() / double( RAND_MAX ) ) + a;
}

static void ThreadSleep( int ms )
{
	::Sleep( ms );
}


static __int64 previousCounter;

static void destroyPreviousCounter()
{
}

static void initPreviousCounter()
{
	previousCounter = 0;
}

static double durationFromOS()
{
    __int64 counter;
    QueryPerformanceCounter( (LARGE_INTEGER*)&counter );

    __int64 frequency;
    QueryPerformanceFrequency( (LARGE_INTEGER*)&frequency );

	__int64 diff = counter - previousCounter;

	double callDuration = (double) diff * 1000.0 / frequency;

	previousCounter = counter;
	
	return callDuration;
}

enum eTransitionMode
{
	NOT_ACTIVE,
	ACTIVE,
};



#endif