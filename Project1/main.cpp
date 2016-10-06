#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "GL\glut.h"
#include "glcin.h"
#include "ctime"
#include "SOIL.h"

#include "glcinTexture.h"

using namespace std;

//SHOW FPS ON CONSOLE
#define SHOW_FPS false

//Variables==============
glcin gledu;								//classe "fachada"
GLdouble width = 800, height = 800;			//window size
GLdouble planeColor[4] = { 1, 1, 1, 0 };	//cartesianPlane color

GLdouble cameraTranslateVelocity = 0.1;
GLdouble cameraRotateVelocity = 2;

GLdouble scaleParameter = 0.1;				//parameter for apply scale in objects
GLdouble objectTranslateVelocity = 1;
GLdouble objectRotateVelocity = 1;
int selectedObject = 0;

GLdouble z_near = 2.5, z_far = 500, FOV = 45;	//near, far and field of view of the camera

bool isRotating = false;		//bool to rotate the camera with mouse move

int mouseX, mouseY;								//mouse pointer position
GLdouble mouseSensiblity = 10;					//mouse sensibilty to rotate camera    -  smaller is faster!

GLuint	 textures[10];							//textures array
GLdouble fogDensity = 15;					
GLdouble fogVelocity = 0.5;
GLfloat  fogColor[4] = {0, 0, 0, 0};				//fog color
GLdouble dollyZoomVelocity = 0.25;
GLdouble dollyTranslateVelocity = 1.6;


GLdouble backgroundColor[4] = {0, 0, 0, 0};


bool keyboardButtons[256];


time_t aux_time, start_time, time_now;
int fps;
//=======================

/*//not used
GLuint loadBMP_custom(const char * imagepath){
	// Data read from the header of the BMP file
	unsigned char header[54]; // Each BMP file begins by a 54-bytes header
	unsigned int dataPos;     // Position in the file where the actual data begins
	unsigned int width, height;
	unsigned int imageSize;   // = width*height*3
	// Actual RGB data
	unsigned char * data;

	// Open the file
	FILE * file = fopen(imagepath, "rb");
	if (!file){ 
		printf("Image could not be opened\n"); 
		return 0; 
	}

	//The first thing in the file is a 54-bytes header, lets read it
	if (fread(header, 1, 54, file) != 54){ // If not 54 bytes read : problem
		printf("Not a correct BMP file\n");
		return false;
	}

	//The header always begins by BM
	if (header[0] != 'B' || header[1] != 'M'){
		printf("Not a correct BMP file\n");
		return 0;
	}

	// Read ints from the byte array
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	// Some BMP files have missing information
	if (imageSize == 0)    imageSize = width*height * 3; // 3 = one byte for each Red, Green and Blue component
	if (dataPos == 0)      dataPos = 54;				 // The BMP header is done that way

	// Create a buffer
	data = new unsigned char[imageSize];

	// Read the actual data from the file into the buffer
	fread(data, 1, imageSize, file);

	//Now we can close the file
	fclose(file);

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	return textureID;
}
*/

void renderFunction(void)
{

	if (SHOW_FPS){
		fps++;
		time(&time_now);
		//cout << time_now << endl;

		if (time_now - aux_time > 1){
			time(&time_now);
			time(&aux_time);
			cout << fps << " fps" << endl;
			fps = 0;
		}
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				//Clean the color buffer and depth buffer
	//glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3]);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//clean the texture buffer
	glBindTexture(GL_TEXTURE_2D, NULL);

	if (gledu.director){
		gledu.startDirector(FOV, width, height, planeColor);
	}
	else{
		gledu.startUserCamera(FOV, width, height, planeColor);
	}
	

	glFlush();														//this paints everything
	glutSwapBuffers();												//Swap the buffers? I think this works like "i painted everything in another buffer and now i have to swap the atual buffer to the painted buffer"
}

void reshapeFunction(GLsizei w, GLsizei h){
	width = w;
	height = h;
}

void handleKeyboard(GLubyte key, GLint x, GLint y)
{
	if (key == 'n' || key == 'N'){
		keyboardButtons[key] = true;
	}
	if (key == 'z' || key == 'Z'){
		keyboardButtons[key] = true;
	}
	if (key == 'z' || key == 'X'){
		keyboardButtons[key] = true;
	}

	int render = gledu.keyboardControl(key, x, y, cameraTranslateVelocity, objectTranslateVelocity, objectRotateVelocity, scaleParameter, FOV);
	if (render == 1){
		renderFunction();
	}
}

void keyboardUp(GLubyte key, GLint x, GLint y){
	if (key == 'n' || key == 'N'){
		keyboardButtons[key] = false;
	}
	if (key == 'z' || key == 'Z'){
		keyboardButtons[key] = false;
	}
	if (key == 'z' || key == 'X'){
		keyboardButtons[key] = false;
	}
}

void hadleSpecialKeyboard(GLint key, GLint x, GLint y)
{
	if (key == GLUT_KEY_UP){

	}
	else if (key == GLUT_KEY_DOWN){

	}
	else if (key == GLUT_KEY_LEFT){

	}
	else if (key == GLUT_KEY_RIGHT){

	}
}

void mouseClick(GLint btn, GLint state, GLint x, GLint y)
{
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		mouseX = x;
		mouseY = y;
		isRotating = true;
	}
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_UP){
		isRotating = false;
	}

	if (btn == GLUT_WHEEL_UP){
		if (keyboardButtons['n'] || keyboardButtons['N']){
			gledu.fogDensity -= fogVelocity;
			renderFunction();
		}
		if ((keyboardButtons['z'] || keyboardButtons['Z']) && gledu.z_near < 60){
			gledu.z_near += dollyZoomVelocity;
			gledu.translateCamera(0, 0, -dollyTranslateVelocity);
			renderFunction();
		}
	}
	else if (btn == GLUT_WHEEL_DOWN){
		if (keyboardButtons['n'] || keyboardButtons['N']){
			gledu.fogDensity += fogVelocity;
			renderFunction();
		}
		if ((keyboardButtons['z'] || keyboardButtons['Z']) && gledu.z_near > 2.5){
			gledu.z_near -= dollyZoomVelocity;
			gledu.translateCamera(0, 0, dollyTranslateVelocity);
			renderFunction();
		}
	}
}

void mouseMove(GLint x, GLint y)
{
	if (isRotating){
		if (x > mouseX + mouseSensiblity){
			gledu.rotateCamera(cameraRotateVelocity, 0, 1, 0);
			mouseX = x;
		}
		else if (x < mouseX - mouseSensiblity){
			gledu.rotateCamera(-cameraRotateVelocity, 0, 1, 0);
			mouseX = x;
		}

		if (y > mouseY + mouseSensiblity){
			gledu.rotateCamera(cameraRotateVelocity, 1, 0, 0);
			mouseY = y;
		}
		else if (y < mouseY - mouseSensiblity){
			gledu.rotateCamera(-cameraRotateVelocity, 1, 0, 0);
			mouseY = y;
		}
		renderFunction();
	}
}

void init(){
	
	time(&start_time);
	time(&time_now);
	time(&aux_time);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3]);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("GL Cin - Projeto 1 PG");
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glutDisplayFunc(renderFunction);
	glutReshapeFunc(reshapeFunction);
	glutKeyboardFunc(handleKeyboard);
	glutKeyboardUpFunc(keyboardUp);
	glutSpecialFunc(hadleSpecialKeyboard);
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMove);

	
	
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	
	glFogi(GL_FOG_INDEX, 32);
	glFogf(GL_FOG_START, 1);
	glFogfv(GL_FOG_COLOR, fogColor);


	glEnable(GL_LIGHTING);
	Point4D aux;
	Point4D temp;
	temp.x = 0;
	temp.y = 10;
	temp.z = 0;
	temp.w = 0;

	aux.x = 1;
	aux.y = 1;
	aux.z = 1;
	aux.w = 5;
	gledu.lightNumber++;
	Light light;
	light.color = aux;
	light.position = temp;
	gledu.lights.push_back(light);

	glEnable(GL_LIGHT0);
	GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position[] = { 1.0, 1.0, 0.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glcinMatrix::identity(gledu.userCamera.operations);
	glcinMatrix::identity(gledu.directorCamera.operations);
	gledu.isWired = false;
	gledu.isGameMode = false;
	gledu.initDirector();
	gledu.z_near = z_near;
	gledu.z_far = z_far;
	gledu.fogDensity = fogDensity;
	gledu.fogVelocity = fogVelocity;
	gledu.dollyZoomVelocity = dollyZoomVelocity;
	gledu.dollyTranslateVelocity = dollyTranslateVelocity;

	gledu.next_position.x = 0; gledu.next_position.y = 0; gledu.next_position.z = 0;
	gledu.is_colliding = false;
	gledu.detect_collision = false;

	gledu.camera.loadObject("Resources/Objects/camera.obj");
}

int main(){
	cout << "**************************************************" << endl;
	cout << "* Object Loader - Version 1.0 - Created by EHMR  *" << endl;
	cout << "* Contact: ehmr@cin.ufpe.br                      *" << endl;
	cout << "* About me: www.cin.ufpe.br/~ehmr                *" << endl;
	cout << "* Last Updated: 8/29/2015  16:06h                *" << endl;
	cout << "*                                                *" << endl;
	cout << "**************************************************\n\n\n" << endl;
	cout << "Controls: Q - start/exit fullscreen" << endl;
	cout << "          F - director view mode in/out" << endl;
	cout << "          L - create new light source" << endl;
	cout << "		   T - generate .obj of selected obj" << endl;
	cout << "          < - select previous object" << endl;
	cout << "          > - select next object" << endl;
	cout << "          + - scale object in 1%" << endl;
	cout << "          - - scale object in -1%" << endl;
	cout << "          X - selected object changes to wire frame" << endl;
	cout << "          Z + scroll up   - dolly zoom in" << endl;
	cout << "          Z + scroll down - dolly zoom out" << endl;
	cout << "          N + scroll up   - Fog density in" << endl;
	cout << "          N + scroll down - Fog density out" << endl;
	cout << "          LMB + mouse move - rotate camera" << endl;
	cout << "          W, A, S, D - move camera" << endl;
	cout << "          1, 2, 3, 4, 5, 6, 7, 8, 9 - move object" << endl;

	init();

	//gledu.loadObject("Resources/Objects/cubo_texture.obj");
	//
	//glGenTextures(1, &gledu.objects[0].texture_image);
	//gledu.objects[0].texture_image = glcinTexture::LoadGLTextures("Resources/textures/NeHe.bmp");
	//gledu.translateObject(0, 2, 0, -10);

	gledu.loadObject("Resources/Objects/yoda.obj");
	gledu.loadObject("Resources/Objects/camel.obj");
	gledu.translateObject(1, -2, 0, -10);
	gledu.loadObject("Resources/Objects/lion.obj");
	gledu.translateObject(2, -4, 0, -10);
	gledu.loadObject("Resources/Objects/whale.obj");
	gledu.translateObject(3, 6, 0, 0);
	gledu.rotateObject(3, 90, 0, 1, -10);
	gledu.loadObject("Resources/Objects/chimp.obj");
	gledu.translateObject(4, -6, 0, -10);
	gledu.loadObject("Resources/Objects/venus.obj");
	gledu.translateObject(5, 8, 0,-10);
	gledu.rotateObject(5, 180, 1, 0, 0);
	gledu.loadObject("Resources/Objects/dog.obj");
	gledu.translateObject(6, -8, 0, -10);
	gledu.rotateObject(6, -90, 0, 1, 0);
	gledu.loadObject("Resources/Objects/spheretri.obj");
	gledu.translateObject(7, 10, 0, -10);
	
	gledu.translateCamera(0, 0, -10);

	glutMainLoop();	
	
	return 0;
}