#include "RenderEngine.h"
#include <time.h>

#ifndef WIN32
#include <sys/time.h>
static double unixLastTime = -1.0;
#else 
#include <windows.h>
#include <mmsystem.h>
static double win32LastTime = -1.0;
#endif


RenderEngine::RenderEngine( unsigned winX, unsigned winY, unsigned aaSamples, unsigned sdlFlags, std::string const& title ) :
	mValid_(false),
	mEarthTexture_(NULL),
	mEarthCloudTexture_(NULL),
	mStarMap_(NULL) {
	// Initialize the LogManager with a logfilename (only on startup)
  LogManager::getSingletonPtr("runtime.log");
	
	// avoid a division by zero on zero Y-Dimension
	if( winY == 0 )
		winY = 1;
	mWindow_.x = winX;
	mWindow_.y = winY;
	// Get the number of Anti-Aliasing Samples to use
	if( aaSamples == 8 || aaSamples == 4 || aaSamples == 2 || aaSamples == 1 )
		mWindow_.aaSampels = aaSamples;
	else 
		mWindow_.aaSampels = 1; 
	mWindow_.flags = sdlFlags;
	mWindow_.title = title;
	std::stringstream log;
	
	if( initWindow() ) // everything is ok
		mValid_ = true;

	initManagers();
	initProperties();
	initProjection();
	initScene();
}

bool
RenderEngine::initWindow() {
	LogManager* l = LogManager::getSingletonPtr();
	std::stringstream log;
	log << "SDL: Init Window with SDL...";
	l->logMessage( log );
	log.str("");
	log << "SDL: WindowX=" << mWindow_.x << " WindowY=" << mWindow_.y << " AASamples=" << mWindow_.aaSampels << " Title=" << mWindow_.title;
	l->logMessage( log );
	log.str("");
	// Initialize SDL for video output 
	if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
		log << "SDL: Unable to initialize SDL with the following error: " << SDL_GetError();
		l->logMessage(log);
		return false;
	}
	else {
		log << "SDL: done. Init SDL-VideoMode...";
		l->logMessage( log );
		log.str("");

		// Fetch the video info
		SDL_VideoInfo const* videoInfo = SDL_GetVideoInfo( );

		 /* This checks if hardware blits can be done */
		if ( videoInfo->blit_hw )
			mWindow_.flags |= SDL_HWACCEL;

		if ( !videoInfo ) {
			log << "SDL: Video query failed with the following error: " << SDL_GetError();
			l->logMessage( log );
			return false;
		}

		// enable double buffering
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		// enable AA
#ifdef WIN32
// this line causes the x11 driver to fail in most cases !?!
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, mWindow_.aaSampels);
#endif
		// per default we hide the mouse Cursor


		// Create a OpenGL screen
#ifndef WIN32
		if( SDL_SetVideoMode(mWindow_.x, mWindow_.y, 16, mWindow_.flags) == NULL ) {
#else
		if( SDL_SetVideoMode(mWindow_.x, mWindow_.y, 32, mWindow_.flags) == NULL ) {
#endif
		  log << "SDL: Unable to create OpenGL screen with the following reason: " << SDL_GetError();
		  l->logMessage(log);
		  SDL_Quit();
		  return false;
		}
		else {
		  // create the RenderWindow containing all informations about the creation 
		  log << "done";
		  l->logMessage(log);
		  // Set the title bar in environments that support it
		  SDL_WM_SetCaption(mWindow_.title.c_str(), NULL);
#ifdef WIN32
		  // get the windowhandle 
		  mWindow_.handle = FindWindow(NULL, mWindow_.title.c_str());
		  MoveWindow( mWindow_.handle, 0,0, mWindow_.x, mWindow_.y, true );
#endif
		  return true;
		}
	}
}

void
RenderEngine::initManagers() {
	// add here any of your additional Ressourcelocations like shader directories and so on
	RessourceManager::getSingletonPtr()->addRessourceLocation("../media/");
	RessourceManager::getSingletonPtr()->addRessourceLocation("../media/images/");
	
	InputManager::getSingleton();
	ilInit();
	TextureManager::getSingleton();
}

void
RenderEngine::initProperties() {
	// set the default depth and color values for the clear Buffer calls
	glClearDepth(1.0f);
	glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );

	glEnable( GL_DEPTH_TEST );
	glDepthFunc(GL_LESS);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glEnable(GL_RESCALE_NORMAL);
	glEnable( GL_NORMALIZE );
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable( GL_BLEND );
	glEnable( GL_POINT_SMOOTH );
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST );
	glShadeModel( GL_SMOOTH );
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST );      
	glHint(GL_FOG_HINT, GL_NICEST);
	glEnable( GL_POLYGON_SMOOTH );
	initLight();
}

void
RenderEngine::initProjection() {
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glViewport(0,0,mWindow_.x, mWindow_.y);
	float ratio = (float)mWindow_.x/(float)mWindow_.y;
	gluPerspective(60.0f,ratio,1.0f,4000.0f);
	SDL_SetVideoMode(mWindow_.x, mWindow_.y, 32, mWindow_.flags);
}

void
RenderEngine::initScene() {
	// load the Texture for the Sphere
	// note that you do not have to know where the texture is.
	// you only have to be sure that the Directory is available in the RessourceManager
	// The RessourceManager will give a Warning if the Texture will not be found.

	mEarthTexture_ = TextureManager::getSingletonPtr()->loadTexture("earthmap4k.jpg", GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	if( mEarthTexture_ == NULL ) // load a lower version if size of texture isn't supported
		mEarthTexture_ = TextureManager::getSingletonPtr()->loadTexture("earthmap1k.jpg", GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR);

	mEarthCloudTexture_ = TextureManager::getSingletonPtr()->loadTexture("earth_clouds_4k.jpg", GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	if( mEarthCloudTexture_ == NULL ) // load a lower version if size of texture isn't supported
		mEarthCloudTexture_ = TextureManager::getSingletonPtr()->loadTexture("earth_clouds_1k.jpg", GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR);

	mStarMap_ = TextureManager::getSingletonPtr()->loadTexture("starmap_4k.jpg", GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	if( mStarMap_ == NULL ) // load a lower version if size of texture isn't supported
		mStarMap_ = TextureManager::getSingletonPtr()->loadTexture("starmap_1k.jpg", GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	
	mSphereRot_ = 0.0f;
}	

void 
RenderEngine::initLight() {
	// create one light
	glEnable( GL_LIGHTING );
	glEnable( GL_LIGHT0 );
	setLight();
}	

void 
RenderEngine::setLight() {
	// set position
	float pos[4] =		{ 20.0f , 20.0f , 0.0f, 1.0f};
	float ambient[4] =  {  0.18f,  0.18f, 0.1f, 1.0f};
	float diffuse[4] =  {  1.0f ,  1.0f , 0.7f, 1.0f};
	float specular[4] = {  1.0f ,  1.0f , 0.7f, 1.0f};
	glLightfv(GL_LIGHT0, GL_POSITION, pos );
	// set the color of the Light
	glLightfv(GL_LIGHT0, GL_AMBIENT,  ambient );
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffuse );
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular );
}

bool
RenderEngine::display( double timeSinceLastFrame ) {
	// render something 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	// update the cameras movement once per frame
	InputManager::getSingletonPtr()->updateCameraMovements( timeSinceLastFrame );
	// do the cameras movement in OpenGL calls
	InputManager::getSingletonPtr()->doGLCameraMovement();
	

	glPushMatrix();
	setLight();

	// set material settings
	float ambient[4] =  { 0.55f, 0.55f, 0.55f, 0.7f};
	float diffuse[4] =  { 1.0f , 1.0f , 1.0f , 0.6f};
	float specular[4] = { 1.0f , 1.0f , 0.9f , 0.3f};
	float emmisive[4] = { 0.0f , 0.0f , 0.0f , 0.3f};
	float shininess = 100;
	glMaterialfv( GL_FRONT, GL_AMBIENT, ambient );
	glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse );
	glMaterialfv( GL_FRONT, GL_SPECULAR, specular );
	glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, emmisive );
	glMaterialf( GL_FRONT, GL_SHININESS, shininess );

	
	//// draw the starmap
	glDisable( GL_LIGHTING );

	glColor3f(1.0f,1.0f,1.0f);

	float size = 2000.0f;
	float txMax = 1.0f;
	if( mStarMap_ != NULL )
		mStarMap_->bind();
	glPushMatrix();
	glRotatef(mSphereRot_ / 5.0f, 0.2f, 0.7f, 0.4f);
	glBegin(GL_QUADS);	
		// top plane
		glTexCoord2f(0.0f,txMax);
		glVertex3f(-size,size, -size);				// Top Left
		glTexCoord2f(txMax, txMax);
		glVertex3f( size,size, -size);				// Top Right
		glTexCoord2f(txMax,0.0f);
		glVertex3f( size,size, size);				// Bottom Right
		glTexCoord2f(0.0f,0.0f);
		glVertex3f(-size,size, size);				// Bottom Left
		// botom plane
		glTexCoord2f(0.0f,txMax);
		glVertex3f(-size,-size, -size);				// Top Left
		glTexCoord2f(txMax, txMax);
		glVertex3f( size,-size, -size);				// Top Right
		glTexCoord2f(txMax,0.0f);
		glVertex3f( size,-size, size);				// Bottom Right
		glTexCoord2f(0.0f,0.0f);
		glVertex3f(-size,-size, size);				// Bottom Left
		// back plane
		glTexCoord2f(0.0f,txMax);
		glVertex3f(-size,-size, -size);				// Top Left
		glTexCoord2f(txMax, txMax);
		glVertex3f( size,-size, -size);				// Top Right
		glTexCoord2f(txMax,0.0f);
		glVertex3f( size,size, -size);				// Bottom Right
		glTexCoord2f(0.0f,0.0f);
		glVertex3f(-size,size, -size);				// Bottom Left
		// front plane
		glTexCoord2f(0.0f,txMax);
		glVertex3f(-size,-size, size);				// Top Left
		glTexCoord2f(txMax, txMax);
		glVertex3f( size,-size, size);				// Top Right
		glTexCoord2f(txMax,0.0f);
		glVertex3f( size,size, size);				// Bottom Right
		glTexCoord2f(0.0f,0.0f);
		glVertex3f(-size,size, size);				// Bottom Left
		// left plane
		glTexCoord2f(0.0f,txMax);
		glVertex3f(-size,-size, -size);				// Top Left
		glTexCoord2f(txMax, txMax);
		glVertex3f( -size,-size, size);				// Top Right
		glTexCoord2f(txMax,0.0f);
		glVertex3f( -size,size, size);				// Bottom Right
		glTexCoord2f(0.0f,0.0f);
		glVertex3f(-size,size, -size);				// Bottom Left
		// right plane
		glTexCoord2f(0.0f,txMax);
		glVertex3f(size,-size, -size);				// Top Left
		glTexCoord2f(txMax, txMax);
		glVertex3f( size,-size, size);				// Top Right
		glTexCoord2f(txMax,0.0f);
		glVertex3f( size,size, size);				// Bottom Right
		glTexCoord2f(0.0f,0.0f);
		glVertex3f(size,size, -size);				// Bottom Left
	glEnd();							
	if( mStarMap_ != NULL )
		mStarMap_->unbind();
	glPopMatrix();
	
	glEnable( GL_LIGHTING );

	glMatrixMode( GL_MODELVIEW );

	glTranslatef( 0.0f,0.0f,-20.0f );
	glRotatef(mSphereRot_,0.0f,1.0f,0.0f);
	glRotatef(23.44f,0.0f,0.0f,1.0f);
	glRotatef(90.0f,1.0f,0.0f,0.0f);

	// set material settings
	ambient[3] = 1.0f;
	diffuse[3] = 1.0f;
	specular[3] = 1.0f;
	emmisive[3] = 1.0f;
	glMaterialfv( GL_FRONT, GL_AMBIENT, ambient );
	glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse );
	glMaterialfv( GL_FRONT, GL_SPECULAR, specular );
	glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, emmisive );

		// Draw a Textured Sphere
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		// Draw the Earth
		if( mEarthTexture_ != NULL )
			mEarthTexture_->bind();
		glColor4f(1.0f,1.0f,1.0f, 1.0f);
		GLUquadric* qobj = gluNewQuadric();
			gluQuadricTexture(qobj,GL_TRUE);
			gluQuadricOrientation(qobj, GLU_INSIDE);
			gluQuadricDrawStyle(qobj, GLU_FILL);
			gluQuadricNormals( qobj, GLU_SMOOTH);
			gluQuadricOrientation( qobj, GLU_OUTSIDE);
			gluSphere(qobj,5.0f,100,100);
		gluDeleteQuadric(qobj);
		if( mEarthTexture_ != NULL )
			mEarthTexture_->unbind();

		// draw the grid of the Earth
		// set material settings
		ambient[3] = 0.3f;
		diffuse[3] = 0.3f;
		specular[3] = 0.3f;
		emmisive[3] = 0.3f;
		glMaterialfv( GL_FRONT, GL_AMBIENT, ambient );
		glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse );
		glMaterialfv( GL_FRONT, GL_SPECULAR, specular );
		glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, emmisive );
		glColor4f(1.0f,1.0f,1.0f, 0.3f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		qobj = gluNewQuadric();
			gluQuadricTexture(qobj,GL_FALSE);
			gluQuadricOrientation(qobj, GLU_INSIDE);
			gluQuadricDrawStyle(qobj, GLU_FILL);
			gluQuadricNormals( qobj, GLU_SMOOTH);
			gluQuadricOrientation( qobj, GLU_OUTSIDE);
			gluSphere(qobj,5.1f,50,50);
		gluDeleteQuadric(qobj);

		// draw the Axis of the Earth
		glBegin( GL_LINES );
			glColor3f(1.0f,1.0f,0.0f);
			glVertex3f(0.0f, 0.0, -10.0f);
			glVertex3f(0.0f, 0.0, 10.0f);
		glEnd();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		// draw the clouds of the Earth
		// set material settings
		ambient[3] = 0.3f;
		diffuse[3] = 0.3f;
		specular[3] = 0.3f;
		emmisive[0] = 0.7f;
		emmisive[1] = 0.7f;
		emmisive[2] = 0.7f;
		emmisive[3] = 0.3f;
		glMaterialfv( GL_FRONT, GL_AMBIENT, ambient );
		glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse );
		glMaterialfv( GL_FRONT, GL_SPECULAR, specular );
		glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, emmisive );
		glPushMatrix();
		glRotatef(mSphereRot_/2.0f, 0.3f, 0.6f, 0.4f );
		if( mEarthCloudTexture_ != NULL )
			mEarthCloudTexture_->bind();
		glColor4f(1.0f,1.0f,1.0f, 0.3f);
		qobj = gluNewQuadric();
			gluQuadricTexture(qobj,GL_TRUE);
			gluQuadricOrientation(qobj, GLU_INSIDE);
			gluQuadricDrawStyle(qobj, GLU_FILL);
			gluQuadricNormals( qobj, GLU_SMOOTH);
			gluQuadricOrientation( qobj, GLU_OUTSIDE);
			gluSphere(qobj,5.4f,120,120);
		gluDeleteQuadric(qobj);
		if( mEarthCloudTexture_ != NULL )
			mEarthCloudTexture_->unbind();
		glPopMatrix();

	glPopMatrix();
	
	mSphereRot_ += 3.0f * (float)timeSinceLastFrame;
	return true;
}

void
RenderEngine::startRenderLoop() {
	unsigned frame = 0;
	if( mValid_ ) {
		// a Framecounter
		// renderengine was successfully initialized
		bool done = false;
			while( !done ) {
				double timeSinceLastFrame = 0.0;

#ifndef WIN32
				timeval tvBegin;
				gettimeofday(&tvBegin, NULL);


				double tv = tvBegin.tv_sec + (tvBegin.tv_usec / 1000000.0);
				if( unixLastTime >= 0.0f )
					timeSinceLastFrame = tv - unixLastTime;
				else
					timeSinceLastFrame = 0.0f;
				unixLastTime = tv;
#else
				double currentTime = timeGetTime() / 1000.0;	
				if( win32LastTime >= 0.0 )
					timeSinceLastFrame = currentTime - win32LastTime;
				else
					timeSinceLastFrame = 0.0;
				win32LastTime = currentTime;
#endif
				done = !display( timeSinceLastFrame );
				SDL_Event sdlEvent;
				while ( SDL_PollEvent(&sdlEvent) ) {
					// call the InputManagers callback methods
					if ( sdlEvent.type == SDL_KEYDOWN ) {
						done = !InputManager::getSingletonPtr()->keyDown( sdlEvent );
						if( done )
							break;
					}
					if ( sdlEvent.type == SDL_KEYUP ) {
						done = !InputManager::getSingletonPtr()->keyUp( sdlEvent );
						if( done )
							break;
					}
					if ( sdlEvent.type == SDL_MOUSEBUTTONDOWN ) {
						done = !InputManager::getSingletonPtr()->mouseDown( sdlEvent );
						if( done )
							break;
					}
					if ( sdlEvent.type == SDL_MOUSEBUTTONUP ) {
						done = !InputManager::getSingletonPtr()->mouseUp( sdlEvent );
						if( done )
							break;
					}
					if ( sdlEvent.type == SDL_MOUSEMOTION ) {
						done = !InputManager::getSingletonPtr()->mouseMoved( sdlEvent );
						if( done )
							break;
					}
				}
				++frame;
				SDL_GL_SwapBuffers();
			}
	}
	else
		LogManager::getSingletonPtr()->logMessage("RenderError: SDL wasnt setup successfully. Cannot start RenderLoop.");
	std::stringstream log;
	log << "Renderengine: " << frame << " Frames rendered.";
	LogManager::getSingletonPtr()->logMessage( log );
	SDL_Quit();
}

RenderEngine::~RenderEngine() {
	// delete all Singleton managers and all used textures
	RessourceManager::destroy();
	LogManager::destroy();
	// call the Texturemanager to delete all used textures
	TextureManager::getSingleton().deleteTexture( mEarthTexture_ );
	TextureManager::getSingleton().deleteTexture( mEarthCloudTexture_ );
	TextureManager::getSingleton().deleteTexture( mStarMap_ );
	TextureManager::destroy();
}
