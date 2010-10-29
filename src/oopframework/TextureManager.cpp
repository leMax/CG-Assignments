#include <TextureManager.h>

// SINGLETON
TextureManager* TextureManager::mInstance_ = NULL;

TextureManager&
TextureManager::getSingleton( ) {
  if( !mInstance_ )
    mInstance_ = new TextureManager();
  return *mInstance_;
}

TextureManager*
TextureManager::getSingletonPtr( ) {
  if( !mInstance_ )
    mInstance_ = new TextureManager();
  return mInstance_;
}

TextureManager::TextureManager() {
}

Texture*
TextureManager::loadTexture( std::string tex, GLuint texType, 
							 GLuint minFilter, GLuint magFilter, bool dstFormat, bool forceReload ) {
     
	// get the full path of the texture
	tex = RessourceManager::getSingletonPtr()->getPath( tex ) + tex;
    // check the parameters
	bool generateMipMaps = checkParamState( texType, minFilter, magFilter );

	LogManager* l = LogManager::getSingletonPtr();
	std::stringstream log;
	if( !forceReload ) {
		log << "TextureManager: Searching in Texturepool for texture '" << tex << "'...";
		l->logMessage( log );
		std::list< Texture >::iterator it = mTexturePool_.begin();
		while( it != mTexturePool_.end() ) {
			if( tex == it->name && !it->isPrivate ) {
				log.str("");
				log << "TextureManager: done. Texture is previousely loaded. No load required.\n";
				l->logMessage( log );
				it->refCnt += 1;
				return &(*it);
			}
			++it;
		}
		log.str("");
		log << "TextureManager: done.\nTexture wasn't loaded before so we try to load it now...";
		l->logMessage( log );
	}
	else {
		log.str("");
		log << "TextureManager: Try to load the Texture now...";
		l->logMessage( log );
	}
	
	// create a new ILImage
	ILuint imageID;				// index for DevIL texture
	ilGenImages(1,&imageID);	// generate IL-ID for texture
	ilBindImage(imageID);		// bind ID as current Texture
	// get the Data from IL
	ILboolean ret = ilLoadImage ( tex.c_str() );

	if( !ret ) {
		ilDeleteImages(1,&imageID);
		log.str("");
		log << "TextureManager Error: Could not load Imagefile '" << tex << "'.";
		l->logMessage( log );
		return NULL;
	}
	unsigned char * texData = ilGetData();

	// check if something is wrong
	if( texData == NULL ) {
		ilDeleteImages(1,&imageID);
		log.str("");
		log << "TextureManager Error: Could not retrieve Texture Data of ImageFile '" << tex << "'. Maybe the file is corrupted.";
		l->logMessage( log );
		return NULL;
	}
	else {
		// get the texture properties
		unsigned width		= ilGetInteger( IL_IMAGE_WIDTH );
		unsigned height		= ilGetInteger( IL_IMAGE_HEIGHT );	
		unsigned components = ilGetInteger( IL_IMAGE_BYTES_PER_PIXEL );
		unsigned format		= ilGetInteger( IL_IMAGE_FORMAT );	
		log.str("");
		log << "TextureManager: Properties:" << 
				" width = " << width <<  
				" height = " << height <<  
				" components = " << components <<  
				" format = " << format;
		l->logMessage( log );	

		// now we check if the width and height of the image is too big for the current machine
		GLint texSize; 
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texSize);
		if( width > (unsigned)texSize ||
			height > (unsigned)texSize ) {
			log.str("");
			log << "TextureManager Warning: The dimensions of the Texture '" << tex << "' are bigger than the maximum supported dimension of " << texSize << "Pixels.\n";
			l->logMessage( log );	
			return NULL;
		}

		// create OpenGL texture
		GLuint GLtexture;
		glGenTextures(1, &GLtexture);
		// bind the texture to the next action we make
		glBindTexture(   texType, GLtexture );
		glTexParameteri( texType, GL_TEXTURE_MIN_FILTER, minFilter );
		glTexParameteri( texType, GL_TEXTURE_MAG_FILTER, magFilter );
		glTexParameterf( texType, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameterf( texType, GL_TEXTURE_WRAP_T, GL_REPEAT );
		if( generateMipMaps )
			glTexParameteri(texType, GL_GENERATE_MIPMAP, GL_TRUE);	
		// 
		if( dstFormat == true ) 
			glTexImage2D(texType, 0, components, width, height, 0, format, GL_UNSIGNED_BYTE, texData);
		else
			glTexImage2D(texType, 0, dstFormat, width, height, 0, format, GL_UNSIGNED_BYTE, texData);
		// create a new Texture
		Texture unit;
		unit.texID = GLtexture;
		unit.texType = texType;
		unit.minFilter = minFilter;
		unit.magFilter = magFilter;
		unit.mipMaps = generateMipMaps;
		unit.name = tex;
		unit.refCnt = 1;
		unit.isPrivate = forceReload;
		unit.width = width;
		unit.height = height;

		log.str("");
		log << "TextureManager: Texture successfully loaded";
		l->logMessage( log );		
		// save the loaded texture in the texture pool
		mTexturePool_.push_back( unit );
		// free memory of DevIL
		ilDeleteImages(1,&imageID);
		glBindTexture( texType, NULL );
		return &(mTexturePool_.back());
	}
}

bool
TextureManager::checkParamState( GLuint& texType, 
								 GLuint& minFilter, 
								 GLuint& magFilter ) {
	LogManager* l = LogManager::getSingletonPtr();
	std::stringstream log;
	// check the texture type 
	if( texType != GL_TEXTURE_2D && texType != GL_TEXTURE_2D ) {
		log << "TextureManager: Warning: unknown texturetype - setting texturetype to GL_TEXTURE_2D.";
		l->logMessage( log );
		texType = GL_TEXTURE_2D;
	}
	// check the minFilter
	if( minFilter != GL_NEAREST && minFilter != GL_LINEAR &&
		minFilter != GL_NEAREST_MIPMAP_NEAREST &&
		minFilter != GL_NEAREST_MIPMAP_LINEAR  && 
		minFilter != GL_LINEAR_MIPMAP_NEAREST  && 
		minFilter != GL_LINEAR_MIPMAP_LINEAR ) {
		log << "TextureManager: Warning: unknown Min-Filter - setting Min-Filter to GL_NEAREST_MIPMAP_LINEAR.";
		l->logMessage( log );
		minFilter = GL_NEAREST_MIPMAP_LINEAR;
	}
	// check the magFilter
	if( magFilter != GL_NEAREST && magFilter != GL_LINEAR &&
		minFilter != GL_NEAREST_MIPMAP_NEAREST &&
		minFilter != GL_NEAREST_MIPMAP_LINEAR  && 
		minFilter != GL_LINEAR_MIPMAP_NEAREST  && 
		minFilter != GL_LINEAR_MIPMAP_LINEAR  ) {
		log << "TextureManager: Warning: unknown Mag-Filter - setting Mag-Filter to GL_LINEAR.";
		l->logMessage( log );
		magFilter = GL_LINEAR;
	}	
	bool hasMipMapFilter = false;
	// check the minFilter if it has mipmapping
	if( minFilter == GL_NEAREST_MIPMAP_NEAREST ||
		minFilter == GL_NEAREST_MIPMAP_LINEAR  || 
		minFilter == GL_LINEAR_MIPMAP_NEAREST  || 
		minFilter == GL_LINEAR_MIPMAP_LINEAR ) {
		hasMipMapFilter = true;
	}
	return hasMipMapFilter;
}

void
TextureManager::deleteTexture( Texture* t ) {
	if( t != NULL ) {
		if( t->isPrivate )
			glDeleteTextures(1, &t->texID);
		else if(t->refCnt > 1)
			t->refCnt -= 1;
		else {
			glDeleteTextures(1, &t->texID);
			// delete it from the list if it is in
			mTexturePool_.remove( *t );
		}

	}
}

void
TextureManager::destroy() {
  if( mInstance_ )
    delete mInstance_;
  mInstance_ = NULL;
}

TextureManager::~TextureManager() {
	
}
