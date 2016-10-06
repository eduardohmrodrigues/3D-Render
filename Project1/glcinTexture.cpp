#include "glcinTexture.h"


glcinTexture::glcinTexture()
{
}


glcinTexture::~glcinTexture()
{
}


GLuint glcinTexture::LoadGLTextures(const char* filename)                                    // Load Bitmaps And Convert To Textures
{
	GLuint texture;
	// load an image file directly as a new OpenGL texture
	texture = SOIL_load_OGL_texture
		(
		filename,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
		);

	if (texture == 0){
		cout << "Texture not opened" << endl;
	}
	return texture;
}