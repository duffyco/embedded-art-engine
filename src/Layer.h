#ifndef Layer_h_
#define Layer_h_

#include "appheaders.h"
#include "DebugStream.h"

#include <string>

#include <GL/gl.h>
#include <GL/glu.h>

#include <corona.h>

enum LayerMode
{
	FADING_IN,
	LAYER_ACTIVE_WAITING,
	LAYER_ACTIVE,
	FADING_OUT,
	LAYER_FADED_OUT,
	LAYER_OFF,
};

class Layer
{
	public:
		Layer( std::string filename, LayerMode startMode );
		void preformFadeOperation( double lastCallTime );

		static int increaseFPS (int i )
		{
			FPS += i;
			return FPS;
		}

		static int decreaseFPS (int i )
		{
			FPS -= i;
			return FPS;
		}

		virtual void turnOn()
		{
			std::ostringstream os;
			os << "Layer: " << m_filename << " - ON. ";
			m_debug.print( os.str() );
		}
		
		virtual void turnOff()
		{
			std::ostringstream os;
			os << "Layer: " << m_filename << " - OFF. ";
			m_debug.print( os.str() );
		}

		virtual void setMode( LayerMode newMode )
		{
			m_currentMode = newMode;

			std::ostringstream os;
			os << "Mode Change: " << m_filename << " - " << (int) newMode;
			m_debug.print( os.str() );
		}


		virtual void updateData( double lastCallTime ) = 0;

		void setFade( float fade, double time );
		void setCurrentFade( eTransitionMode mode, float value );

		LayerMode getMode()
		{ return m_currentMode; }

		float getFade()
		{ return fade; }

protected:
	    DebugStream m_debug;

        std::string m_filename;
    
		GLuint texture;
		GLfloat polyW, polyH;

		static int FPS;
		static int FPSPrint;

		double lastUpdate;
		bool fadeIn;

		float fade;
		float currentFade;

		LayerMode m_currentMode;
};

#endif
