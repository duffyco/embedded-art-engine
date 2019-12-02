#ifndef OnLayer_h_
#define OnLayer_h_

#include <string>
#include <iostream>
#include <vector>
#include "Layer.h"

#include <GL/gl.h>
#include <GL/glu.h>


class OnLayer: public Layer
{
public:
	OnLayer( std::string filename )
		: Layer( filename, LAYER_ACTIVE )
	{
		currentFade = 1.0;
		fade = 1.0;
	}

	~OnLayer();

	void turnOn() 
	{ }

	void turnOff() 
	{ }

	void updateData( double lastCallTime )
	{ }

};

#endif