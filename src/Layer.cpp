#include "appheaders.h"

#include "Layer.h"
#include "RenderEngine.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <stdexcept>

int Layer::FPS = 33;
int Layer::FPSPrint = 0;

Layer::Layer( std::string filename, LayerMode startMode )
        : m_filename( filename )
        , fade( 0.01 )
		, m_debug( LAYER_FILENAME )
	, currentFade( 0.0 )
	, polyW( 0.0)
	, polyH(0.0)
	, lastUpdate( FPS )
	, m_currentMode( startMode )
{
	corona::Image* image = corona::OpenImage( filename.c_str(), corona::PF_R8G8B8A8 );

	if( image == NULL )
	{
		std::cout << "File Not Found: " << filename << std::endl;
		throw new std::exception;
	}

	GLint width = image->getWidth();
	GLint height = image->getHeight();
	unsigned char* pixels = (unsigned char*) image->getPixels();

	polyW = (width / 2.f);
	polyH = (height / 2.f);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
   	glPushMatrix();
		
	delete image;
}

void Layer::preformFadeOperation( double lastCallTime )
{
	if( FPSPrint > 1000 )
	{
		string str( LAYER_FILENAME );
		str += "FPS";
		ifstream file( str.c_str() );
		if( file.good() )
		{
			DebugStream fpsds( str );

			std::ostringstream os;
			os << "Lastcall: " << lastCallTime;
			fpsds.print( os.str() );
		}
		FPSPrint = 0;
	}
	else
		FPSPrint++;
	
	if( currentFade <= 0 )
		return;
	
	glTranslatef(polyW, polyH, 0.0 );
	glScalef( 1.0f, -1.0f, 1.0f );
	glTranslatef(-polyW, -polyH, 0.0 );

	glBindTexture( GL_TEXTURE_2D, texture );
	glEnable( GL_TEXTURE_2D );

	glBegin(GL_QUADS);
		glColor4f( 1.0, 1.0, 1.0, currentFade );

		int zeroH = ( 1 - ( SCREEN_RES_HEIGHT / 2 ) ) + polyH;
		int zeroW = 0; //SCREEN_RES_WIDTH / 2 - polyW;

		glTexCoord2f(0.0, 0.0); glVertex3f(zeroW,zeroH,0);
        glTexCoord2f(0.0, 1.0); glVertex3f(zeroW,zeroH+ polyH*2,0);
        glTexCoord2f(1.0, 1.0); glVertex3f(zeroW + polyW*2,zeroH + polyH*2,0);
        glTexCoord2f(1.0, 0.0); glVertex3f(zeroW + polyW*2,zeroH,0);
	glEnd();
	
}
/*
void Layer::updateData( double lastCallTime )
{
	lastUpdate += lastCallTime;

	if( lastUpdate > Layer::FPS )
	{
		if( fadeIn )
		{
			currentFade += (fade + 0.02);
			if( currentFade > 1.0 )
				currentFade = 1.0;				
		}
		else 
		{
			currentFade -= fade;
			if( currentFade < 0.0 )
				currentFade = 0.0;
		}

		lastUpdate = 0.0;
	}

}
*/
void Layer::setFade( float infade, double intime )
{
	fade = infade / (intime / 16 );
}
