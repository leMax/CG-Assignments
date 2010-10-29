#ifndef LOGMANAGER_HPP
#define LOGMANAGER_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

/** This class provides a simple mechanism to log messages by writing them on the terminal and into a file.
* @brief Simply log messages on the terminal and into a Logfile.
* @code
* LogManager* l = LogManager::getSingletonPtr();
* // log a simple string
* l->logMessage("This is an important information");
* // log concartenated strings 
* std::stringstream log;
* log << "This is the first message";
* log << " and this is the second";
* l->logMessage( log );
* @endcode
* @author Andy Reimann andy.reimann@uni-weimar.de
*/
class LogManager {
	public:
		/** Get a pointer to one single instance.
		* @param logFileName the name of the logfile to use.
		* @note this function only works correctly when it is called before an instance of the LogManager exists.
		* @return A pointer of one single instance.
		*/
		static LogManager* getSingletonPtr( std::string logFileName );
		/** Get a pointer to one single instance.
		* @return A pointer of one single instance.
		*/
		static LogManager* getSingletonPtr(  );
		/** Get a reference to one single instance.
		* @return A reference of one single instance.
		*/
		static LogManager& getSingleton(  );
		/** Destroys the one single instance.
		*/
		static void destroy();
		/** this function writes a message into the logfile.
		* @param m the message to write.
		*/
		void logMessage( std::string const& m );
		/** this function writes a message into the logfile.
		* @param m the message to write.
		*/
		void logMessage( std::stringstream const& m );
	private:
		static LogManager* mInstance_;; //!< the one single instance
		/** Constructs a LogManager object
		* @param logFileName the name of the logfile to use.
		*/
		LogManager( std::string logFileName );
		~LogManager(); //! destructor
		std::fstream mFile_; //!< the filestream we're going to write in
		long int	mCounter_;  //!< counter for the log entries
};

/** This function shifts the content of a string into the LogManager.
* @param lm A reference to the loaMamager.
* @param s the String to shift to the LogManager.
*/
LogManager& operator<<(LogManager& lm, std::string s);
/** This function shifts the content of a stringstream into the LogManager.
* @param lm A reference to the loaMamager.
* @param s the Stringstream to shift to the LogManager.
*/
LogManager& operator<<(LogManager& lm, std::stringstream& s);



#endif
