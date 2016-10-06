#pragma once
#include <vector>

using namespace std;

enum coord_ref{
	GL_CIN_X = 0,
	GL_CIN_Y = 1,
	GL_CIN_Z = 2
};

struct Point2D{
	GLdouble x, y;
};

struct Point3D{
	GLdouble x, y, z;
};

struct Point4D{
	GLdouble x, y, z, w;
};

struct Face{
	vector<GLint> vertex;
	vector<GLint> normals;
	GLint normal;			//normal of face
	vector<GLint> texture;
};

struct Light{
	Point4D color;
	Point4D position;
};

struct Camera{
	GLdouble rotX, rotY, rotZ;
	GLdouble operations[4][4];
	Point3D position;
};