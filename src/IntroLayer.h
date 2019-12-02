#ifndef IntroLayer_h_
#define IntroLayer_h_

#include <string>
#include <iostream>
#include <vector>
#include "Layer.h"
#include "MainLayer.h"
#include "TransitionManager.h"

#include <GL/gl.h>
#include <GL/glu.h>


class IntroLayer : public Layer
{
public:
	IntroLayer( std::string filename );
	~IntroLayer();

	void turnOn() 
	{     
        if( m_currentMode == LAYER_OFF || m_currentMode == FADING_OUT )
		{
			m_on = true;
			fadeIn = true; 
			setMode( FADING_IN );
			if( m_layerPos == 0 )
				TransitionManager::whiteWash( true );

			Layer::turnOn();
		}
	}

	void turnOff() 
	{ 
		if( m_currentMode == LAYER_ACTIVE ) //|| m_currentMode == LAYER_ACTIVE_WAITING )
		{
			MainLayer::setIntroLayersComplete( false );
			fadeIn = false; 
			m_on = false;
			Layer::turnOff();
		}
	}

	void updateData( double lastCallTime );
	
private:
	bool fadeInLayer();
	bool allLayersAre( LayerMode mode, LayerMode orMode );

	bool m_on;
	double m_random;
	static std::vector<Layer*> allLayers;

	int m_layerPos;
};

#endif