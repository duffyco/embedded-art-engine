#include "appheaders.h"
#include "VignetteLayer.h"

bool VignetteLayer::m_disabled = false;

VignetteLayer::VignetteLayer( std::string filename )
	: Layer( filename, LAYER_OFF )
        , m_on( false )
{
	m_random = unifRand( 5000, 15000 );
	setFade( 1.0, 750 );  // Adjust the composite value in updateData 0.3, 750
}


VignetteLayer::~VignetteLayer()
{
}

void VignetteLayer::updateData( double lastCallTime )
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
			if( currentFade > 1.0 && m_currentMode != LAYER_ACTIVE )
			{
				currentFade = 1.0;
				//fadeIn = false;
				//setFade( 1.0, m_random );
				setMode( LAYER_ACTIVE );
			}
		        else if ( currentFade > 1.0 )
			{
		          currentFade = 1.0;
			}
		}
		else if( !fadeIn  )
		{
			currentFade -= fade;
			if( currentFade < 0.0 && m_currentMode == FADING_OUT ) 
			{
				currentFade = 0.0;
				setMode( LAYER_OFF );
				setFade( 0.3, 750 ); 
			}
		    else if ( currentFade < 0.0 )
		    {
                currentFade = 0.0;
				fadeIn = true;
            }
		}

		lastUpdate = 0.0;
	}
}
