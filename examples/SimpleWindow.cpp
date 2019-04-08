#include <venum/window.hpp>
#include <venum/fpslimiter.hpp>
#include <venum/event.hpp>
#include <venum/renderer.hpp>

/* Main function */
int main( int argc, char** argv )
{
	/* Creating Window named "window" with a title "Title of a Window" */
	ven::Window window( "Title of a Window" );
	/* Creating FPSLimiter named "fps" with limit of 30 fps */
    	ven::FPSLimiter fps;
		
	/* Creating Event named "event" */
    	ven::Event event;
	/* Attaching "window" as a window pointer to "event"s Mouse */
    	event.Mouse.AttachWindowPointer( &window );
		
	/* Creating Renderer named "renderer" */
	ven::Renderer renderer;
		
	/* Loop running while the window is running */
	while( window.Running(  ) )
	{
		/* Starting fps measurements in this frame */
		fps.UpdateStart(  );
		/* Polling an Event from "window". Returns true, if event happened. */
        	window.PollEvent( &event.Event );
				
		/* Cleaning renderers buffer */
		renderer.Clean(  );
				
		/* Swapping windows buffer */
		window.Swapb(  );
		/* Updating fps */
        	fps.UpdateEnd(  );
	}
		
	/* Returning 0 when program closes */
	return 0;
}
