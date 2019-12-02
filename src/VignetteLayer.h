#ifndef VignetteLayer_h_
#define VignetteLayer_h_

#include <string>
#include <iostream>
#include "Layer.h"

#include <GL/gl.h>
#include <GL/glu.h>


class VignetteLayer : public Layer
{
public:
	VignetteLayer( std::string filename );
	~VignetteLayer();

	void turnOn() 
	{     
          if( ( m_currentMode == LAYER_OFF || m_currentMode == FADING_OUT ) && !m_disabled )
		  {
			fadeIn = true; 
			setMode( FADING_IN );

			Layer::turnOn();		
		  }
	}

	void turnOff() 
	{ 
		if( m_currentMode == LAYER_ACTIVE || ( ( m_currentMode == FADING_IN ) && m_disabled ) )
		{
			fadeIn = false; 
            setFade( 1.0, 1500 ); //500
			setMode( FADING_OUT );

			Layer::turnOff();
		}
	}

	void updateData( double lastCallTime );

	static void setDisabled( bool disabled )
	{ m_disabled = disabled; }

private:
	bool m_on;
	static bool m_disabled;
	double m_random;
};

#endif
