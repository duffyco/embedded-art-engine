#include "appheaders.h"
#include "IntroLayer.h"
#include <ctime>

std::vector<Layer*> IntroLayer::allLayers;

IntroLayer::IntroLayer( std::string filename )
	: Layer( filename, LAYER_OFF )
        , m_on( false )
{
	setMode( LAYER_OFF );
	setFade( 1.0, 1000 );  // Adjust the composite value in updateData

	allLayers.push_back( this );
	m_layerPos = allLayers.size() - 1;
}


IntroLayer::~IntroLayer()
{
}

bool IntroLayer::allLayersAre( LayerMode mode, LayerMode orMode )
{
	bool out = true;
	for( std::vector<Layer*>::iterator it = allLayers.begin(); it != allLayers.end(); ++it )
		out = (out && (  ( (*it)->getMode() == mode )  || ( (*it)->getMode()  == orMode ) ) );

	return out;
}

bool IntroLayer::fadeInLayer()
{
	if( m_layerPos == 0 )
		return true;

	return ( allLayers.at( m_layerPos - 1 )->getMode() == LAYER_ACTIVE_WAITING );
}

void IntroLayer::updateData( double lastCallTime )
{
	lastUpdate += lastCallTime;

	if( lastUpdate > Layer::FPS )
	{
		if( m_currentMode == LAYER_OFF )
		{
		}
		else if( fadeIn )
		{
			if( m_currentMode == LAYER_ACTIVE )
			{
				currentFade = 0.0;
				return;
			}

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
					MainLayer::setIntroLayersComplete( true );
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
