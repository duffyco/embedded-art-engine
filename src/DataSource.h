#ifndef DataSource_h_
#define DataSource_h_

#include <vector>
#include <string>

using namespace std;

enum SensorPosition
{
	FRONT, 
	MIDDLE, 
	BACK,
	OFF
};

class DataSource
{
public:
	DataSource();
	virtual ~DataSource();

	virtual vector< vector<int> > getNewData() = 0;

	virtual void run() = 0;
	virtual void start() = 0;

	virtual bool errorState() = 0;

protected:
	static int m_sensorID;
};

#endif