#include "appheaders.h"

#include "RenderEngine.h"
#include "Layer.h"
#include "TransitionManager.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <stdexcept>
#include <ctime>
#include <ostream>

using namespace std;

std::string RenderEngine::m_status = "";
boost::mutex RenderEngine::mutex;

RenderEngine::RenderEngine()
	: m_debugOn( false )
{
}

RenderEngine::~RenderEngine()
{
	for(std::vector<Layer*>::iterator it = layers.begin(); it != layers.end(); ++it) 
	{
		delete *it;
	}
}

void RenderEngine::init( vector<Layer*> inLayers )
{
    glShadeModel(GL_SMOOTH);
	layers = inLayers;
}

void RenderEngine::adjust( int attribute, float value )
{
}

double RenderEngine::durationSinceLastCall()
{
    return durationFromOS();
}

void RenderEngine::render()
{
	double lastCall = durationSinceLastCall();
    
	glClearColor( 0.0, 0.0, 0.0, 0.0 ); 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA,  GL_ONE_MINUS_SRC_ALPHA );

	for(std::vector<Layer*>::iterator it = layers.begin(); it != layers.end(); ++it) {
		glLoadIdentity();
		gluOrtho2D( 0, (GLsizei) SCREEN_RES_WIDTH, 0, (GLsizei) SCREEN_RES_HEIGHT );
		(*it)->updateData( lastCall );
		(*it)->preformFadeOperation( lastCall );
	}

	    std::ostringstream ostr;
        
	if( m_debugOn ) {
		if( !TransitionManager::getTransitionManager()->isConnected() )
			ostr << "***NOT CONNECTED.  PLEASE ATTACH SENSOR.***";

		ostr << "Lastcall: " << lastCall << " i: " << "\n";
	}

    print( 1000, 1000, 0, ostr.str() );
	glFlush();
	glutSwapBuffers();

}

void RenderEngine::idle()
{
	glutPostRedisplay();
}

void RenderEngine::resize( int w, int h)
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

void RenderEngine::print(int x, int y,int z, std::string str)
{
    glBindTexture(GL_TEXTURE_2D,0);
    glColor3f(0.5f, 0.5f, 0.5f); 
    //set the position of the text in the window using the x and y coordinates
    glRasterPos2f(x,y);
    //get the length of the string to display

    const char* cStr = str.c_str();

    //loop to display character by character
    for (int i = 0; i < str.length(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,cStr[i]);
    }
}
