#include "InputManager.h"
// SINGLETON
InputManager* InputManager::mInstance_ = NULL;

InputManager&
InputManager::getSingleton( ) {
  if( !mInstance_ )
    mInstance_ = new InputManager();
  return *mInstance_;
}

InputManager*
InputManager::getSingletonPtr( ) {
  if( !mInstance_ )
    mInstance_ = new InputManager();
  return mInstance_;
}

InputManager::InputManager() {
	// init the camera movement
	mCameraMovement_.moveBackward = false;
	mCameraMovement_.moveForward = false;
	mCameraMovement_.moveLeft = false;
	mCameraMovement_.moveRight = false;
	mCameraMovement_.rotX = mCameraMovement_.rotY = mCameraMovement_.rotZ = 0.0f;
	mCameraMovement_.transX = mCameraMovement_.transY = mCameraMovement_.transZ = 0.0f;
	mCameraMovement_.firstMouseMotionCaptured = false;
	mLMBDown_ = false;
}

bool
InputManager::keyDown( SDL_Event const& e ) {
	//LogManager::getSingletonPtr()->logMessage("InputManager: Key-Down Event occured");
	if( e.key.keysym.sym == SDLK_UP || e.key.keysym.sym == SDLK_w )
		mCameraMovement_.moveForward = true;
	if( e.key.keysym.sym == SDLK_DOWN || e.key.keysym.sym == SDLK_s )
		mCameraMovement_.moveBackward = true;
	if( e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_a )
		mCameraMovement_.moveLeft = true;
	if( e.key.keysym.sym == SDLK_RIGHT || e.key.keysym.sym == SDLK_d )
		mCameraMovement_.moveRight = true;

	if( e.key.keysym.sym == SDLK_ESCAPE )
		return false;
	return true;
}

bool
InputManager::keyUp( SDL_Event const& e ) {
	//LogManager::getSingletonPtr()->logMessage("InputManager: Key-Up Event occured");
	if( e.key.keysym.sym == SDLK_UP || e.key.keysym.sym == SDLK_w )
		mCameraMovement_.moveForward = false;
	if( e.key.keysym.sym == SDLK_DOWN || e.key.keysym.sym == SDLK_s )
		mCameraMovement_.moveBackward = false;
	if( e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_a )
		mCameraMovement_.moveLeft = false;
	if( e.key.keysym.sym == SDLK_RIGHT || e.key.keysym.sym == SDLK_d )
		mCameraMovement_.moveRight = false;
	return true;
}

bool
InputManager::mouseDown( SDL_Event const& e ) {
	//LogManager::getSingletonPtr()->logMessage("InputManager: Mouse-Down Event occured");
	if( e.button.button == SDL_BUTTON_LEFT )
		mLMBDown_ = true;
	return true;
}

bool
InputManager::mouseUp( SDL_Event const& e ) {
	//LogManager::getSingletonPtr()->logMessage("InputManager: Mouse-Up Event occured");
	if( e.button.button == SDL_BUTTON_LEFT )
		mLMBDown_ = false;
	return true;
}

bool
InputManager::mouseMoved( SDL_Event const& e ) {
	//LogManager::getSingletonPtr()->logMessage("InputManager: Mouse-Moved Event occured");
	if( mLMBDown_ ) {
		float maxXAngle = 70.0f;
		mCameraMovement_.rotY += e.motion.xrel/1.0f; // yaw
		mCameraMovement_.rotX += e.motion.yrel/1.0f; // pitch
		mCameraMovement_.rotX = std::max( -maxXAngle, std::min( mCameraMovement_.rotX, maxXAngle ));
	}
	if( !mCameraMovement_.firstMouseMotionCaptured ) {
		// capture the init rotation to negate it
		mCameraMovement_.rotXShift = mCameraMovement_.rotX;
		mCameraMovement_.rotYShift = mCameraMovement_.rotY;
		mCameraMovement_.firstMouseMotionCaptured = true;
	}
	return true;
}

void
InputManager::doGLCameraMovement() const {
	// perform the Camera rotation
	glRotatef(mCameraMovement_.rotX, 1.0f, 0.0f, 0.0f);
	glRotatef(mCameraMovement_.rotY, 0.0f, 1.0f, 0.0f);
	glRotatef(mCameraMovement_.rotZ, 0.0f, 0.0f, 1.0f);
	// perform the Camera translation
	glTranslatef( mCameraMovement_.transX, mCameraMovement_.transY, mCameraMovement_.transZ );
}

void
InputManager::updateCameraMovements( double timeSinceLastFrame ) {
	float acceleration = 20.0f * (float)timeSinceLastFrame;
	float speed = 0.8f;
	float xRadius = (mCameraMovement_.rotX / 180.0f * (float)M_PI);
	float yRadius = (mCameraMovement_.rotY / 180.0f * (float)M_PI);
	float sinXRad = (float)std::sin(xRadius);
	float sinYRad = (float)std::sin(yRadius);
	float cosYRad = (float)std::cos(yRadius);
	if( mCameraMovement_.moveForward ) {
		mCameraMovement_.transX += -sinYRad * speed * acceleration;
		mCameraMovement_.transY += sinXRad * speed * acceleration;
		mCameraMovement_.transZ += cosYRad * speed * acceleration;
	}
	if( mCameraMovement_.moveBackward ) {
		mCameraMovement_.transX += sinYRad * speed * acceleration;
		mCameraMovement_.transY += -sinXRad * speed * acceleration;
		mCameraMovement_.transZ += -cosYRad * speed * acceleration;
	}
	if( mCameraMovement_.moveRight ) {
		mCameraMovement_.transX += -cosYRad * speed * acceleration;
		mCameraMovement_.transZ += -sinYRad * speed * acceleration;
	}
	if( mCameraMovement_.moveLeft ) {
		mCameraMovement_.transX += cosYRad * speed * acceleration;
		mCameraMovement_.transZ += sinYRad * speed * acceleration;
	}


}

InputManager::~InputManager() {
}
