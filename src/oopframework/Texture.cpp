#include <Texture.h>

Texture::Texture() :
	texID(0),
	texType(GL_TEXTURE_2D),
	minFilter(GL_NEAREST),
	magFilter(GL_NEAREST),
	mipMaps(false),
	name(""),
	refCnt(0),
	isPrivate(true) {
}

void
Texture::bind() {
	if( !glIsEnabled( GL_TEXTURE_2D ) )
		glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, texID );
}

void
Texture::unbind() {
	if( glIsEnabled( GL_TEXTURE_2D ) )
		glDisable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, NULL );
}

unsigned 
Texture::getWidth() const {
	return width;
}

unsigned 
Texture::getHeight() const {
	return height;
}

bool
Texture::operator ==( Texture const& rhs ) {
	if( texID == rhs.texID &&
		texType == rhs.texType &&
		minFilter == rhs.minFilter &&
		magFilter == rhs.magFilter &&
		mipMaps == rhs.mipMaps && 
		name == rhs.name &&
		isPrivate == rhs.isPrivate )
		return true;
	else
		return false;
}

Texture::~Texture() {
}
