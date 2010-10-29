#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <IL/il.h>

#include <Texture.h>
#include <LogManager.h>
#include <RessourceManager.h>

#include <list>
#include <string>

/** This class provides an interface for loading textures.
 * It also takes care of not double loading any texture since it will safe all previously 
 * loaded textures internally.
 */
class TextureManager {
	public:
		/** Get a reference to one single instance.
		 * @return A reference of one single instance.
		 */
		static TextureManager& getSingleton( );
		/** Get a pointer to one single instance.
		 * @return A pointer of one single instance.
		 */
		static TextureManager* getSingletonPtr( );
        /** This function simply loads a texture from a file and returns the ID of this texture.
		 * @param tex The name of the Texture.
		 * @param texType The type of Texture (currently only GLTEXTURE_2D is possible).
		 * @param minFilter The type of Min Filter to use. Possible Min Filters are: GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR.
		 * @param magFilter The type of Mag Filter to use. Possible Mag Filters are: GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR.
		 * @param dstFormat The destination Format of the Texture. If this value is set to true, the loading routine will try to auto detect the format of the texture. 
		 * If not, valid formats are GL_RGB, GL_RGB4, GL_RGB8, GL_RGB12, GL_RGB16, GL_RGBA, GL_RGBA4, GL_RGBA8, GL_RGBA12, GL_RGBA16, GL_LUMINANCE, GL_LUMINANCE4, GL_LUMINANCE8, GL_LUMINANCE12, GL_LUMINANCE16, GL_DEPTH16, GL_DEPTH24, GL_DEPTH32.
		 * @param forceReload If true, the Texture will be reloaded even if it is already loaded in a previouse step.
		 * @note The name of the Texture has to be available in any of the Registered RessourceLogations of the RessourceManager
		 */
		Texture* loadTexture( std::string tex, 
							GLuint texType = GL_TEXTURE_2D, 
							GLuint minFilter = GL_NEAREST_MIPMAP_LINEAR, 
							GLuint magFilter = GL_NEAREST_MIPMAP_LINEAR,
							bool dstFormat = true,
							bool forceReload = false );		
		/** This will delete a Texture if no reference exists anymore (refCnt) or the Texture itself is private.
		 * @param t A Pointer to the Texture to delete.
		 */
		void deleteTexture( Texture* t );
		/** Destroys the one single instance.
		 */
		static void destroy();
	private:
		static TextureManager* mInstance_; //!< the one single instance

		bool checkParamState( GLuint& texType, 
							  GLuint& minFilter, 
							  GLuint& magFilter);

		TextureManager();	//!< constructor
		~TextureManager();	//!< destructor

		std::list< Texture > mTexturePool_; //!< List of previously loaded textures to avoid float load
};