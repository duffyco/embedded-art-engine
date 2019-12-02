#ifndef Transition_Manager_h_
#define Transition_Manager_h_

#include "DataSource.h"
#include "Layer.h"
#include "DebugStream.h"

#include <vector>
#include <string>
#include <utility>

#include <boost/thread.hpp>  

using namespace std;

typedef std::pair<SensorPosition, Layer*> SensorPair;

class TransitionManager
{
public:
	static TransitionManager* getTransitionManager();

	void addSource( DataSource* ds ) 
	{ m_sources.push_back( ds ); }

	void registerLayer( Layer* layer, int sensorID, SensorPosition pos );

	vector<Layer*> getLayers()
	{ return m_layers; }

	bool isConnected()
	{ 
		return ( m_sources.size() > 0 ) && !m_sources.at( 0 )->errorState(); 
	}

	void start();
	void run();

	void setEmergencySensor( int sensorID )
	{ m_emergencySensor = sensorID; }

	void emergencyMode();

        vector<SensorPair> getObserverAt( int i )
	{ return m_observers.at( i ); }

	static void whiteWash( bool disabled );

private:

	int m_emergencySensor ;
	eTransitionMode m_mode;
	static TransitionManager* m_instance;

	vector<DataSource*> m_sources;
	vector< vector<SensorPair> > m_observers;

	vector<Layer*> m_layers;

	boost::thread* m_workerThread;

	DebugStream m_debug;

	TransitionManager();
	~TransitionManager();
};

#endif
