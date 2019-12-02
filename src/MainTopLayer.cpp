#include "appheaders.h"
#include "MainTopLayer.h"
#include <ctime>

LayerMode MainTopLayer::m_currentMode = LAYER_OFF;

MainTopLayer::MainTopLayer( std::string filename )
	: Layer( filename )
        , m_on( false )
{
	setFade( 1.0, 250 );  // Adjust the composite value in updateData
}


MainTopLayer::~MainTopLayer()
{
}

void MainTopLayer::updateData( double lastCallTime )
{
	lastUpdate += lastCallTime;

	if( lastUpdate > Layer::FPS )
	{
		if( m_currentMode == LAYER_OFF )
		{
		}
		else if( fadeIn )
		{
 			currentFade += fade;
			if( currentFade > 1.0  )
			{
				currentFade = 1.0;
				fadeIn = false;
				setFade( 1.0, 2500 );
				setMode( LAYER_ACTIVE );
			}
		}
		else if( !fadeIn  )
		{
			currentFade -= fade;
			
			if ( currentFade < 0.0 )
            {
                currentFade = 0.0;
				setMode( LAYER_FADED_OUT );
				setFade( 1.0, 150 );
            }
		}

		lastUpdate = 0.0;
	}

}