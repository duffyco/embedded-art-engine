#ifndef DebugStream_h_
#define DebugStream_h_

#include <string>
#include <fstream>

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace boost::posix_time;
using namespace std;

class DebugStream
{
public:
	DebugStream( string filename )
		: m_filename( filename )
	{
		std::string::size_type pos = filename.find_last_of( "." );
		if( std::string::npos != pos )
		{
			m_expName = filename.substr( 0, pos ) + "-EXCEPTION." + filename.substr( pos +1, filename.size() );
		}

	}

	~DebugStream()
	{
	}

	void exception( string exceptionData )
	{
		ofstream output;
		output.open( m_expName.c_str(), (ios_base::out | ios_base::app ) );		
		output << to_iso_extended_string( second_clock::universal_time() ) << ": " << exceptionData << std::endl;
		output.close();
	}

	void print( string debugStatement )
	{
		ofstream output;
		output.open( m_filename.c_str(), (ios_base::out | ios_base::app ) );		
		output << to_iso_extended_string( second_clock::universal_time() ) << ": " << debugStatement << std::endl;
		output.close();
	}

private:
	string m_filename;
	string m_expName;
};

#endif