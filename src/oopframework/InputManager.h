#ifndef INPUTMANAGER
#define INPUTMANAGER

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <cstdlib>
#include <cmath>
#include <LogManager.h>

#include <SDL/SDL.h>

#define _USE_MATH_DEFINES

#ifndef M_PI
	#define M_PI 3.14159265358979323846	
#endif

struct movement {
	float rotX, rotY, rotZ;			//!< The Rotations of the Camera
	float transX, transY, transZ;	//!< The Translations of the Camera
	bool moveForward;				//!< indicates if we should move forward
	bool moveBackward;				//!< indicates if we should move backward
	bool moveLeft;					//!< indicates if we should move left
	bool moveRight;					//!< indicates if we should move right

	/** This is only a little helper for the following problem:
	 * The initial mouseposition will cause to start the application with a fuzzy like rotation.
	 * we capture the init rotation and negate it so that the application always starts with a zero rotation.
	 */
	bool firstMouseMotionCaptured;	//!< this is only a little helper
	float rotXShift;
	float rotYShift;
};

/** This class is the Manager for any Keyboard- or Mouseinput.
 * @brief Handles Keyboard- and MouseInput
 * @author Andy Reimann andy.reimann@uni-weimar.de
 */
class InputManager {
	public:	
		/** Get a reference to one single instance.
		 * @return A reference of one single instance.
		 */
		static InputManager& getSingleton( );
		/** Get a pointer to one single instance.
		 * @return A pointer of one single instance.
		 */
		static InputManager* getSingletonPtr( );

		/** This function is automatically called on such an event in SDL.
		 * It handles Key-Down-Events
		 * @return true if the Engine should go on rendering, false if not.
		 */
		bool keyDown( SDL_Event const& e );
		/** This function is automatically called on such an event in SDL.
		 * It handles Key-Up-Events
		 * @return true if the Engine should go on rendering, false if not.
		 */
		bool keyUp( SDL_Event const& e );
		/** This function is automatically called on such an event in SDL.
		 * It handles Mouse-Down-Events
		 * @return true if the Engine should go on rendering, false if not.
		 */
		bool mouseDown( SDL_Event const& e );
		/** This function is automatically called on such an event in SDL.
		 * It handles Mouse-Up-Events
		 * @return true if the Engine should go on rendering, false if not.
		 */
		bool mouseUp( SDL_Event const& e );
		/** This function is automatically called on such an event in SDL.
		 * It handles Mouse-Moved-Events
		 * @return true if the Engine should go on rendering, false if not.
		 */
		bool mouseMoved( SDL_Event const& e );

		/** This function performs the Camera movement in OpenGL calls
		 */
		void doGLCameraMovement() const;
		/** This function updates the Camera movement based on the current movement states.
		 * @param timeSinceLastFrame The elapsed time since the rendering of the last Frame in seconds.
		 */
		void updateCameraMovements( double timeSinceLastFrame );


	private:
		static InputManager* mInstance_; //!< the one single instance
		InputManager(); //!< normal constructor
		~InputManager(); //!< normal destructor

		movement mCameraMovement_; //!< the Camera movement
		bool	 mLMBDown_;			//!< Indicates whether the left mouse button is down or not
};

#endif
