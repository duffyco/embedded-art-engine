#ifndef RenderEngine_h_
#define RenderEngine_h_

#include <string>
#include <vector>
#include "Layer.h"

#include <boost/thread.hpp>

using namespace std;

const int SCREEN_RES_WIDTH = 1920;
const int SCREEN_RES_HEIGHT = 1080;

class RenderEngine
{
	public:
		RenderEngine();
		~RenderEngine();

		void init( vector<Layer*> inLayers );
		void render();
		void resize(int w, int h);
		void idle();

		void adjust( int attribute, float value );

		static string getStatus()
		{
			boost::mutex::scoped_lock scoped_lock( mutex );
			return m_status;
		}

		static void setStatus( string newStatus )
		{
			boost::mutex::scoped_lock scoped_lock( mutex );
			m_status = newStatus;
		}

		bool getDebug()
		{ return m_debugOn; }

		void debugOn( bool on )
		{ m_debugOn = on; }

	private:
		void print(int x, int y,int z, std::string str);
		static std::string m_status;
		static boost::mutex mutex;
		bool m_debugOn;

		std::vector<Layer*> layers;

		double durationSinceLastCall();
};


#endif
