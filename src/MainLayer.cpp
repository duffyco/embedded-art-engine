#include "appheaders.h"
#include "MainLayer.h"
#include <ctime>

bool MainLayer::m_introLayersComplete = false;
std::vector<Layer*> MainLayer::allLayers;

MainLayer::MainLayer( std::string filename )
	: Layer( filename, LAYER_OFF )
        , m_on( false )
	, hideTime( 0 )
	, hideDelay( unifRand( 0, 5000 ) )
{
	m_random = unifRand( 5000, 12000 );

	allLayers.push_back( this );
	m_layerPos = allLayers.size() - 1;
}


MainLayer::~MainLayer()
{
}

bool MainLayer::allLayersAre( LayerMode mode )
{
	bool out = true;
	for( std::vector<Layer*>::iterator it = allLayers.begin(); it != allLayers.end(); ++it )
		out = (out && (*it)->getMode() == mode );

	return out;
}


void MainLayer::updateData( double lastCallTime )
{
	lastUpdate += lastCallTime;

	if( lastUpdate > Layer::FPS )
	{
		if( m_on && m_introLayersComplete && ( m_currentMode != LAYER_ACTIVE ) )
		{
			currentFade = 1.0;
			fadeIn = false;
			setFade( 1.0, m_random );
			setMode( LAYER_ACTIVE );
			TransitionManager::whiteWash( false );
		}
		else if( m_currentMode == LAYER_OFF )
		{
		}
		else if( fadeIn )
		{
			currentFade += fade;
			if ( currentFade > 1.0 )
			{
				currentFade = 1.0;
		                fadeIn = false;
			}
		}
		else if( !fadeIn  )
		{
			currentFade -= fade;
			if( currentFade < 0.0 && m_currentMode == FADING_OUT ) 
			{
				currentFade = 0.0;
				setMode( LAYER_OFF );

				if( allLayersAre( LAYER_OFF ) )
					TransitionManager::whiteWash( false );
			}

		    else if ( currentFade < 0.0 )
		    {
		        currentFade = 0.0;
		
				if( hideTime > hideDelay )
				{
		   			fadeIn = true;
	   				hideTime = 0;
				}
				else
				{
					hideTime += lastUpdate;
				}
		    }
		}

		lastUpdate = 0.0;
	}

}
