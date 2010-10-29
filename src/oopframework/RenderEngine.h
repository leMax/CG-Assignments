#ifndef RENDERENGINE_HPP
#define RENDERENGINE_HPP

#include <LogManager.h>
#include <TextureManager.h>
#include <RessourceManager.h>
#include <InputManager.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <SDL/SDL.h>
#ifdef _WIN32
	#include <windows.h>
#endif

/** This struct defines the Window Settings
 * @brief Definition of Window Settings
 * @author Andy Reimann andy.reimann@uni-weimar.de
 */
struct windowSettings {
	unsigned	x;			//!< The window width
	unsigned	y;			//!< the window height
	unsigned	aaSampels;	//!< the number of Anti-Aliasing Samples to use
	unsigned	flags;		//!< The SDL-Flags to use for the Renderwindow
	std::string title;		//!< The Title of the Render Window
#ifdef _WIN32
	HWND		handle;		//!< The handle of the Window - only if we're on a windows machine
#endif
};

/** This class is the main renderengine of the Framework. The tasks are open the Renderloop, init the LogManager and the inputManager.
 * @brief The main Render Engine which creates and handles all the other classes and starts the Renderloop.
 * @author Andy Reimann
 */
class RenderEngine {
	public:
		/** The constructor.
		 * @param winX The Resolution in X-Dimension.
		 * @param winY The Resolution in Y-Dimension.
		 * @param aaSamples The number of Anti-Aliasing Samples to use when rendering.
		 * @param title The title of the Renderwindow.
		 * @note The Window Title is only set, if the machine supports this feature.
		 */
		RenderEngine( unsigned winX, unsigned winY, unsigned aaSamples, unsigned sdlFlags, std::string const& title );
		/** This function will start the RenderLoop if the RenderEngine was initialized and is valid.
		 */
		void startRenderLoop();


		~RenderEngine();
	private:

		/** This function will open the Renderwindow after parsing the needed variables like the window metrics, ...
		 */
		bool initWindow();
		/** This function inits all the managers used in the OOP-Framework
		 */
		void initManagers();
		/** This function inits all the OpenGL-States which have to be only done one time before rendering
		 */
		void initProperties();
		/** This function initializes the kind of Projection to use
		 */
		void initProjection();
		/** This function does everything which has to be done/loaded before rendering to setup the Scene.
		 */
		void initScene();
		
		/** This function will init an OpenGL Light.
		 * it have to be calles only once at the begining.
		 */
		void initLight();
		/** This function will set the OpenGL Light.
		 * it have to be calles every frame.
		 */
		void setLight();

		/** This routine contains the OpenGL-Calls which are needed to render the Scene
		 * @param timeSinceLastFrame The elapsed time since the rendering of the last Frame in seconds.
		 */
		bool display( double timeSinceLastFrame );

		bool mValid_; //!< If this is true, it indicates that the renderengine was initialized successfully

		windowSettings mWindow_; //!< The settings of the Window

		Texture* mEarthTexture_; //!< the Texture of the Earth in the Example Program, which is rendered.
		Texture* mEarthCloudTexture_; //!< the Cloud Texture of the Earth in the Example Program, which is rendered.
		Texture* mStarMap_; //!< The stars Texture
		float mSphereRot_;


};

#endif
