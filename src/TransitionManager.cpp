#include "TransitionManager.h"
#include <cmath>
#include <utility>
#include "VignetteLayer.h"

TransitionManager* TransitionManager::m_instance = NULL;

TransitionManager* TransitionManager::getTransitionManager()
{
	if( m_instance == NULL )
		m_instance = new TransitionManager();

	return m_instance;
}


TransitionManager::TransitionManager()
	: m_debug( TRANSITION_MANAGER_FILENAME )
{
	m_workerThread = NULL;
}

TransitionManager::~TransitionManager()
{
	if( m_layers.size() > 0 )
	{
		for( vector<Layer*>::iterator it = m_layers.begin(); it != m_layers.end(); ++it )
			delete *it;
	}

	if( m_instance != NULL )
	{
		delete m_instance;
		m_instance = NULL;
	}
	
	if( m_workerThread != NULL )
	{
		delete m_workerThread;
		m_workerThread = NULL;
	}
}

void TransitionManager::start()
{
	m_workerThread = new boost::thread( &TransitionManager::run, this );

	for( vector<DataSource*>::iterator it = m_sources.begin(); it != m_sources.end(); ++it )
		(*it)->start();
}

void TransitionManager::whiteWash( bool disabled )
{
	vector<Layer*> layers = m_instance->getLayers();

	for( vector<Layer*>::iterator it = layers.begin(); it != layers.end(); ++it )
	{
		VignetteLayer* vl = dynamic_cast<VignetteLayer*>( *it );

		if( vl != 0 )
		{
			vl->turnOff();
			vl->setDisabled( disabled );
		}
	}
}

void TransitionManager::emergencyMode()
{
	vector<SensorPair> sensorObservers = m_observers.at( m_emergencySensor );

	for( vector<SensorPair>::iterator it = sensorObservers.begin(); it != sensorObservers.end(); ++it )
		( (Layer*) it->second )->turnOn();
}

void TransitionManager::run()
{
	while( true )
	{
		try
		{
			ThreadSleep( 1000 );

			for( std::vector<DataSource*>::iterator allSensorsIT = m_sources.begin(); allSensorsIT != m_sources.end(); ++allSensorsIT) 
			{
				vector< vector<int> > sensors = (*allSensorsIT)->getNewData();

				if( ( *allSensorsIT )->errorState() )
				{
					emergencyMode();
					continue;
				}

				if( sensors.size() == 0 )
					continue;
                
				// For Each Physical Sensor
				int i=0;
				for( vector< vector<int> >::iterator aSensorIT = sensors.begin(); aSensorIT != sensors.end(); ++aSensorIT) 
				{
					vector<int> sensorData = *aSensorIT;
					vector<SensorPair> sensorObservers; 

					try
					{
						sensorObservers = m_observers.at( i );
					}
					catch( out_of_range oor )
					{
						i++;
						continue;
					}

					double stdDeviation = 0;
					double avg = 0;


					std::ostringstream os;
					os << "Data [" << sensorData.size() << "]: ";
                                
					// For Each Piece of Sensor Data
					for( vector<int>::iterator sensorDataIT = sensorData.begin(); sensorDataIT != sensorData.end(); ++sensorDataIT )
					{
						os << *sensorDataIT << ", ";
						stdDeviation += pow( *sensorDataIT, 2.0 );
						avg += *sensorDataIT;
					}
                                
					m_debug.print( os.str() );

					if( sensorData.size() == 0 )
						continue;

					avg = avg / sensorData.size();
					stdDeviation = sqrt( (double) stdDeviation / sensorData.size() );

					SensorPosition activeData =  (SensorPosition)( (int)floor(avg) );

					os.clear();
					 
					os << "\nSensor: " << i << " :  Average [" << avg << "] StdDev ["<< stdDeviation << "] Active Data [" << activeData << "]";
					
					m_debug.print( os.str() );

					// Go through Observers for that Sensor
					for( vector<SensorPair>::iterator sensorPairIT = sensorObservers.begin(); sensorPairIT != sensorObservers.end(); ++sensorPairIT )
					{
						SensorPair sPair = *sensorPairIT;

						if( sPair.first != activeData )
							( (Layer*) sPair.second)->turnOff();
						else
							( (Layer*) sPair.second)->turnOn();
					}				

					i++;
				}
			}
		}
		catch( ... )
		{
			m_debug.exception( "Unknown Exception in Main Loop" );
		}
	}
}

void TransitionManager::registerLayer( Layer* layer, int sensorID, SensorPosition pos )
{
	m_layers.push_back( layer );

	SensorPair newObserver( pos, layer);

	try
	{
		vector<SensorPair>& region = m_observers.at( sensorID );		
		region.push_back( newObserver );
	}
	catch( std::out_of_range ex )
	{
		vector<SensorPair> region;
		region.push_back( newObserver );
		m_observers.push_back( region );
	}
}
