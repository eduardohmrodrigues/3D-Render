#pragma once

#include <stdlib.h>
#include "glcinMatrix.h"
#include "glcinStructs.h"
#include "glcinObject.h"
#include "GL\glut.h"
#include <string>
#include <vector>

//using namespace std;

class glcin
{
public:
	vector<glcinObject> objects;
	vector<Light> lights;
	glcinObject camera;
	Camera userCamera;
	Camera directorCamera;
	GLdouble directorOperations[4][4];
	int lightNumber;
	int selectedObject;

	bool director, isWired, isGameMode;
	GLdouble z_near;
	GLdouble z_far;
	GLdouble fogDensity;
	GLdouble dollyZoomVelocity;
	GLdouble dollyTranslateVelocity;
	GLdouble fogVelocity;

	Point3D next_position;
	bool is_colliding;
	bool detect_collision;
	

public:
	glcin();
	~glcin();

	void loadObject(string filepath);															//load the object stored in filepath
	void renderObjects();																		//render all objects in the scene
	void renderObjectsDirector();																//render the objects in director view
	void initDirector();																		//initialize the parameter of director's view
	void renderCamera();																		//render the camera object
	void createLight();																			//create a new light in the scene
	void renderLights();																		//render all lights
	void translateCamera(GLdouble x, GLdouble y, GLdouble z);									//translate user camera
	void calculatePosition(Point3D positionOut);
	void translateDirectorCamera(GLdouble x, GLdouble y, GLdouble z);
	void rotateCamera(GLdouble rot, GLdouble x, GLdouble y, GLdouble z);						//rotate user camera	
	void translateObject(GLint selectedObject, GLdouble x, GLdouble y, GLdouble z);				//translate the selected object
	void scaleObject(GLint selectedObject, GLdouble scaleX, GLdouble scaleY, GLdouble scaleZ);	//scale the selected object
	void rotateObject(GLint selectedObject, GLdouble rot, GLdouble x, GLdouble y, GLdouble z);	//rotate the selected object
	void translateLight(GLint selectedLight, GLdouble x, GLdouble y, GLdouble z);				//translate the selected light
	void createCartesianPlane(GLdouble density, GLdouble left, GLdouble right, GLdouble near, GLdouble far, GLdouble height, GLdouble color[4]);	//create a cartesian plane in the scene
	void createGround(GLdouble size, GLdouble height);											//create a ground in scene
	void startDirector(GLdouble FOV, GLdouble width, GLdouble height,  GLdouble planeColor[4]);
	void startUserCamera(GLdouble FOV, GLdouble width, GLdouble height, GLdouble planeColor[4]);
	int keyboardControl(GLubyte key, GLint x, GLint y, GLdouble cameraTranslateVelocity, GLdouble objectTranslateVelocity, GLdouble objectRotateVelocity, GLdouble scaleParameter, GLdouble FOV);
	bool check_collision();
};
