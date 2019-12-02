#ifndef SensorDataSource_h_
#define SensorDataSource_h_

#include "appheaders.h"
#include "TimeoutSerial.h"
#include "DataSource.h"
#include "DebugStream.h"

#include <boost/thread.hpp>  

#include <string>
#include <vector>
#include <utility>

using namespace std;
using namespace boost;

enum SensorDataSourceType
{
	DISTANCE_SENSOR,
	PRESSURE_SENSOR
};

class SensorDataSource : public DataSource
{
public:
	SensorDataSource( string comport, int baudrate, vector<SensorDataSourceType> types );
	~SensorDataSource();

	vector< vector<int> > getNewData()
	{ 
		boost::mutex::scoped_lock scoped_lock( m_mutex );
		vector<	vector<int> > outData = m_incomingData; 
		m_incomingData.clear();
		return outData;
	}

	void run();
	void start();

	bool errorState()
	{ return m_errorState; }

private:
	SensorPosition convert( int sensorData, SensorDataSourceType type );
	SensorPosition convertPressureSensor( int sensorData );
	SensorPosition convertDistanceSensor( int sensorData );

	TimeoutSerial m_usbConnection;
	string m_sensorName;
    int m_baudrate;
    DebugStream m_debug;

    vector<	vector<int> > m_incomingData;
	boost::thread* m_workerThread;
	boost::mutex m_mutex;
    bool m_errorState;
	vector<SensorDataSourceType>  m_types;
};

#endif
