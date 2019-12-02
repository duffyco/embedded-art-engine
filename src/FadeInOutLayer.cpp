#include "appheaders.h"
#include "FadeInOutLayer.h"
#include <ctime>

std::vector<Layer*> FadeInOutLayer::allLayers;

FadeInOutLayer::FadeInOutLayer( std::string filename )
	: Layer( filename, LAYER_OFF )
        , m_on( false )
{
	setMode( LAYER_OFF );
	setFade( 1.0, 1000 );  // Adjust the composite value in updateData

	allLayers.push_back( this );
	m_layerPos = allLayers.size() - 1;
}


FadeInOutLayer::~FadeInOutLayer()
{
}

bool FadeInOutLayer::allLayersAre( LayerMode mode, LayerMode orMode )
{
	bool out = true;
	for( std::vector<Layer*>::iterator it = allLayers.begin(); it != allLayers.end(); ++it )
		out = (out && (  ( (*it)->getMode() == mode )  || ( (*it)->getMode()  == orMode ) ) );

	return out;
}

bool FadeInOutLayer::fadeInLayer()
{
	if( m_layerPos == 0 )
		return true;

	return ( allLayers.at( m_layerPos - 1 )->getMode() == LAYER_ACTIVE_WAITING );
}

void FadeInOutLayer::updateData( double lastCallTime )
{
	lastUpdate += lastCallTime;

	if( lastUpdate > Layer::FPS )
	{
		if( m_currentMode == LAYER_OFF )
		{
		}
		else if( fadeIn )
		{
			if( !MainLayer::allLayersAre( LAYER_OFF ) )
				return;

			if( !fadeInLayer() )
				return; 

			if( m_layerPos > 0 )	
			   fade = allLayers.at( m_layerPos - 1)->getFade() * (float) 1.1;

			currentFade += fade;
			if( currentFade > 1.0 && m_currentMode != LAYER_ACTIVE )
			{
				currentFade = 1.0;
				setMode( LAYER_ACTIVE_WAITING );

				if( allLayersAre( LAYER_ACTIVE_WAITING, LAYER_ACTIVE ) )
				{
					for( std::vector<Layer*>::iterator it = allLayers.begin(); it != allLayers.end(); ++it )
						(*it)->setMode( LAYER_ACTIVE );
				}
			}
		}
		else if( !fadeIn  )
		{
			currentFade -= fade;
			if( currentFade < 0.0) 
			{
				currentFade = 0.0;
				setMode( LAYER_OFF );
			}
		}

		lastUpdate = 0.0;
	}

}
