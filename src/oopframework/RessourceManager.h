#ifndef RESSOURCEMANAGER
#define RESSOURCEMANAGER
/*******************************************************
 * Ressourcenmanager - handles the ressource locations *
 *******************************************************
 * If you want to load something, the program will     *
 * search in all ressource locations for the file.     *
 ******************************************************/

#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <LogManager.h>

/** This Class provides an easy to handle access to the Paths of every File of the RessourceLocations registered.
 * @brief Access the Paths of every File which is part of the registered RessourceLocations
 * @author Andy Reimann
 */
class RessourceManager {

	public:
		/** Get a reference to one single instance.
		 * @return A reference of one single instance.
		 */
		static RessourceManager& getSingleton( );
		/** Get a pointer to one single instance.
		 * @return A pointer of one single instance.
		 */
		static RessourceManager* getSingletonPtr( );
		/** Add a path to the Ressource Locations.
		 * @param loc the Location to add to the RessourceLocations.
		 */
		void addRessourceLocation( std::string const& loc );
		/** Get the full path to a file.
		 * @param filename The Filename to get the Path from.
		 * @note If the FileName was not found in any RessourceLocation, an Warning will be created and an empty string will be returned.
		 */
		std::string getPath( std::string const& filename );
		/** Destroys the one single instance.
		 */
		static void destroy();

	private:
		/** This internal Function tests, whether a File exists in a specific path.
		 * @param file The FileName to search for.
		 * @param path The Path to search in for the FileName.
		 */
		bool fileExists( std::string const& file, std::string const& path );
		static RessourceManager* mInstance_; //!< the one single instance
		RessourceManager(); //!< constructor
		~RessourceManager(); //!< destructor

		std::vector<std::string> mLocations_; //!< The list of registered RessourceLocations

};

#endif