#include "RessourceManager.h"

// SINGLETON
RessourceManager* RessourceManager::mInstance_ = NULL;


RessourceManager&
RessourceManager::getSingleton( ) {
  if( !mInstance_ )
    mInstance_ = new RessourceManager();
  return *mInstance_;
}

RessourceManager*
RessourceManager::getSingletonPtr( ) {
  if( !mInstance_ )
    mInstance_ = new RessourceManager();
  return mInstance_;
}

void 
RessourceManager::addRessourceLocation( std::string const& loc ) {
	std::stringstream log;
	log << "Ressourcemanager: Adding Ressource Location '" << loc << "' to Ressource Manager.";
	LogManager::getSingletonPtr()->logMessage( log );
	std::string locNew = loc;
	if( locNew.substr( locNew.length()-1, 1 ) != "/" )
		locNew.append("/");
	mLocations_.push_back( locNew );
}

std::string 
RessourceManager::getPath( std::string const& filename ) {
	std::vector<std::string>::iterator it = mLocations_.begin();
	while( it != mLocations_.end() ) {
		if( fileExists( filename, *it ) ) {
			return *it;
		}
		++it;
	}
	std::stringstream log;
	log << "Ressourcemanager Warning: Could not get path for the File " << filename << ". File was not found in available ressource locations.";
	LogManager::getSingletonPtr()->logMessage( log );
	return std::string("");
}

bool 
RessourceManager::fileExists( std::string const& file, std::string const& path ) {
	// todo: check if path ends with /
	std::string fullName = path + file;
	std::ifstream inp;
	inp.open(fullName.c_str(), std::ifstream::in);
	inp.close();
	if(!inp.fail()) {
		std::stringstream log;
		log << "Ressourcemanager: File " << file << " found in ressource location " << path;
		LogManager::getSingletonPtr()->logMessage( log );
		return true;
	}
	return false;
}


RessourceManager::RessourceManager() {
}

RessourceManager::~RessourceManager() {
	mLocations_.clear();
}

void
RessourceManager::destroy() {
  if( mInstance_ )
    delete mInstance_;
  mInstance_ = NULL;
}
