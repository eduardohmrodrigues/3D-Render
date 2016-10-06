#define _CRT_SECURE_NO_WARNINGS
#include "glcinObject.h"


glcinObject::glcinObject()
{
}


glcinObject::~glcinObject()
{
}

//load the .obj file passed on the filepath
void glcinObject::loadObject(string filepath){
	filename = filepath.substr(filepath.find_last_of("/") + 1, filepath.size());
	filename = filename.substr(0, filename.find(".obj"));

	cout << "Loading " << filename << "..." << endl;

	ifstream in(filepath); //Open the file

	if (!in.is_open()){
		cout << "file not opened" << endl;
		//throw exception("File not opened");
	}

	char buffer[256];
	float tempX, tempY, tempZ;


	while (!in.eof()){

		in.getline(buffer, 256);	//read the first line
		
		if (buffer[0] == 'v' && buffer[1] == ' '){						//if is a vertex
			sscanf(buffer, "v %f %f %f", &tempX, &tempY, &tempZ);		//read the vertex coords
			Point3D temp;
			temp.x = tempX;
			temp.y = tempY;
			temp.z = tempZ;
			vertex.push_back(temp);										//save the vertex coords in the vertex vector
		}
		else if (buffer[0] == 'v' && buffer[1] == 'n'){					//if is a normal vector
			sscanf(buffer, "vn %f %f %f", &tempX, &tempY, &tempZ);		//read the normal vector
			Point3D temp;
			temp.x = tempX;
			temp.y = tempY;
			temp.z = tempZ;
			normals.push_back(temp);									//save the normal vector in the normals vector
		}
		else if (buffer[0] == 'v' && buffer[1] == 't'){					//if is a texture vertex
			sscanf(buffer, "vt %f %f", &tempX, &tempY);		//read the normal vector
			Point2D temp;
			temp.x = tempX;
			temp.y = tempY;
			texture.push_back(temp);
		}
		else if (buffer[0] == 'f'){										//if is a face
			scanFace(buffer);											//scan the face
		}

		//Clean buffer
		for (int i = 0; i < 256; i++){
			buffer[i] = '#';
		}
	}

	if (faceNormals.size() > 0){
		calcVertexNormals();
	}

	normalizeObject();

	glcinMatrix::identity(objectOperations);
	glcinMatrix::identity(objectScale);

	get_limits();
}

//render the respective object on the opengl window
void glcinObject::renderObject(){
	color[0] = 1;
	color[1] = 1;
	color[2] = 1;

	for (int j = 0; j < faces.size(); j++){
		glColor3f(color[0], color[1], color[2]);
		GLfloat col[4] = { 1, 1, 1, 0 };//GAMBIARRAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
		
		
		GLfloat ambient[] = { color[0], color[1], color[2], .0 };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	
		//GLfloat diffuse[] = { 1.0, 0.8, 0.0, 1.0 };
		//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
		//GLfloat specular[] = { 0.0, 0.0, 1.0, 1.0 };
		//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
		//glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 80.0);
		//GLfloat emission[] = { .5, 0.0, 0.0, 1.0 };
		//glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
		
		if (normals.size() == 0)
			triangulate_faces();

		if (texture.size() > 0)
			glBindTexture(GL_TEXTURE_2D, texture_image);
		else
			glBindTexture(GL_TEXTURE_2D, NULL);

		glBegin(GL_POLYGON);
		for (int k = 0; k < faces[j].vertex.size(); k++){//call all the vertex who makes the face[k]
			Point3D aux;
			Point3D temp;

			aux.x = objectOperations[0][0] * normals[(faces[j].normals[k] - 1)].x + objectOperations[0][1] * normals[(faces[j].normals[k] - 1)].y + objectOperations[0][2] * normals[(faces[j].normals[k] - 1)].z;
			aux.y = objectOperations[1][0] * normals[(faces[j].normals[k] - 1)].x + objectOperations[1][1] * normals[(faces[j].normals[k] - 1)].y + objectOperations[1][2] * normals[(faces[j].normals[k] - 1)].z;
			aux.z = objectOperations[2][0] * normals[(faces[j].normals[k] - 1)].x + objectOperations[2][1] * normals[(faces[j].normals[k] - 1)].y + objectOperations[2][2] * normals[(faces[j].normals[k] - 1)].z;

			glNormal3d(aux.x, aux.y, aux.z);
			
			//use the face normal to render
			//aux.x = objectOperations[0][0] * faceNormals[(faces[j].normal - 1)].x + objectOperations[0][1] * faceNormals[(faces[j].normal - 1)].y + objectOperations[0][2] * faceNormals[(faces[j].normal - 1)].z;
			//aux.y = objectOperations[1][0] * faceNormals[(faces[j].normal - 1)].x + objectOperations[1][1] * faceNormals[(faces[j].normal - 1)].y + objectOperations[1][2] * faceNormals[(faces[j].normal - 1)].z;
			//aux.z = objectOperations[2][0] * faceNormals[(faces[j].normal - 1)].x + objectOperations[2][1] * faceNormals[(faces[j].normal - 1)].y + objectOperations[2][2] * faceNormals[(faces[j].normal - 1)].z;
			//
			//glNormal3d(aux.x, aux.y, aux.z);

			if (texture.size() > 0){
				aux.x = objectOperations[0][0] * texture[(faces[j].texture[k] - 1)].x + objectOperations[0][1] * texture[(faces[j].texture[k] - 1)].y;
				aux.y = objectOperations[1][0] * texture[(faces[j].texture[k] - 1)].x + objectOperations[1][1] * texture[(faces[j].texture[k] - 1)].y;

				glTexCoord2f(aux.x, aux.y);
			}


			aux.x = objectScale[0][0] * vertex[(faces[j].vertex[k] - 1)].x + objectScale[0][1] * vertex[(faces[j].vertex[k] - 1)].y + objectScale[0][2] * vertex[(faces[j].vertex[k] - 1)].z + objectScale[0][3];
			aux.y = objectScale[1][0] * vertex[(faces[j].vertex[k] - 1)].x + objectScale[1][1] * vertex[(faces[j].vertex[k] - 1)].y + objectScale[1][2] * vertex[(faces[j].vertex[k] - 1)].z + objectScale[1][3];
			aux.z = objectScale[2][0] * vertex[(faces[j].vertex[k] - 1)].x + objectScale[2][1] * vertex[(faces[j].vertex[k] - 1)].y + objectScale[2][2] * vertex[(faces[j].vertex[k] - 1)].z + objectScale[2][3];

			temp.x = objectOperations[0][0] * aux.x + objectOperations[0][1] * aux.y + objectOperations[0][2] * aux.z + objectOperations[0][3];
			temp.y = objectOperations[1][0] * aux.x + objectOperations[1][1] * aux.y + objectOperations[1][2] * aux.z + objectOperations[1][3];
			temp.z = objectOperations[2][0] * aux.x + objectOperations[2][1] * aux.y + objectOperations[2][2] * aux.z + objectOperations[2][3];
			glVertex3d(temp.x, temp.y, temp.z);
		}
		glEnd();
	}

	//for (int k = 0; k < faceNormals.size(); k++){
	//	cout << faceNormals[k].x << " -- " << faceNormals[k].y << " -- " << faceNormals[k].z << endl; //SACAQUIIII
	//}
	//cout << endl;
}

//render this object multiplying the coordinates by the passed matrix
void glcinObject::renderObjectMultMatrix(GLdouble matrixOperations[4][4]){
	color[0] = 0;
	color[1] = 0;
	color[2] = 1;

	for (int j = 0; j < faces.size(); j++){
		glColor3f(color[0], color[1], color[2]);
		GLfloat col[4] = { 1, 1, 1, 0 };//GAMBIARRAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA


		GLfloat ambient[] = { color[0], color[1], color[2], .0 };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);


		glBegin(GL_POLYGON);
		for (int k = 0; k < faces[j].vertex.size(); k++){//call all the vertex who makes the face[k]
			Point3D aux, temp, last;

			if (normals.size() > 0){
				aux.x = objectOperations[0][0] * normals[(faces[j].normals[k] - 1)].x + objectOperations[0][1] * normals[(faces[j].normals[k] - 1)].y + objectOperations[0][2] * normals[(faces[j].normals[k] - 1)].z;
				aux.y = objectOperations[1][0] * normals[(faces[j].normals[k] - 1)].x + objectOperations[1][1] * normals[(faces[j].normals[k] - 1)].y + objectOperations[1][2] * normals[(faces[j].normals[k] - 1)].z;
				aux.z = objectOperations[2][0] * normals[(faces[j].normals[k] - 1)].x + objectOperations[2][1] * normals[(faces[j].normals[k] - 1)].y + objectOperations[2][2] * normals[(faces[j].normals[k] - 1)].z;

				glNormal3d(aux.x, aux.y, aux.z);
			}

			aux.x = objectScale[0][0] * vertex[(faces[j].vertex[k] - 1)].x + objectScale[0][1] * vertex[(faces[j].vertex[k] - 1)].y + objectScale[0][2] * vertex[(faces[j].vertex[k] - 1)].z + objectScale[0][3];
			aux.y = objectScale[1][0] * vertex[(faces[j].vertex[k] - 1)].x + objectScale[1][1] * vertex[(faces[j].vertex[k] - 1)].y + objectScale[1][2] * vertex[(faces[j].vertex[k] - 1)].z + objectScale[1][3];
			aux.z = objectScale[2][0] * vertex[(faces[j].vertex[k] - 1)].x + objectScale[2][1] * vertex[(faces[j].vertex[k] - 1)].y + objectScale[2][2] * vertex[(faces[j].vertex[k] - 1)].z + objectScale[2][3];

			temp.x = objectOperations[0][0] * aux.x + objectOperations[0][1] * aux.y + objectOperations[0][2] * aux.z + objectOperations[0][3];
			temp.y = objectOperations[1][0] * aux.x + objectOperations[1][1] * aux.y + objectOperations[1][2] * aux.z + objectOperations[1][3];
			temp.z = objectOperations[2][0] * aux.x + objectOperations[2][1] * aux.y + objectOperations[2][2] * aux.z + objectOperations[2][3];

			last.x = matrixOperations[0][0] * temp.x + matrixOperations[0][1] * temp.y + matrixOperations[0][2] * temp.z + matrixOperations[0][3];
			last.y = matrixOperations[1][0] * temp.x + matrixOperations[1][1] * temp.y + matrixOperations[1][2] * temp.z + matrixOperations[1][3];
			last.z = matrixOperations[2][0] * temp.x + matrixOperations[2][1] * temp.y + matrixOperations[2][2] * temp.z + matrixOperations[2][3];


			glVertex3d(last.x, last.y, last.z);
		}
		glEnd();
	}
}

//render the object wireframe
void glcinObject::renderLines(){

	glBindTexture(GL_TEXTURE_2D, NULL);
	glDisable(GL_LIGHTING);
	for (int j = 0; j < faces.size(); j++){
		glBegin(GL_LINE_STRIP);
		glColor3f(1, 0, 0);

		for (int k = 0; k < faces[j].vertex.size(); k++){//call all the vertex who makes the face[k]
			Point3D aux;
			Point3D temp;

			aux.x = objectScale[0][0] * vertex[(faces[j].vertex[k] - 1)].x + objectScale[0][1] * vertex[(faces[j].vertex[k] - 1)].y + objectScale[0][2] * vertex[(faces[j].vertex[k] - 1)].z + objectScale[0][3];
			aux.y = objectScale[1][0] * vertex[(faces[j].vertex[k] - 1)].x + objectScale[1][1] * vertex[(faces[j].vertex[k] - 1)].y + objectScale[1][2] * vertex[(faces[j].vertex[k] - 1)].z + objectScale[1][3];
			aux.z = objectScale[2][0] * vertex[(faces[j].vertex[k] - 1)].x + objectScale[2][1] * vertex[(faces[j].vertex[k] - 1)].y + objectScale[2][2] * vertex[(faces[j].vertex[k] - 1)].z + objectScale[2][3];

			temp.x = objectOperations[0][0] * aux.x + objectOperations[0][1] * aux.y + objectOperations[0][2] * aux.z + objectOperations[0][3];
			temp.y = objectOperations[1][0] * aux.x + objectOperations[1][1] * aux.y + objectOperations[1][2] * aux.z + objectOperations[1][3];
			temp.z = objectOperations[2][0] * aux.x + objectOperations[2][1] * aux.y + objectOperations[2][2] * aux.z + objectOperations[2][3];
			glVertex3d(temp.x, temp.y, temp.z);
		}
		glEnd();
	}
	glEnable(GL_LIGHTING);
}

//render the axis arrows of the object
void glcinObject::renderArrows(){
	glDisable(GL_LIGHTING);

	Point3D aux;
	Point3D temp;

	//rendering the x axis
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);

	aux.x = objectScale[0][0] + objectScale[0][3];
	aux.y = objectScale[1][0] + objectScale[1][3];
	aux.z = objectScale[2][0] + objectScale[2][3];

	temp.x = objectOperations[0][0] * aux.x + objectOperations[0][1] * aux.y + objectOperations[0][2] * aux.z + objectOperations[0][3];
	temp.y = objectOperations[1][0] * aux.x + objectOperations[1][1] * aux.y + objectOperations[1][2] * aux.z + objectOperations[1][3];
	temp.z = objectOperations[2][0] * aux.x + objectOperations[2][1] * aux.y + objectOperations[2][2] * aux.z + objectOperations[2][3];
	glVertex3d(temp.x, temp.y, temp.z);
	axisCoord[0] = temp;

	aux.x = objectScale[0][3];
	aux.y = objectScale[1][3];
	aux.z = objectScale[2][3];

	temp.x = objectOperations[0][0] * aux.x + objectOperations[0][1] * aux.y + objectOperations[0][2] * aux.z + objectOperations[0][3];
	temp.y = objectOperations[1][0] * aux.x + objectOperations[1][1] * aux.y + objectOperations[1][2] * aux.z + objectOperations[1][3];
	temp.z = objectOperations[2][0] * aux.x + objectOperations[2][1] * aux.y + objectOperations[2][2] * aux.z + objectOperations[2][3];
	glVertex3d(temp.x, temp.y, temp.z);

	glEnd();

	//rendering the y axis
	glBegin(GL_LINES);
	glColor3f(0, 1, 0);

	aux.x = objectScale[0][1] + objectScale[0][3];
	aux.y = objectScale[1][1] + objectScale[1][3];
	aux.z = objectScale[2][1] + objectScale[2][3];

	temp.x = objectOperations[0][0] * aux.x + objectOperations[0][1] * aux.y + objectOperations[0][2] * aux.z + objectOperations[0][3];
	temp.y = objectOperations[1][0] * aux.x + objectOperations[1][1] * aux.y + objectOperations[1][2] * aux.z + objectOperations[1][3];
	temp.z = objectOperations[2][0] * aux.x + objectOperations[2][1] * aux.y + objectOperations[2][2] * aux.z + objectOperations[2][3];
	glVertex3d(temp.x, temp.y, temp.z);
	axisCoord[1] = temp;

	aux.x = objectScale[0][3];
	aux.y = objectScale[1][3];
	aux.z = objectScale[2][3];

	temp.x = objectOperations[0][0] * aux.x + objectOperations[0][1] * aux.y + objectOperations[0][2] * aux.z + objectOperations[0][3];
	temp.y = objectOperations[1][0] * aux.x + objectOperations[1][1] * aux.y + objectOperations[1][2] * aux.z + objectOperations[1][3];
	temp.z = objectOperations[2][0] * aux.x + objectOperations[2][1] * aux.y + objectOperations[2][2] * aux.z + objectOperations[2][3];
	glVertex3d(temp.x, temp.y, temp.z);

	glEnd();

	//rendering the z axis
	glBegin(GL_LINES);
	glColor3f(0, 0, 1);

	aux.x = objectScale[0][2] + objectScale[0][3];
	aux.y = objectScale[1][2] + objectScale[1][3];
	aux.z = objectScale[2][2] + objectScale[2][3];

	temp.x = objectOperations[0][0] * aux.x + objectOperations[0][1] * aux.y + objectOperations[0][2] * aux.z + objectOperations[0][3];
	temp.y = objectOperations[1][0] * aux.x + objectOperations[1][1] * aux.y + objectOperations[1][2] * aux.z + objectOperations[1][3];
	temp.z = objectOperations[2][0] * aux.x + objectOperations[2][1] * aux.y + objectOperations[2][2] * aux.z + objectOperations[2][3];
	glVertex3d(temp.x, temp.y, temp.z);
	axisCoord[2] = temp;

	aux.x = objectScale[0][3];
	aux.y = objectScale[1][3];
	aux.z = objectScale[2][3];

	temp.x = objectOperations[0][0] * aux.x + objectOperations[0][1] * aux.y + objectOperations[0][2] * aux.z + objectOperations[0][3];
	temp.y = objectOperations[1][0] * aux.x + objectOperations[1][1] * aux.y + objectOperations[1][2] * aux.z + objectOperations[1][3];
	temp.z = objectOperations[2][0] * aux.x + objectOperations[2][1] * aux.y + objectOperations[2][2] * aux.z + objectOperations[2][3];
	glVertex3d(temp.x, temp.y, temp.z);

	glEnd();

	glEnable(GL_LIGHTING);
}

//detect what facetype is on the buffer and return the respective face
Face glcinObject::scanFace(char buffer[256])
{
	Face faceOut;

	//type1 = f a b c
	//type2 = f a/b/c
	//type3 = f a//c
	bool type1 = true, type2 = false, type3 = false;

	for (int i = 0; i < 256; i++){				//verify the face type in the .obj
		if (buffer[i] == '/' && buffer[i + 1] == '/'){
			type3 = true;
			type1 = false;
			i = 256;
		}
		else if (buffer[i] == '/'){
			type2 = true;
			type1 = false;
			i = 256;
		}
	}
	
	//scanning the face
	char * face;

	//strtok makes the split of the buffer char [] separating by spaces. This function 'tokenize' the string
	if (type1){
		face = strtok(buffer, " ");
		int v = 0;
		while (face != NULL){
			if (face[0] == 'f')
				face = strtok(NULL, " ");
			else{
				faceOut.vertex.push_back(stoi(face));
				face = strtok(NULL, " ");
			}
		}
		faces.push_back(faceOut);
	}
	else if (type2){
		face = strtok(buffer, " ");
		while (face != NULL)
		{
			if (face[0] == 'f'){
				face = strtok(NULL, " ");
			}
			else{
				int v, vt, vn;
				sscanf(face, "%d/%d/%d", &v, &vt, &vn);
				faceOut.vertex.push_back(v);
				faceOut.texture.push_back(vt);
				faceOut.normals.push_back(vn);

				face = strtok(NULL, " ");
			}
		}
		faces.push_back(faceOut);
	}
	else if (type3){
		face = strtok(buffer, " ");
		while (face != NULL)
		{
			if (face[0] == 'f'){
				face = strtok(NULL, " ");
			}
			else{
				int v, vn;
				sscanf(face, "%d//%d", &v, &vn);
				faceOut.vertex.push_back(v);
				faceOut.normals.push_back(vn);
				face = strtok(NULL, " ");
				//cout << v << "//" << vn << endl;
			}
		}
		faces.push_back(faceOut);
	}
	return faceOut;
}

//calculate the vertex normals
void glcinObject::calcVertexNormals(){
	normals.clear();

	vector<Face> participate;

	for (int i = 0; i < vertex.size(); i++){
		participate.clear();
		
		//looking for each face maked by the vertex with index = i
		for (int j = 0; j < faces.size(); j++){
			for (int k = 0; k < faces[j].vertex.size(); k++){
				if (faces[j].vertex[k]-1 == i){
					participate.push_back(faces[j]);
					k = faces[j].vertex.size();
				}
			}
		}

		//calculate the median of all the normals
		Point3D aux;
		aux.x = 0; aux.y = 0; aux.z = 0;

		for (int j = 0; j < participate.size(); j++){
			aux.x += faceNormals[participate[j].normal - 1].x/participate.size();
			aux.y += faceNormals[participate[j].normal - 1].y/participate.size();
			aux.z += faceNormals[participate[j].normal - 1].z/participate.size();
		}
		normals.push_back(aux);
	}
	
	//saving the normal index of each vertex in each face
	for (int i = 0; i < faces.size(); i++){
		for (int j = 0; j < faces[i].vertex.size(); j++){
			faces[i].normals.push_back(faces[i].vertex[j]);
		}
	}


}

//normalize the object size
void glcinObject::normalizeObject(){
	GLdouble maxX, maxY, maxZ;
	maxX = vertex[0].x; maxY = vertex[0].y; maxZ = vertex[0].z;

	for (int i = 0; i < vertex.size(); i++){
		if (vertex[i].x > maxX)
			maxX = vertex[i].x;
		if (vertex[i].y > maxY)
			maxY = vertex[i].y;
		if (vertex[i].z > maxZ)
			maxZ = vertex[i].z;
	}

	GLdouble len = sqrt(maxX*maxX + maxY*maxY + maxZ*maxZ);
	for (int i = 0; i < vertex.size(); i++){
		vertex[i].x /= len;
		vertex[i].y /= len;
		vertex[i].z /= len;
	}
}

//calculate the vectorial product
Point3D glcinObject::vectorProduct(Point3D vector1, Point3D vector2){
	Point3D out;

	out.x = (vector1.y*vector2.z) - (vector1.z*vector2.y);
	out.y = (vector1.z*vector2.x) - (vector1.x*vector2.z);
	out.z = (vector1.x*vector2.y) - (vector1.y*vector2.x);

	double length = sqrt(out.x*out.x + out.y*out.y + out.z*out.z);
	out.x = out.x / length;
	out.y = out.y / length;
	out.z = out.z / length;

	return out;
}

//triangulate all faces (not concave polygons)
void glcinObject::triangulate_faces(){
	Face aux, temp;

	//after turn one face in N faces, i have to pop_back the original face to push_back the new faces
	//so i put all new faces in this vector
	vector<Face> find;

	//vertices to save data and create new faces
	GLint v1, v2, v3, t1, t2, t3, n1, n2, n3;
	int vertex_size = vertex.size();

	for (int i = 0; i < faces.size(); i++){
		find.clear();

		//if face is not a triangle
		if (faces[i].vertex.size() > 3){
			aux = faces[i];
			v1 = aux.vertex[0];
			
			if (!aux.texture.empty())
				t1 = aux.texture[0];
			
			if (!aux.normals.empty())
				n1 = aux.normals[0];
			
			//each 3 vertex create a new face
			for (int j = 1; j < aux.vertex.size() - 1; j++){
				temp.vertex.clear();
				temp.texture.clear();
				temp.normals.clear();

				v2 = aux.vertex[j];
				v3 = aux.vertex[j + 1];

				temp.vertex.push_back(v1);
				temp.vertex.push_back(v2);
				temp.vertex.push_back(v3);

				if (!aux.texture.empty()){
					t2 = aux.texture[j];
					t3 = aux.texture[j + 1];

					temp.texture.push_back(t1);
					temp.texture.push_back(t2);
					temp.texture.push_back(t3);
				}

				if (!normals.empty()){
					n2 = aux.normals[j];
					n3 = aux.normals[j + 1];

					temp.normals.push_back(n1);
					temp.normals.push_back(n2);
					temp.normals.push_back(n3);
				}

				find.push_back(temp);
			}
			
			//change the old face for one new face and push_back all the other faces created
			faces[i] = find[0];
			for (int k = 1; k < find.size(); k++){
				//Face face = find[k];
				faces.push_back(find[k]);
			}

		}
	}

	if (vertex_size != vertex.size() || normals.empty()){
		calc_face_normals();
		calcVertexNormals();
	}

}

//calculate the face normals (use only with triangulated objects)
void glcinObject::calc_face_normals(){
	faceNormals.clear();
	GLint v1, v2, v3;
	Point3D vector1, vector2, normalVector;

	for (int i = 0; i < faces.size(); i++){
		v1 = faces[i].vertex[0] - 1;
		v2 = faces[i].vertex[1] - 1;
		v3 = faces[i].vertex[2] - 1;

		vector1.x = vertex[v2].x - vertex[v1].x;
		vector1.y = vertex[v2].y - vertex[v1].y;
		vector1.z = vertex[v2].z - vertex[v1].z;

		vector2.x = vertex[v3].x - vertex[v1].x;
		vector2.y = vertex[v3].y - vertex[v1].y;
		vector2.z = vertex[v3].z - vertex[v1].z;

		normalVector = vectorProduct(vector1, vector2);
		faceNormals.push_back(normalVector);
		faces[i].normal = faceNormals.size();
	}
}

//export this obj in a .obj file
void glcinObject::write_object(){
	ofstream obj_file;

	triangulate_faces();

	obj_file.open("Resources/my_obj/" + filename + ".obj");
	
	obj_file << "# Object Loader - Version 1.0 - Created by EHMR  " << endl;
	obj_file << "# Contact: ehmr@cin.ufpe.br                      " << endl;
	obj_file << "# About me: www.cin.ufpe.br/~ehmr                " << endl;
	obj_file << "# Last Updated: 8/5/2015   13:59h                " << endl;
	obj_file << "# Num verts: " << vertex.size()					<< endl;
	obj_file << "# Num faces: " << faces.size()						<< endl << endl;

	for (int i = 0; i < vertex.size(); i++){
		obj_file << "v " << vertex[i].x << " " << vertex[i].y << " " << vertex[i].z << endl;
	}

	for (int i = 0; i < texture.size(); i++){
		obj_file << "vt " << texture[i].x << " " << texture[i].y << endl;
	}

	for (int i = 0; i < normals.size(); i++){
		obj_file << "vn " << normals[i].x << " " << normals[i].y << " " << normals[i].z << endl;
	}

	for (int i = 0; i < faces.size(); i++){
		obj_file << "f ";
		for (int j = 0; j < faces[i].vertex.size(); j++){
			obj_file << faces[i].vertex[j] << "/";
			if (faces[i].texture.size() > 0)
				obj_file << faces[i].texture[j];
			obj_file << "/" << faces[i].normals[j] << " ";
		}
		obj_file << endl;
	}

	obj_file.close();
	cout << filename << ".obj created" << endl;
}

/*
void glcinObject::subdivision_surface(){
	vector<Face> new_faces;
	vector<Point3D> new_vertex;
	double aux;
	Face temp;
	Point3D out;

	for (int i = 0; i < 1; i++){
		temp.vertex.clear();
		for (GLdouble t = 0; t <= 1; t += t_value){
			out.x = 0; out.y = 0; out.z = 0;
			for (int j = 0; j <= 2; j++){
				aux = combin[j] * pow((1 - t), (2 - j))*pow(t, j);
				out.x += aux*vertex[faces[i].vertex[j]].x;
				out.y += aux*vertex[faces[i].vertex[j]].y;
				out.z += aux*vertex[faces[i].vertex[j]].z;
			}
			new_vertex.push_back(out);
			temp.vertex.push_back(new_vertex.size());
		}
		new_faces.push_back(temp);
	}
	
	//cout << vertex.size() << endl;
	//
	//faces = new_faces;
	//vertex = new_vertex;
	//
	//cout << vertex.size() << endl;
	faces[0] = new_faces[0];
	for (int i = 1; i < new_faces.size(); i++){
		faces.push_back(new_faces[i]);
	}
}

GLint glcinObject::binomial(GLint n, GLint k){
	if (k > n) {
		return 0;
	}
	unsigned long long r = 1;
	for (unsigned long long d = 1; d <= k; d++) {
		r *= n--;
		r /= d;
	}
	return r;
}
*/

//get the limits of the object to generate a bound box
void glcinObject::get_limits(){
	double out[6];

	double min_x = 9999999, max_x = -9999999, min_y = 9999999, max_y = -9999999, min_z = 9999999, max_z = -9999999;
	Point3D aux, temp;
	
	for (int i = 0; i < vertex.size(); i++){
		aux.x = objectScale[0][0] * vertex[i].x + objectScale[0][1] * vertex[i].y + objectScale[0][2] * vertex[i].z + objectScale[0][3];
		aux.y = objectScale[1][0] * vertex[i].x + objectScale[1][1] * vertex[i].y + objectScale[1][2] * vertex[i].z + objectScale[1][3];
		aux.z = objectScale[2][0] * vertex[i].x + objectScale[2][1] * vertex[i].y + objectScale[2][2] * vertex[i].z + objectScale[2][3];

		temp.x = objectOperations[0][0] * aux.x + objectOperations[0][1] * aux.y + objectOperations[0][2] * aux.z + objectOperations[0][3];
		temp.y = objectOperations[1][0] * aux.x + objectOperations[1][1] * aux.y + objectOperations[1][2] * aux.z + objectOperations[1][3];
		temp.z = objectOperations[2][0] * aux.x + objectOperations[2][1] * aux.y + objectOperations[2][2] * aux.z + objectOperations[2][3];

		if (temp.x < min_x)
			min_x = temp.x;
		if (temp.y < min_y)
			min_y = temp.y;
		if (temp.z < min_z)
			min_z = temp.z;

		if (temp.x > max_x)
			max_x = temp.x;
		if (temp.y > max_y)
			max_y = temp.y;
		if (temp.z > max_z)
			max_z = temp.z;
	}

	limits[0] = min_x;
	limits[1] = max_x;
	limits[2] = min_y;
	limits[3] = max_y;
	limits[4] = min_z;
	limits[5] = max_z;

	for (int i = 0; i < 6; i+=2)
		cout << filename << ": " << endl << limits[i] << " - " << limits[i + 1] << endl;
}