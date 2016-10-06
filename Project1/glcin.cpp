#include "glcin.h"


glcin::glcin()
{
}

glcin::~glcin()
{
}

//load and store the obj passed in filepath
void glcin::loadObject(string filepath){
	glcinObject temp;
	temp.loadObject(filepath);

	objects.push_back(temp);
}

//render all loaded objects on the scene
void glcin::renderObjects(){
	for (GLint i = 0; i < objects.size(); i++){
		if ((i == selectedObject && !isWired) || i != selectedObject){
			objects[i].renderObject();
		}
		if (i == selectedObject){
			objects[i].renderLines();
			objects[i].renderArrows();
		}
	}
}

//render objects
void glcin::renderObjectsDirector(){
	for (GLint i = 0; i < objects.size(); i++){
		objects[i].renderObject();
	}
	//camera.renderObjectMultMatrix(directorOperations);
}

void glcin::initDirector(){
	glcinMatrix::identity(directorCamera.operations);

	GLdouble matrixY[4][4], matrixX[4][4], matrixDirectorRot[4][4], temp[4][4];

	//set the directors rotation and translation
	glcinMatrix::rotationY(-90, matrixY);
	glcinMatrix::rotationX(30, matrixX);
	glcinMatrix::multMatrix(directorCamera.operations, matrixX, matrixY);

	directorCamera.operations[0][3] = 0;
	directorCamera.operations[1][3] = 3;
	directorCamera.operations[2][3] = -25;
	directorCamera.operations[3][3] = 1;
	//==========================================

	//init the operation matrix of camera obj
	glcinMatrix::identity(directorOperations);

	director = false;
}

void glcin::createLight(){
	bool newLight = false;
	Point4D temp;
	Point4D aux;

	if (lightNumber == 8){
		cout << "Max number of light sources" << endl;
	}
	else {
		cout << "Enter the light position: \nx = ";
		cin >> temp.x;
		cout << "y = ";
		cin >> temp.y;
		cout << "z = ";
		cin >> temp.z;
		cout << "w = ";
		cin >> temp.w;
		cout << endl;

		cout << "Enter the light color: \nR = ";
		cin >> aux.x;
		cout << "G = ";
		cin >> aux.y;
		cout << "B = ";
		cin >> aux.z;
		cout << "A = ";
		cin >> aux.w;
		cout << endl;

		float col[] = { (GLfloat)aux.x, (GLfloat)aux.y, (GLfloat)aux.z, (GLfloat)aux.w };
		switch (lightNumber){
		case 0:
			glEnable(GL_LIGHT0);
			glLightfv(GL_LIGHT0, GL_DIFFUSE, col);
			lightNumber++;
			newLight = true;
			break;
		case 1:
			glEnable(GL_LIGHT1);
			glLightfv(GL_LIGHT1, GL_DIFFUSE, col);
			lightNumber++;
			newLight = true;
			break;
		case 2:
			glEnable(GL_LIGHT2);
			glLightfv(GL_LIGHT2, GL_DIFFUSE, col);
			lightNumber++;
			newLight = true;
			break;
		case 3:
			glEnable(GL_LIGHT3);
			glLightfv(GL_LIGHT3, GL_DIFFUSE, col);
			lightNumber++;
			newLight = true;
			break;
		case 4:
			glEnable(GL_LIGHT4);
			glLightfv(GL_LIGHT4, GL_DIFFUSE, col);
			lightNumber++;
			newLight = true;
			break;
		case 5:
			glEnable(GL_LIGHT5);
			glLightfv(GL_LIGHT5, GL_DIFFUSE, col);
			lightNumber++;
			newLight = true;
			break;
		case 6:
			glEnable(GL_LIGHT6);
			glLightfv(GL_LIGHT6, GL_DIFFUSE, col);
			lightNumber++;
			newLight = true;
			break;
		case 7:
			glEnable(GL_LIGHT7);
			glLightfv(GL_LIGHT7, GL_DIFFUSE, col);
			lightNumber++;
			newLight = true;
			break;
		}
	}

	if (newLight){
		Light light;
		light.color = aux;
		light.position = temp;
		lights.push_back(light);
	}
}

void renderLightCubes(Light lights){ //render a cube in the position of the selected light
	glDisable(GL_LIGHTING);
	glBegin(GL_LINE_STRIP);
	glColor3f(1, 0, 0);
	// BASE
	glVertex3f(lights.position.x -0.1, lights.position.y-0.1, lights.position.z-0.1);
	glVertex3f(lights.position.x-0.1, lights.position.y-0.1, lights.position.z+0.1);
	glVertex3f(lights.position.x + 0.1, lights.position.y-0.1, lights.position.z+0.1);
	glVertex3f(lights.position.x + 0.1, lights.position.y-0.1, lights.position.z-0.1);
	glVertex3f(lights.position.x - 0.1, lights.position.y - 0.1, lights.position.z - 0.1);
	glEnd();

	//TOP
	glBegin(GL_LINE_STRIP);
	glColor3f(1, 0, 0);
	glVertex3f(lights.position.x - 0.1, lights.position.y + 0.1, lights.position.z - 0.1);
	glVertex3f(lights.position.x - 0.1, lights.position.y + 0.1, lights.position.z + 0.1);
	glVertex3f(lights.position.x + 0.1, lights.position.y + 0.1, lights.position.z + 0.1);
	glVertex3f(lights.position.x + 0.1, lights.position.y + 0.1, lights.position.z - 0.1);
	glVertex3f(lights.position.x - 0.1, lights.position.y + 0.1, lights.position.z - 0.1);
	glEnd();

	//LEFT
	glBegin(GL_LINE_STRIP);
	glColor3f(1, 0, 0);
	glVertex3f(lights.position.x - 0.1, lights.position.y + 0.1, lights.position.z - 0.1);
	glVertex3f(lights.position.x - 0.1, lights.position.y + 0.1, lights.position.z + 0.1);
	glVertex3f(lights.position.x - 0.1, lights.position.y - 0.1, lights.position.z + 0.1);
	glVertex3f(lights.position.x - 0.1, lights.position.y - 0.1, lights.position.z - 0.1);
	glVertex3f(lights.position.x - 0.1, lights.position.y + 0.1, lights.position.z - 0.1);
	glEnd();

	//RIGHT
	glBegin(GL_LINE_STRIP);
	glColor3f(1, 0, 0);
	glVertex3f(lights.position.x + 0.1, lights.position.y + 0.1, lights.position.z - 0.1);
	glVertex3f(lights.position.x + 0.1, lights.position.y + 0.1, lights.position.z + 0.1);
	glVertex3f(lights.position.x + 0.1, lights.position.y - 0.1, lights.position.z + 0.1);
	glVertex3f(lights.position.x + 0.1, lights.position.y - 0.1, lights.position.z - 0.1);
	glVertex3f(lights.position.x + 0.1, lights.position.y + 0.1, lights.position.z - 0.1);
	glEnd();

	glEnable(GL_LIGHTING);
}

void glcin::renderLights(){
	GLint aux = -1;
	if (selectedObject > objects.size()-1){
		aux = selectedObject - objects.size();
	}

	if (lightNumber > 0){
		if (aux == 0){
			renderLightCubes(lights[0]);
		}
		float pos[] = { lights[0].position.x, lights[0].position.y, lights[0].position.z, lights[0].position.w };
		glLightfv(GL_LIGHT0, GL_POSITION, pos);
	}
	if (lightNumber > 1){
		if (aux == 1){
			renderLightCubes(lights[1]);
		}
		float pos[] = { lights[1].position.x, lights[1].position.y, lights[1].position.z, lights[1].position.w };
		glLightfv(GL_LIGHT1, GL_POSITION, pos);
	}
	if (lightNumber > 2){
		if (aux == 2){
			renderLightCubes(lights[2]);
		}
		float pos[] = { lights[2].position.x, lights[2].position.y, lights[2].position.z, lights[2].position.w };
		glLightfv(GL_LIGHT2, GL_POSITION, pos);
	}
	if (lightNumber > 3){
		if (aux == 3){
			renderLightCubes(lights[3]);
		}
		float pos[] = { lights[3].position.x, lights[3].position.y, lights[3].position.z, lights[3].position.w };
		glLightfv(GL_LIGHT3, GL_POSITION, pos);
	}
	if (lightNumber > 4){
		if (aux == 4){
			renderLightCubes(lights[4]);
		}
		float pos[] = { lights[4].position.x, lights[4].position.y, lights[4].position.z, lights[4].position.w };
		glLightfv(GL_LIGHT4, GL_POSITION, pos);
	}
	if (lightNumber > 5){
		if (aux == 5){
			renderLightCubes(lights[5]);
		}
		float pos[] = { lights[5].position.x, lights[5].position.y, lights[5].position.z, lights[5].position.w };
		glLightfv(GL_LIGHT5, GL_POSITION, pos);
	}
	if (lightNumber > 6){
		if (aux == 6){
			renderLightCubes(lights[6]);
		}
		float pos[] = { lights[6].position.x, lights[6].position.y, lights[6].position.z, lights[6].position.w };
		glLightfv(GL_LIGHT6, GL_POSITION, pos);
	}
	if (lightNumber > 7){
		if (aux == 7){
			renderLightCubes(lights[7]);
		}
		float pos[] = { lights[7].position.x, lights[7].position.y, lights[7].position.z, lights[7].position.w };
		glLightfv(GL_LIGHT7, GL_POSITION, pos);
	}
}

void glcin::translateCamera(GLdouble x, GLdouble y, GLdouble z){
	GLdouble buff[4][4];

	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			buff[i][j] = userCamera.operations[i][j];
		}
	}

	userCamera.operations[0][3] += x;
	userCamera.operations[1][3] += y;
	userCamera.operations[2][3] += z;

	directorOperations[0][3] -= x;
	directorOperations[1][3] -= y;
	directorOperations[2][3] -= z;

	calculatePosition(userCamera.position);

	//cout << next_position.x << " " << next_position.y << " " << next_position.z << endl;
	//next_position.x = userCamera.operations[0][3];
	//next_position.y = userCamera.operations[1][3];
	//next_position.z = userCamera.operations[2][3];
	//
	is_colliding = check_collision();
	
	if (is_colliding){
		directorOperations[0][3] += x;
		directorOperations[1][3] += y;
		directorOperations[2][3] += z;

		for (int i = 0; i < 4; i++){
			for (int j = 0; j < 4; j++){
				userCamera.operations[i][j] = buff[i][j];
			}
		}

		userCamera.position.x = next_position.x; 
		userCamera.position.y = next_position.y; 
		userCamera.position.z = next_position.z;

		is_colliding = false;
	}
}

void glcin::calculatePosition(Point3D positionOut){
	//achar a inclinação da reta diretora da camera e calculare
	//a translação em cada eixo de acordo com o seno
	GLdouble out[4][4];
	glcinMatrix::zero(out);

	glcinMatrix::invert(userCamera.operations, out);
	next_position.x = out[0][3]; 
	next_position.y = out[1][3];
	next_position.z = out[2][3];
}

void glcin::translateDirectorCamera(GLdouble x, GLdouble y, GLdouble z){
	directorCamera.operations[0][3] += x;
	directorCamera.operations[1][3] += y;
	directorCamera.operations[2][3] += z;
}

void glcin::rotateCamera(GLdouble rot, GLdouble x, GLdouble y, GLdouble z){
	GLdouble matrixOperation[4][4], matrixOperation2[4][4];
	bool isOperation = false;
	
	if (x == 1){
		userCamera.rotX += rot;
		if (userCamera.rotX > 359){
			userCamera.rotX = userCamera.rotX - 359;
		}
		else if (userCamera.rotX < 0){
			userCamera.rotX = userCamera.rotX + 359;
		}

		glcinMatrix::rotationX(rot, matrixOperation);
		isOperation = true;

	} else if (y == 1){
		userCamera.rotY += rot;
		if (userCamera.rotY > 359){
			userCamera.rotY = userCamera.rotY - 359;
		}
		else if (userCamera.rotY < 0){
			userCamera.rotY = userCamera.rotY + 359;
		}

		Point3D axis;
		axis.x = userCamera.operations[0][1];
		axis.y = userCamera.operations[1][1];
		axis.z = userCamera.operations[2][1];

		glcinMatrix::rodrigues(axis, rot, matrixOperation);
		isOperation = true;

	}
	else if (z == 1){
		userCamera.rotZ += rot;
		if (userCamera.rotZ > 359){
			userCamera.rotZ = userCamera.rotZ - 359;
		}
		else if (userCamera.rotZ < 0){
			userCamera.rotZ = userCamera.rotZ + 359;
		}

		glcinMatrix::rotationZ(rot, matrixOperation);
		isOperation = true;
	}

	if (isOperation){
		GLdouble temp[4][4];
		for (int i = 0; i < 4; i++){
			for (int j = 0; j < 4; j++){
				temp[i][j] = userCamera.operations[i][j];
			}
		}

		glcinMatrix::multMatrix(userCamera.operations, matrixOperation, temp);
	}	
}

void glcin::translateObject(GLint selectedObject, GLdouble x, GLdouble y, GLdouble z){
		objects[selectedObject].xPosition += x;
		objects[selectedObject].yPosition += y;
		objects[selectedObject].zPosition += z;

		objects[selectedObject].objectOperations[0][3] = objects[selectedObject].xPosition;
		objects[selectedObject].objectOperations[1][3] = objects[selectedObject].yPosition;
		objects[selectedObject].objectOperations[2][3] = objects[selectedObject].zPosition;

		objects[selectedObject].get_limits();
}

void glcin::scaleObject(GLint selectedObject, GLdouble scaleX, GLdouble scaleY, GLdouble scaleZ){
	objects[selectedObject].objectScale[0][0] += scaleX;
	objects[selectedObject].objectScale[1][1] += scaleY;
	objects[selectedObject].objectScale[2][2] += scaleZ;

	objects[selectedObject].get_limits();
}

void glcin::rotateObject(GLint selectedObject, GLdouble rot, GLdouble x, GLdouble y, GLdouble z){
	GLdouble matrixOperation[4][4];
	bool isOperation = false;

	if (x == 1){
		objects[selectedObject].rotX += rot;
		glcinMatrix::rotationX(rot, matrixOperation);
		isOperation = true;

	}
	else if (y == 1){
		objects[selectedObject].rotY += rot;
		glcinMatrix::rotationY(rot, matrixOperation);
		isOperation = true;
	}
	else if (z == 1){
		objects[selectedObject].rotZ += rot;
		glcinMatrix::rotationZ(rot, matrixOperation);
		isOperation = true;
	}


	if (isOperation){
		if (objects[selectedObject].rotX > 359){
			objects[selectedObject].rotX = 0;
		}
		else if (objects[selectedObject].rotX < 0){
			objects[selectedObject].rotX = 359;
		}

		if (objects[selectedObject].rotY > 359){
			objects[selectedObject].rotY = 0;
		}
		else if (objects[selectedObject].rotY < 0){
			objects[selectedObject].rotY = 359;
		}
		if (objects[selectedObject].rotZ > 359){
			objects[selectedObject].rotZ = 0;
		}
		else if (objects[selectedObject].rotZ < 0){
			objects[selectedObject].rotZ = 359;
		}

		GLdouble temp[4][4];
		glcinMatrix::identity(temp);
		for (int i = 0; i < 3; i++){
			for (int j = 0; j < 3; j++){
				temp[i][j] = objects[selectedObject].objectOperations[i][j];
			}
		}

		glcinMatrix::multMatrix(objects[selectedObject].objectOperations, matrixOperation, temp);

		objects[selectedObject].objectOperations[0][3] = objects[selectedObject].xPosition;
		objects[selectedObject].objectOperations[1][3] = objects[selectedObject].yPosition;
		objects[selectedObject].objectOperations[2][3] = objects[selectedObject].zPosition;
		objects[selectedObject].objectOperations[3][3] = 1;

		objects[selectedObject].get_limits();
	}
}

void glcin::translateLight(GLint selectedLight, GLdouble x, GLdouble y, GLdouble z){
	lights[selectedLight].position.x += x;
	lights[selectedLight].position.y += y;
	lights[selectedLight].position.z += z;
}

void glcin::createCartesianPlane(GLdouble density, GLdouble left, GLdouble right, GLdouble near, GLdouble far, GLdouble height, GLdouble color[4]){
	glBindTexture(GL_TEXTURE_2D, NULL);
	glDisable(GL_LIGHTING);

	GLdouble temp = left;

	glBegin(GL_LINES);
	while (left < right){
		glColor4d(color[0], color[1], color[2], color[3]);
		glVertex3d(left, height, near);
		left += density;
		glVertex3d(left, height, far);
		left += density;
	}
	glEnd();

	left = temp;

	glBegin(GL_LINES);
	while (near < far){
		glColor4d(color[0], color[1], color[2], color[3]);
		glVertex3d(left, height, near);
		near += density;
		glVertex3d(right, height, near);
		near += density;
	}
	glEnd();

	glEnable(GL_LIGHTING);
}

void glcin::createGround(GLdouble size, GLdouble height){

}

void glcin::startDirector(GLdouble FOV, GLdouble width, GLdouble height, GLdouble planeColor[4]){
	glBindTexture(GL_TEXTURE_2D, NULL);

	GLdouble eduTrans[4][4];

	glEnable(GL_FOG);
	glFogf(GL_FOG_END, fogDensity);
	//First View
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, width / 2, height);
	//gluPerspective(FOV, ((GLfloat)width / 2) / (GLfloat)height, z_near, z_far);
	glFrustum(-(width / 2) / height, (width / 2)/height, -1, 1, z_near, z_far);

	glMatrixMode(GL_MODELVIEW);										//Select the modelview matrix, all transformations will be applied in scene
	glLoadIdentity();												//Reset the modelview matrix to default value

	glcinMatrix::identity(eduTrans);
	glcinMatrix::trans(eduTrans, userCamera.operations);

	glLoadMatrixd(eduTrans[0]);
	//glMultMatrixd(eduTrans[0]);
	renderLights();

	glDisable(GL_LIGHTING);

	glBegin(GL_LINES);
	glColor3d(1, 0, 0);
	glVertex3d(-10, 0, 0);
	glVertex3d(10, 0, 0);
	glColor3d(0, 0, 1);
	glVertex3d(0, -10, 0);
	glVertex3d(0, 10, 0);
	glColor3d(0, 1, 0);
	glVertex3d(0, 0, -10);
	glVertex3d(0, 0, 10);
	glEnd();

	glEnable(GL_LIGHTING);


	renderObjects();
	createCartesianPlane(0.5, -50, 50, -50, 50, -1, planeColor);

	//SECOND VIEW============================
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(width / 2, 0, width / 2, height);
	//gluPerspective(FOV, ((GLfloat)width / 2) / (GLfloat)height, z_near, z_far);
	glFrustum(-(width / 2) / height, (width / 2) / height, -1, 1, 2.5, z_far);

	glMatrixMode(GL_MODELVIEW);										//Select the modelview matrix, all transformations will be applied in scene
	glLoadIdentity();												//Reset the modelview matrix to default value
	glDisable(GL_FOG);
	glFogf(GL_FOG_END, fogDensity);

	GLdouble aux[4][4];
	glcinMatrix::zero(aux);
	glcinMatrix::invert(userCamera.operations, aux);

	glcinMatrix::identity(eduTrans);
	glcinMatrix::trans(eduTrans, directorCamera.operations);
	glLoadMatrixd(eduTrans[0]);
	//glMultMatrixd(eduTrans[0]);
	renderLights();

	createCartesianPlane(0.5, -50, 50, -50, 50, -1, planeColor);
	renderObjects();
	camera.renderObjectMultMatrix(aux);

	glDisable(GL_LIGHTING);

	glBegin(GL_LINES);
	glColor3d(1, 0, 0);
	glVertex3d(-10, 0, 0);
	glVertex3d(10, 0, 0);
	glColor3d(0, 0, 1);
	glVertex3d(0, -10, 0);
	glVertex3d(0, 10, 0);
	glColor3d(0, 1, 0);
	glVertex3d(0, 0, -10);
	glVertex3d(0, 0, 10);
	glEnd();
	glEnable(GL_LIGHTING);
}

void glcin::startUserCamera(GLdouble FOV, GLdouble width, GLdouble height, GLdouble planeColor[4]){
	glBindTexture(GL_TEXTURE_2D, NULL);

	glViewport(0, 0, width, height);
	//gluPerspective(FOV, (GLfloat)width / (GLfloat)height, z_near, z_far);
	glFrustum(-width / height, width / height, -1, 1, z_near, z_far);

	glEnable(GL_FOG);
	glMatrixMode(GL_MODELVIEW);										//Select the modelview matrix, all transformations will be applied in scene
	glLoadIdentity();												//Reset the modelview matrix to default value

	glFogf(GL_FOG_END, fogDensity);
	//glDisable(GL_FOG);
	GLdouble eduTrans[4][4];
	glcinMatrix::identity(eduTrans);
	glcinMatrix::trans(eduTrans, userCamera.operations);

	glLoadMatrixd(eduTrans[0]);
	//glMultMatrixd(eduTrans[0]);
	renderLights();

	glDisable(GL_LIGHTING);

	glBegin(GL_LINES);
	glColor3d(1, 0, 0);
	glVertex3d(-10, 0, 0);
	glVertex3d(10, 0, 0);
	glColor3d(0, 0, 1);
	glVertex3d(0, -10, 0);
	glVertex3d(0, 10, 0);
	glColor3d(0, 1, 0);
	glVertex3d(0, 0, -10);
	glVertex3d(0, 0, 10);
	glEnd();

	glEnable(GL_LIGHTING);


	renderObjects();
	createCartesianPlane(0.5, -50, 50, -50, 50, -1, planeColor);
}

int glcin::keyboardControl(GLubyte key, GLint x, GLint y, GLdouble cameraTranslateVelocity, GLdouble objectTranslateVelocity, GLdouble objectRotateVelocity, GLdouble scaleParameter, GLdouble FOV){
	if (key == 27){
		exit(0);
	}
	else if (key == 'w' || key == 'W'){
		translateCamera(0, 0, cameraTranslateVelocity);
		return 1;
	}
	else if (key == 'a' || key == 'A'){
		translateCamera(cameraTranslateVelocity, 0, 0);
		return 1;
	}
	else if (key == 's' || key == 'S'){
		translateCamera(0, 0, -cameraTranslateVelocity);
		return 1;
	}
	else if (key == 'd' || key == 'D'){
		translateCamera(-cameraTranslateVelocity, 0, 0);
		return 1;
	}
	else if (key == 'x' || key == 'X'){
		isWired = !isWired;
		return 1;
	}
	else if (key == 't' || key == 'T'){
		//objects[selectedObject].subdivision_surface();
		//objects[selectedObject].triangulate_faces();
		objects[selectedObject].write_object();
		return 1;
	}
	else if (key == '1'){
		if (selectedObject < objects.size()){
			translateObject(selectedObject, objectTranslateVelocity, 0, 0);
		}
		else{
			translateLight(selectedObject - objects.size(), objectTranslateVelocity, 0, 0);
		}
		return 1;
	}
	else if (key == '2'){
		if (selectedObject < objects.size()){
			translateObject(selectedObject, -objectTranslateVelocity, 0, 0);
		}
		else{
			translateLight(selectedObject - objects.size(), -objectTranslateVelocity, 0, 0);
		}
		return 1;
	}
	else if (key == '3'){
		if (selectedObject < objects.size()){
			translateObject(selectedObject, 0, objectTranslateVelocity, 0);
		}
		else{
			translateLight(selectedObject - objects.size(), 0, objectTranslateVelocity, 0);
		}
		return 1;
	}
	else if (key == '4'){
		if (selectedObject < objects.size()){
			translateObject(selectedObject, 0, -objectTranslateVelocity, 0);
		}
		else{
			translateLight(selectedObject - objects.size(), 0, -objectTranslateVelocity, 0);
		}
		return 1;
	}
	else if (key == '5'){
		if (selectedObject < objects.size()){
			translateObject(selectedObject, 0, 0, objectTranslateVelocity);
		}
		else{
			translateLight(selectedObject - objects.size(), 0, 0, objectTranslateVelocity);
		}
		return 1;
	}
	else if (key == '6'){
		if (selectedObject < objects.size()){
			translateObject(selectedObject, 0, 0, -objectTranslateVelocity);
		}
		else{
			translateLight(selectedObject - objects.size(), 0, 0, -objectTranslateVelocity);
		}
		return 1;
	}
	else if (key == '7'){
		if (selectedObject < objects.size()){
			rotateObject(selectedObject, objectRotateVelocity, 1, 0, 0);
		}
		return 1;
	}
	else if (key == '8'){
		if (selectedObject < objects.size()){
			rotateObject(selectedObject, objectRotateVelocity, 0, 1, 0);
		}
		return 1;
	}
	else if (key == '9'){
		if (selectedObject < objects.size()){
			rotateObject(selectedObject, objectRotateVelocity, 0, 0, 1);
		}
		return 1;
	}
	else if (key == '>' || key == '.'){
		selectedObject++;
		if (selectedObject > (objects.size() + lightNumber - 1)){
			selectedObject = 0;
		}
		return 1;
	}
	else if (key == '<' || key == ','){
		selectedObject--;
		if (selectedObject < 0){
			selectedObject = objects.size() + lightNumber - 1;
		}
		return 1;
	}
	else if (key == 'c' || key == 'C'){//EXTRA
		loadObject("Resources/Objects/cube.obj");
		return 1;
	}
	else if (key == 'l' || key == 'L'){//EXTRA
		createLight();
		return 1;
	}
	else if (key == 'f' || key == 'F'){
		director = !director;
		return 1;
	}
	else if (key == '+' || key == '='){
		scaleObject(selectedObject, scaleParameter, scaleParameter, scaleParameter);
		return 1;
	}
	else if (key == '-' || key == '_'){
		scaleObject(selectedObject, -scaleParameter, -scaleParameter, -scaleParameter);
		return 1;
	}
	else if (key == 'q' || key == 'Q'){
		if (!isGameMode){
			isGameMode = true;
			glutFullScreen();
		}
		else{
			isGameMode = false;
			glutReshapeWindow(800, 800);
			glutPositionWindow(8, 31);
		}
		return 1;
	}
	return 0;
}

bool glcin::check_collision(){
	bool out = false;
	for (int i = 0; i < objects.size(); i++){

		//cout << next_position.x << " " << objects[i].limits[0] << " " << objects[i].limits[1] << endl;
		//cout << next_position.y << " " << objects[i].limits[2] << " " << objects[i].limits[3] << endl;
		//cout << next_position.z << " " << objects[i].limits[4] << " " << objects[i].limits[5] << endl << endl;

		if (next_position.x >= objects[i].limits[0] && next_position.x <= objects[i].limits[1] && next_position.y >= objects[i].limits[2] && next_position.y <= objects[i].limits[3] && next_position.z >= objects[i].limits[4] && next_position.z <= objects[i].limits[5]){
			i = objects.size();
			out = true;
		}
	}

	return out;
}