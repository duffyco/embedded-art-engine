#ifndef FadeInOutLayer_h_
#define FadeInOutLayer_h_

#include <string>
#include <iostream>
#include <vector>
#include "Layer.h"
#include "MainLayer.h"
#include "TransitionManager.h"

#include <GL/gl.h>
#include <GL/glu.h>


class FadeInOutLayer : public Layer
{
public:
	FadeInOutLayer( std::string filename );
	~FadeInOutLayer();

	void turnOn() 
	{     
        if( m_currentMode == LAYER_OFF || m_currentMode == FADING_OUT )
		{
			m_on = true;
			fadeIn = true; 
			setMode( FADING_IN );
			Layer::turnOn();
		}
	}

	void turnOff() 
	{ 
		if( m_currentMode == LAYER_ACTIVE ) //|| m_currentMode == LAYER_ACTIVE_WAITING )
		{
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