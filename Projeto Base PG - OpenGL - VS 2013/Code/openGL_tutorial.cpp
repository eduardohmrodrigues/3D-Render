
#include "openGL_tutorial.h"
#include <iostream>
#include <math.h>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
//maybe
#include <cstdio>

using namespace std;

//STRUCTS
struct CartesianPoint{
	float points[3];

	CartesianPoint(float a, float b, float c)
	{
		points[0] = a;
		points[1] = b;
		points[2] = c;
	};
};

struct Face{
	vector<int> faces;

	//Face(vector<float> a) : faces(a) {};
};

struct Object{
	//string name;
	vector<CartesianPoint> vertex;
	vector<CartesianPoint> normals;
	vector<CartesianPoint> texture;
	vector<Face> faces;

	Object(vector<CartesianPoint> a, vector<CartesianPoint> b, vector<CartesianPoint> c, vector<Face> d) : vertex(a), normals(b), texture(c), faces(d) {};
};

//METHODS
int loadObject(string filename);
void renderObjects(void);
Face scanFace(char buffer[]);
void scanTexture(); //to Implement
CartesianPoint rotatePoint(float matrix[4][4], CartesianPoint point);


//VARIABLES
vector<Object> objects;
#define PI 3.14159265

//TEST
float near = -150;
float far = 150;
string objName = "cat.obj";


int loadObject(string filename){
	vector<CartesianPoint> vertex;
	vector<CartesianPoint> normals;
	vector<CartesianPoint> texture;
	vector<Face> faces;

	ifstream in(filename);

	if (!in.is_open()){
		cout << "File not opened" << endl;
		return -1;
	}

	char buffer [256];
	float tempX, tempY, tempZ;

	while (!in.eof()){

		in.getline(buffer, 256);

		if (buffer[0] == 'v' && buffer[1] == ' '){
			sscanf(buffer, "v %f %f %f", &tempX, &tempY, &tempZ);
			//cout << tempX << " " << tempY << " " << tempZ << endl;
			vertex.push_back(CartesianPoint(tempX, tempY, tempZ));
		}
		else if (buffer[0] == 'v' && buffer[1] == 'n'){
			sscanf(buffer, "vn %f %f %f", &tempX, &tempY, &tempZ);
			//cout << tempX << " " << tempY << " " << tempZ << endl;
			normals.push_back(CartesianPoint(tempX, tempY, tempZ));
		}
		else if (buffer[0] == 'v' && buffer[1] == 't'){
			//scanTexture();
		}
		else if (buffer[0] == 'f'){
			Face tempFace = scanFace(buffer);
			faces.push_back(tempFace);
		}
	}

	objects.push_back(Object(vertex, normals, texture, faces));
	return 0;
}

void renderObjects(void){
	for (int i = 0; i < objects.size(); i++){

		vector<CartesianPoint> objNormals = objects[i].normals;
		vector<Face> objFaces = objects[i].faces;
		vector<CartesianPoint> objVertex = objects[i].vertex;
		vector<CartesianPoint> objTexture = objects[i].texture;

		for (int j = 0; j < objFaces.size(); j++){
			glColor3f(1, 0, 0);
			glBegin(GL_POLYGON);
			if (objNormals.size()>0){
				glNormal3f(objNormals[(objFaces[j].faces[0] - 1)].points[0], objNormals[(objFaces[j].faces[0] - 1)].points[1], objNormals[(objFaces[j].faces[0] - 1)].points[2]);
			}
			for (int k = 0; k < objFaces[j].faces.size(); k++){//call all the vertex who makes the face[k]
				glVertex3f(objVertex[(objFaces[j].faces[k] - 1)].points[0], objVertex[(objFaces[j].faces[k] - 1)].points[1], objVertex[(objFaces[j].faces[k] - 1)].points[2]);
			}
			glEnd();
		}
		//glFlush();
	}
}

Face scanFace(char buffer[]){
	Face faceOut;
	char * face;
	face = strtok(buffer, " "); //strtok makes the split of the buffer char [] separating by spaces. This function 'tokenize' the string
	while (face != NULL)
	{
		if (face[0] == 'f'){
			face = strtok(NULL, " ");
		}
		else{
			faceOut.faces.push_back(stoi(face)); //stoi makes a string become a integer
			
			face = strtok(NULL, " "); //the null parameter makes this function keep spliting the last sucesfull word who he splits
		}
	}
	return faceOut;
}

void renderFunction(void)
{
	float sizeScreen = 150;
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	float pos[] = { 6, 5, -2, 5};
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glOrtho(-sizeScreen, sizeScreen, -sizeScreen, sizeScreen, near, far);
	//glLoadIdentity();

	if (objects.size() > 0){
		renderObjects();
	}

	glFlush();
	glutSwapBuffers();
}

void reshapeFunction(GLsizei w, GLsizei h)
{
}

void hadleKeyboard(GLubyte key, GLint x, GLint y)
{
	if (key == 27){
		exit(0);
	}
}
void hadleSpecialKeyboard(GLint key, GLint x, GLint y)
{

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	float matrixOperation[4][4];
	bool isOperation = false;
	if (key == GLUT_KEY_RIGHT){
		double senRotacao = sin(5 * PI / 180);
		double cosRotacao = cos(5 * PI / 180);
		matrixOperation[0][0] = cosRotacao;
		matrixOperation[0][1] = 0;
		matrixOperation[0][2] = senRotacao;
		matrixOperation[0][3] = 0;
		matrixOperation[1][0] = 0;
		matrixOperation[1][1] = 1;
		matrixOperation[1][2] = 0;
		matrixOperation[1][3] = 0;
		matrixOperation[2][0] = -senRotacao;
		matrixOperation[2][1] = 0;
		matrixOperation[2][2] = cosRotacao;
		matrixOperation[2][3] = 0;
		matrixOperation[3][0] = 0;
		matrixOperation[3][1] = 0;
		matrixOperation[3][2] = 0;
		matrixOperation[3][3] = 1;
		isOperation = true;
	}
	else if (key == GLUT_KEY_LEFT){
		double senRotacao = sin(-5 * PI / 180);
		double cosRotacao = cos(-5 * PI / 180);
		matrixOperation[0][0] = cosRotacao;
		matrixOperation[0][1] = 0;
		matrixOperation[0][2] = senRotacao;
		matrixOperation[0][3] = 0;
		matrixOperation[1][0] = 0;
		matrixOperation[1][1] = 1;
		matrixOperation[1][2] = 0;
		matrixOperation[1][3] = 0;
		matrixOperation[2][0] = -senRotacao;
		matrixOperation[2][1] = 0;
		matrixOperation[2][2] = cosRotacao;
		matrixOperation[2][3] = 0;
		matrixOperation[3][0] = 0;
		matrixOperation[3][1] = 0;
		matrixOperation[3][2] = 0;
		matrixOperation[3][3] = 1;
		isOperation = true;
	}
	else if (key == GLUT_KEY_UP){
		double senRotacao = sin(5 * PI / 180);
		double cosRotacao = cos(5 * PI / 180);
		matrixOperation[0][0] = 1;
		matrixOperation[0][1] = 0;
		matrixOperation[0][2] = 0;
		matrixOperation[0][3] = 0;
		matrixOperation[1][0] = 0;
		matrixOperation[1][1] = cosRotacao;
		matrixOperation[1][2] = -senRotacao;
		matrixOperation[1][3] = 0;
		matrixOperation[2][0] = 0;
		matrixOperation[2][1] = senRotacao;
		matrixOperation[2][2] = cosRotacao;
		matrixOperation[2][3] = 0;
		matrixOperation[3][0] = 0;
		matrixOperation[3][1] = 0;
		matrixOperation[3][2] = 0;
		matrixOperation[3][3] = 1;
		isOperation = true;
	}
	else if (key == GLUT_KEY_DOWN){
		double senRotacao = sin(-5 * PI / 180);
		double cosRotacao = cos(-5 * PI / 180);
		matrixOperation[0][0] = 1;
		matrixOperation[0][1] = 0;
		matrixOperation[0][2] = 0;
		matrixOperation[0][3] = 0;
		matrixOperation[1][0] = 0;
		matrixOperation[1][1] = cosRotacao;
		matrixOperation[1][2] = -senRotacao;
		matrixOperation[1][3] = 0;
		matrixOperation[2][0] = 0;
		matrixOperation[2][1] = senRotacao;
		matrixOperation[2][2] = cosRotacao;
		matrixOperation[2][3] = 0;
		matrixOperation[3][0] = 0;
		matrixOperation[3][1] = 0;
		matrixOperation[3][2] = 0;
		matrixOperation[3][3] = 1;
		isOperation = true;
	}
	else if (key == GLUT_KEY_F1){
		far += 10;
		//near -= 1;
	}

	if (isOperation){
		for (int i = 0; i < objects.size(); i++){
			for (int j = 0; j < objects[i].vertex.size(); j++){
				objects[i].vertex[j] = rotatePoint(matrixOperation, objects[i].vertex[j]);
			}
		}

		for (int i = 0; i < objects.size(); i++){
			for (int j = 0; j < objects[i].normals.size(); j++){
				objects[i].normals[j] = rotatePoint(matrixOperation, objects[i].normals[j]);
			}
		}

		for (int i = 0; i < objects.size(); i++){
			for (int j = 0; j < objects[i].texture.size(); j++){
				objects[i].texture[j] = rotatePoint(matrixOperation, objects[i].texture[j]);
			}
		}
	}
	renderFunction();
}

CartesianPoint rotatePoint(float matrix[4][4], CartesianPoint point){
	float tempX, tempY, tempZ;
	tempX = matrix[0][0] * point.points[0] + matrix[0][1] * point.points[1] + matrix[0][2] * point.points[2];
	tempY = matrix[1][0] * point.points[0] + matrix[1][1] * point.points[1] + matrix[1][2] * point.points[2];
	tempZ = matrix[2][0] * point.points[0] + matrix[2][1] * point.points[1] + matrix[2][2] * point.points[2];
	CartesianPoint out(tempX, tempY, tempZ);
	return out;
}

void mouseClick(GLint btn, GLint state, GLint x, GLint y)
{
}

void mouseMove(GLint x, GLint y)
{
}

void testeObject(){
	for (int i = 0; i < objects.size(); i++){

		vector<CartesianPoint> objNormals = objects[i].normals;
		vector<Face> objFaces = objects[i].faces;
		vector<CartesianPoint> objVertex = objects[i].vertex;
		vector<CartesianPoint> objTexture = objects[i].texture;

		for (int j = 0; j < objVertex.size(); j++){
			cout << "v " << objVertex[j].points[0] << " " << objVertex[j].points[1] << " " << objVertex[j].points[2] << endl;
		}

		for (int j = 0; j < objNormals.size(); j++){
			cout << "vn " << objNormals[j].points[0] << " " << objNormals[j].points[1] << " " << objNormals[j].points[2] << endl;
		}

		for (int j = 0; j < objFaces.size(); j++){
			cout << "f ";
			cout << objFaces[j].faces.size() << " ";
			for (int k = 0; k < objFaces[j].faces.size(); k++){
				cout << objFaces[j].faces[k] << " ";
			}
			cout << endl;
		}
		//glFlush();
	}
}

int main(void)
{

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(560, 140);
	glutCreateWindow("OpenGL - Meu cubo 3D");
	glEnable(GL_DEPTH_TEST);

	glutDisplayFunc(renderFunction);
	glutReshapeFunc(reshapeFunction);
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMove);
	glutKeyboardUpFunc(hadleKeyboard);
	glutSpecialUpFunc(hadleSpecialKeyboard);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//int obj = loadObject("cubo.obj");
	//cout << "cat " << endl;
	int obj = loadObject(objName);
	renderObjects();
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	float col[] = {1, 1, 1, 1};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, col);
	//testeObject();
	glutMainLoop();

	return 0;

}