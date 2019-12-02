#ifndef MainLayer_h_
#define MainLayer_h_

#include <string>
#include <iostream>
#include <vector>
#include "Layer.h"
#include "TransitionManager.h" 

#include <GL/gl.h>
#include <GL/glu.h>


class MainLayer : public Layer
{
public:
	MainLayer( std::string filename );
	~MainLayer();

	void turnOn() 
	{     
          if( m_currentMode == LAYER_OFF || m_currentMode == FADING_OUT )
		  {
				m_on = true;
				Layer::turnOn();
		  }
	}

	void turnOff() 
	{ 
		if( m_currentMode == LAYER_ACTIVE || m_currentMode == FADING_IN)
		{
			fadeIn = false; 
			m_on = false;
            setFade( 0.5, 1000 );
			setMode( FADING_OUT );
			Layer::turnOff();
			TransitionManager::whiteWash( true );
		}
	}

	void updateData( double lastCallTime );

	static void setIntroLayersComplete( bool isComplete )
	{ m_introLayersComplete = isComplete; }

	static std::vector<Layer*> allLayers;

	static bool allLayersAre( LayerMode mode );

private:

	int m_layerPos;

	double hideTime;
   	double hideDelay;
	bool m_on;
	double m_random;
	static bool m_introLayersComplete;
};

#endif
