#ifndef MainTopLayer_h_
#define MainTopLayer_h_

#include <string>
#include <iostream>
#include <vector>
#include "Layer.h"

#include <GL/gl.h>
#include <GL/glu.h>


class MainTopLayer : public Layer
{
public:
	MainTopLayer( std::string filename );
	~MainTopLayer();

	void turnOn() 
	{     
          if( m_currentMode == LAYER_OFF || m_currentMode == FADING_OUT )
		{
			std::cout << "Layer: " << m_filename << " - ON " << std::endl;
			fadeIn = true; 
			setMode( FADING_IN );
		}
	}

	void turnOff() 
	{ 
		if( m_currentMode == LAYER_ACTIVE || m_currentMode == FADING_IN || m_currentMode == LAYER_FADED_OUT )
		{
            std::cout << "Layer: " << m_filename << " - OFF " << std::endl;
			fadeIn = false; 
			setMode( LAYER_OFF );
		}
	}

	void updateData( double lastCallTime );
	void setMode( LayerMode newMode )
	{ m_currentMode = newMode; }

	static LayerMode getMode()
	{ return m_currentMode; }

private:
	static LayerMode m_currentMode;
	bool m_on;
};

#endif