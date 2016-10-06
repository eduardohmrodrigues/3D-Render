#pragma once
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <stdlib.h>
#include <gl\glut.h>
#include "glcinMatrix.h"
#include "glcinStructs.h"
#include "SOIL.h"

using namespace std;

class glcinObject
{
public:
	string filename;

	vector<Point3D> vertex, normals;
	vector<Point2D> texture;
	vector<Face> faces;
	vector<Point3D> faceNormals;//only if the object dont have normals
	Point3D axisCoord[3];

	GLfloat color[3]; //cuidar disso, está gambiarrado no renderObject
	GLuint	 texture_image;
	GLdouble objectOperations[4][4];
	GLdouble objectScale[4][4];

	GLdouble limits[6];

	GLdouble xPosition, yPosition, zPosition, rotX, rotY, rotZ;

	Face scanFace(char buffer[256]);


	glcinObject();
	~glcinObject();

	void loadObject(string filepath);									//load the object stored in filepath
	void write_object();
	void renderObject();												//render the loaded object
	void renderObjectMultMatrix(GLdouble matrixOperations[4][4]);		//mult the coords of object and render
	void renderLines();													//render the lines of the object
	void renderArrows();												//render the axis arrows of the object
	bool faceType(char buffer[256]);									//check if the face is write f x y z, f v/vt/vn or f v//vn
	void calcNormalsFromBuffer(char buffer[256]);									//calculate the object normals for each face
	void glcinObject::calcVertexNormals();								//calculate the vertex normals
	Point3D vectorProduct(Point3D vector1, Point3D vector2);			//calculate the vetorial product
	void normalizeObject();												//normalize the object size
	void triangulate_faces();
	void calc_face_normals();

	void get_limits();
	//void subdivision_surface();
	//GLint binomial(GLint n, GLint k);
};

