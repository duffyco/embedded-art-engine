#include "appheaders.h"
#include "SensorDataSource.h"
#include "FadeInOutLayer.h"
#include "RenderEngine.h"
#include "TransitionManager.h"
#include "MainLayer.h"
#include "OnLayer.h"
#include "IntroLayer.h"
#include "VignetteLayer.h"

const bool SENSORS_DISABLED = true;

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <vector>
#include <string>
#include <ctime>
#include <boost/thread.hpp>

static RenderEngine re;
static TransitionManager* trm = NULL;
static boost::thread* workerThread = NULL ;

void connectUSB()
{
	std::vector<SensorDataSourceType> sensorTypes;
	sensorTypes.insert( sensorTypes.end(), DISTANCE_SENSOR );
	sensorTypes.insert( sensorTypes.end(), DISTANCE_SENSOR );
	sensorTypes.insert( sensorTypes.end(), DISTANCE_SENSOR );
	sensorTypes.insert( sensorTypes.end(), PRESSURE_SENSOR );

	while( true )
	{
		TransitionManager* tm = TransitionManager::getTransitionManager();

		if( !tm->isConnected() )
		{
			try
			{
				if( !SENSORS_DISABLED ) 
				{
					TransitionManager::getTransitionManager()->addSource( new SensorDataSource( "/dev/ttyUSB0", 9600, sensorTypes ) );
					TransitionManager::getTransitionManager()->start();
				}
				break;
			}
			catch( ... )
			{

				ThreadSleep( 1000 );
			}
		}
	}
}

void init()
{
	trm  = TransitionManager::getTransitionManager();

	trm->registerLayer( new OnLayer( "./Images/blackbg.png" ), 0, OFF );
	trm->registerLayer( new OnLayer( "./Images/blackbg.png" ), 1, OFF );
	trm->registerLayer( new OnLayer( "./Images/blackbg.png" ), 2, OFF );
	trm->registerLayer( new OnLayer( "./Images/STOOL-1.png" ), 3, OFF );

	trm->registerLayer( new MainLayer( "./Images/vignette-sensor0-zone1.png"), 3, FRONT );
	trm->registerLayer( new MainLayer( "./Images/vignette-sensor1-zone1.png"), 3, FRONT );
	trm->registerLayer( new MainLayer( "./Images/vignette-sensor2-zone1.png"), 3, FRONT );
	trm->registerLayer( new MainLayer( "./Images/STOOL-4.png" ), 3, FRONT );
	trm->registerLayer( new MainLayer( "./Images/vignette-sensor0-zone2.png"), 3, FRONT );
	trm->registerLayer( new MainLayer( "./Images/vignette-sensor1-zone2.png"), 3, FRONT );
	trm->registerLayer( new MainLayer( "./Images/vignette-sensor2-zone2.png"), 3, FRONT );
	trm->registerLayer( new MainLayer("./Images/STOOL-7.png" ), 3, FRONT);
	trm->registerLayer( new MainLayer( "./Images/vignette-sensor0-zone3.png"), 3, FRONT );
	trm->registerLayer( new MainLayer( "./Images/vignette-sensor1-zone3.png"), 3, FRONT );
	trm->registerLayer( new MainLayer( "./Images/vignette-sensor2-zone3.png"), 3, FRONT );
	trm->registerLayer( new MainLayer("./Images/STOOL-9.png" ), 3, FRONT);

	trm->registerLayer( new IntroLayer( "./Images/STOOL-2.png" ), 3, FRONT );
	trm->registerLayer( new IntroLayer( "./Images/STOOL-3.png" ), 3, FRONT );
	trm->registerLayer( new IntroLayer( "./Images/STOOL-4.png" ), 3, FRONT );
	trm->registerLayer( new IntroLayer("./Images/STOOL-5.png" ), 3, FRONT);
	trm->registerLayer( new IntroLayer("./Images/STOOL-6.png" ), 3, FRONT);
	trm->registerLayer( new IntroLayer("./Images/STOOL-7.png" ), 3, FRONT);
	trm->registerLayer( new IntroLayer("./Images/STOOL-8.png" ), 3, FRONT);
	trm->registerLayer( new IntroLayer("./Images/STOOL-9.png" ), 3, FRONT);

	trm->registerLayer( new FadeInOutLayer( "./Images/STOOL-2.png" ), 4, FRONT );
	trm->registerLayer( new FadeInOutLayer( "./Images/STOOL-3.png" ), 4, FRONT );
	trm->registerLayer( new FadeInOutLayer( "./Images/STOOL-4.png" ), 4, FRONT );
	trm->registerLayer( new FadeInOutLayer("./Images/STOOL-5.png" ), 4, FRONT);
	trm->registerLayer( new FadeInOutLayer("./Images/STOOL-6.png" ), 4, FRONT);
	trm->registerLayer( new FadeInOutLayer("./Images/STOOL-7.png" ), 4, FRONT);
	trm->registerLayer( new FadeInOutLayer("./Images/STOOL-8.png" ), 4, FRONT);
	trm->registerLayer( new FadeInOutLayer("./Images/STOOL-9.png" ), 4, FRONT);




	trm->registerLayer( new VignetteLayer( "./Images/vignette-sensor0-zone1.png"), 0, FRONT );
	trm->registerLayer( new VignetteLayer( "./Images/vignette-sensor1-zone1.png"), 1, FRONT );
	trm->registerLayer( new VignetteLayer( "./Images/vignette-sensor2-zone1.png"), 2, FRONT );
	trm->registerLayer( new VignetteLayer( "./Images/vignette-sensor0-zone2.png"), 0, MIDDLE );
	trm->registerLayer( new VignetteLayer( "./Images/vignette-sensor1-zone2.png"), 1, MIDDLE );
	trm->registerLayer( new VignetteLayer( "./Images/vignette-sensor2-zone2.png"), 2, MIDDLE );
	trm->registerLayer( new VignetteLayer( "./Images/vignette-sensor0-zone3.png"), 0, BACK );
	trm->registerLayer( new VignetteLayer( "./Images/vignette-sensor1-zone3.png"), 1, BACK );
	trm->registerLayer( new VignetteLayer( "./Images/vignette-sensor2-zone3.png"), 2, BACK );


	trm->setEmergencySensor( 3 );

	re.init( trm->getLayers() );


	trm->start();
}

void render()
{
	re.render();
}

void resize( int w, int h)
{
	re.resize( w, h );
}

void idle()
{
	re.idle();
}

static bool sensor0 = false;
static bool sensor1 = false;
static bool sensor2 = false;

void key(unsigned char key, int x, int y)
{
    if (key == 'q') exit(0);
	else if( key == 'd' ) re.debugOn( !re.getDebug() );
	else if( key == 'b' ) Layer::decreaseFPS( 1 );
	else if( key == 'v' ) Layer::increaseFPS( 1 );
	else if( key == '1' )
	{	
		vector<SensorPair> sensorObservers = trm->getObserverAt( 3 );
	
		for( vector<SensorPair>::iterator it = sensorObservers.begin(); it != sensorObservers.end(); ++it )
			( (Layer*) it->second )->turnOn();

	}
	else if( key == '2' )
	{
		vector<SensorPair> sensorObservers = trm->getObserverAt( 3 );
	
		for( vector<SensorPair>::iterator it = sensorObservers.begin(); it != sensorObservers.end(); ++it )
			( (Layer*) it->second )->turnOff();

	}
	else if( key == '3' )
	{	
		vector<SensorPair> sensorObservers = trm->getObserverAt( 4 );
	
		for( vector<SensorPair>::iterator it = sensorObservers.begin(); it != sensorObservers.end(); ++it )
			( (Layer*) it->second )->turnOn();

	}
	else if( key == '4' )
	{
		vector<SensorPair> sensorObservers = trm->getObserverAt( 4 );
	
		for( vector<SensorPair>::iterator it = sensorObservers.begin(); it != sensorObservers.end(); ++it )
			( (Layer*) it->second )->turnOff();

	}

	else if( key == '7' )
	{
		vector<SensorPair> sensorObservers = trm->getObserverAt( 0 );
	
		for( vector<SensorPair>::iterator it = sensorObservers.begin(); it != sensorObservers.end(); ++it )
		{
			if( sensor0 )
			    ( (Layer*) it->second )->turnOff();
			else 
			   ( (Layer*) it->second )->turnOn();
		}

		sensor0 = !sensor0;

	}
	else if( key == '8' )
	{	
		vector<SensorPair> sensorObservers = trm->getObserverAt( 1 );
	
		for( vector<SensorPair>::iterator it = sensorObservers.begin(); it != sensorObservers.end(); ++it )
		{
			if( sensor1 )
			    ( (Layer*) it->second )->turnOff();
			else 
			   ( (Layer*) it->second )->turnOn();
		}
		sensor1 = !sensor1;
	}

	else if( key == '9' )
	{
		vector<SensorPair> sensorObservers = trm->getObserverAt( 2 );
	
		for( vector<SensorPair>::iterator it = sensorObservers.begin(); it != sensorObservers.end(); ++it )
		{
			if( sensor2 )
			    ( (Layer*) it->second )->turnOff();
			else 
			   ( (Layer*) it->second )->turnOn();
		}
		sensor2 = !sensor2;

	}


}

int main(int argc, char *argv[])
 {
	 srand( time( 0 ) );

     glutInit(&argc, argv);
     glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	 glutInitWindowSize(1920, 1080);

     glutCreateWindow("Texture demo - [q]uit");
     glutSetCursor( GLUT_CURSOR_NONE );

	 init();
	 workerThread = new thread( connectUSB );


	 glutDisplayFunc(render);
     glutReshapeFunc(resize);
	 glutIdleFunc(idle );
     glutKeyboardFunc(key);
	 glutFullScreen();


     glutMainLoop();
 
     return 0;
 }
