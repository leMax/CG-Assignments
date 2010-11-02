#include <RenderEngine.h>
#include <SDL/SDL.h>

#ifdef _WIN32
	// in SDLmain.lib is an SDL_main entypoint
	// we have to disable it
	// @see http://www.spieleprogrammierer.de/phpBB2/viewtopic.php?t=8526
	#undef main
#endif 
//test
int main( int argc, char** argv ) {
    // SDL_OPENGL;          Enable OpenGL in SDL */
    // SDL_GL_DOUBLEBUFFER; Enable double buffering */
    // SDL_RESIZABLE;       Enable window resizing */
	unsigned flags = 0;

	// create the flags according to the operating system (I havn't found some flag setting which worked for both win32 and unix systems)
	flags |= SDL_OPENGL;
	flags |= SDL_RESIZABLE;
#ifdef WIN32
	flags |= SDL_DOUBLEBUF;
#else 
	flags |= SDL_HWPALETTE;
	flags |= SDL_GL_DOUBLEBUFFER;
#endif

	RenderEngine e(1024, 768, 1, flags, 
		"Beleg 1: Universe in a nut-shell");
	e.startRenderLoop();

	return 0;
}
