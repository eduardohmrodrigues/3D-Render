#pragma once
#include "SOIL.h"
#include <iostream>
#include "GL\glut.h"

using namespace std;

class glcinTexture
{
public:
	glcinTexture();
	~glcinTexture();

	static GLuint LoadGLTextures(const char* filename);

};

