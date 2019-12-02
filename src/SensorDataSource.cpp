#include "SensorDataSource.h"
#include "RenderEngine.h"

#include <boost/thread.hpp>  
#include <boost/lexical_cast.hpp>

#include <iostream>
#include <string>
#include <utility>
#include <sstream>


SensorDataSource::SensorDataSource( string sensorName, int baudrate, vector<SensorDataSourceType> types )
	: m_errorState( false )
        , m_sensorName( sensorName )
        , m_baudrate( baudrate )
        , m_usbConnection( sensorName, baudrate )
		, m_debug( DATA_SOURCE_FILENAME )
		, m_types( types )
{
	m_workerThread = NULL;
	m_usbConnection.setTimeout(boost::posix_time::seconds(5));
	m_sensorID++;
}

SensorDataSource::~SensorDataSource()
{
	m_usbConnection.close();
	m_workerThread->join();
	delete m_workerThread;
}

void SensorDataSource::start()
{
	m_workerThread = new thread( &SensorDataSource::run, this );
}

void SensorDataSource::run()
{
	while( true )
	{
		try
		{
                        m_debug.print( "Writing to USB" );
			m_usbConnection.writeString( "ACK\n" );

			string token, recv = m_usbConnection.readStringUntil("\r\n");


			istringstream iss( recv );	

			RenderEngine::setStatus( recv );

			std::getline(iss, token, ':');
			int sensorID = boost::lexical_cast<int>( token );
			std::getline(iss, token, ':'); 
			int sensorData = boost::lexical_cast<int>( token ) ;

			vector<int> sensorPos;

			boost::mutex::scoped_lock scoped_lock( m_mutex );

			try
			{
				std::ostringstream os;
                os << "Data from: " << sensorID << " - " << sensorData;
				m_debug.print( os.str() );
                            
				vector<int>& sensorPos = m_incomingData.at( sensorID );
				sensorPos.push_back( convert( sensorData, m_types.at( sensorID ) ) );
			}
			catch( out_of_range ex )
			{
				sensorPos.push_back( convert( sensorData, m_types.at( sensorID ) ) );
				m_incomingData.push_back( sensorPos );
			}

			
		}
		catch(boost::system::system_error& e)
		{
                    if( !m_errorState )
                    {
						std::ostringstream os;
						os << "Failure: Sensor: "<< m_sensorID << " error message: " << e.what() << std::endl;                    
						os << "Sensor " << m_sensorID << " is offline.";
						m_debug.exception( os.str() );
                    }
                    
                    m_errorState = true;
                    
                    try
                    {
                        m_usbConnection.close();
                        m_usbConnection.open( m_sensorName, m_baudrate );

						if( !m_usbConnection.isOpen() )
							throw std::exception();

						std::ostringstream os;
                        os << "Reconnected to USB...Success";
						m_debug.exception( os.str() );
                        m_errorState = false;
                    }
                    catch( ... )
                    {}
		}
		catch( ... )
		{
                    if( !m_usbConnection.isOpen() )
                    {
						if( !m_errorState )
						{
							std::ostringstream os;
							os << "Failure: Sensor: "<< m_sensorID << " error message: UNKNOWN" << std::endl;                    
							os << "Sensor " << m_sensorID << " is offline.";
							m_debug.exception( os.str() );
			            }

						m_errorState = true;
						try
						{
							m_usbConnection.close();
							m_usbConnection.open( m_sensorName, m_baudrate );

							if( !m_usbConnection.isOpen() )
								throw std::exception();

							std::ostringstream os;
							os << "Reconnected to USB...Success";
							m_debug.exception( os.str() );
							m_errorState = false;
						}
						catch( ... )
						{}
                    }
		}
	}
}

SensorPosition SensorDataSource::convert( int sensorData, SensorDataSourceType type )
{
	switch( type )
	{
	case DISTANCE_SENSOR:
		return convertDistanceSensor( sensorData );
	case PRESSURE_SENSOR:
		return convertPressureSensor( sensorData );
	}
}

SensorPosition SensorDataSource::convertPressureSensor( int sensorData )
{
		if( sensorData > 60 )
			return FRONT ;
		else 
			return OFF;		

}

SensorPosition SensorDataSource::convertDistanceSensor( int sensorData )
{
                if( sensorData < 10 )
                      return OFF;
		if( sensorData < 100 )
			return BACK;
		else if( sensorData < 200 )
			return MIDDLE;
		else if( sensorData < 300 )
			return FRONT ;
		else if ( sensorData > 300 ) 
			return OFF;		
}
