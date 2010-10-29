#include "LogManager.h"

// SINGLETON
LogManager* LogManager::mInstance_ = NULL;


LogManager*
LogManager::getSingletonPtr( std::string logFileName ) {
  if( !mInstance_ )
    mInstance_ = new LogManager(logFileName);
  return mInstance_;
}

LogManager*
LogManager::getSingletonPtr( ) {
  if( !mInstance_ )
    mInstance_ = new LogManager("runtime.log");
  return mInstance_;
}

LogManager&
LogManager::getSingleton( ) {
  if( !mInstance_ )
    mInstance_ = new LogManager("runtime.log");
  return *mInstance_;
}

LogManager::LogManager( std::string logFileName ) :
  mCounter_(0) {
    mFile_.open(logFileName.c_str(), std::ios::out);
}

void
LogManager::logMessage( std::string const& m ) {
  ++mCounter_;
  mFile_ << " [" << mCounter_ << "] : " << m << std::endl;
  std::cout << " [" << mCounter_ << "] : " << m << std::endl;
}

void
LogManager::logMessage( std::stringstream const& m ) {
  logMessage( m.str() );
}

LogManager::~LogManager() {
    mFile_.close();
}

void
LogManager::destroy() {
  if( mInstance_ )
    delete mInstance_;
  mInstance_ = NULL;
}

LogManager& operator<<(LogManager& lm, std::string s) {
  lm.logMessage(s);
  return lm;
}

LogManager& operator<<(LogManager& lm, std::stringstream& s) {
  lm.logMessage(s);
  s.str("");
  return lm;
}
