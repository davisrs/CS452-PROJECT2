/* OBJ2H program, copyright Jeff Chastine */
/* Original file: cube.obj */
/* Version 1.0*/


#ifndef CUBE_H
#define CUBE_H

const int num_indices = 90;
const int num_vertices = 24;
const int num_normals = 8;


GLfloat vertices[] = {
	-2.0f, 0.00000f, 0.00000f, 
	-2.0f, 0.00000f, 2.00000f, 
	-2.0f, 0.25f, 0.00000f, 
	-2.0f, 0.25f, 2.00000f, 
	2.00000f, 0.00000f, 0.00000f, 
	2.00000f, 0.00000f, 2.00000f, 
	2.00000f, 0.25f, 0.00000f, 
	2.00000f, 0.25f, 2.00000f

};

GLfloat colors[] = {
	1.0f, 1.0f, 1.0f, 1.0f, 
	1.0f, 1.0f, 1.0f, 1.0f, 
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f

};

GLuint indices[] = {
	0,	6,	4,
	0,	2,	6,
	0,	3,	2,
	0,	1,	3,
	2,	7,	6,
	2,	3,	7,
	4,	6,	7,
	4,	7,	5,
	0,	4,	5,
	0,	5,	1,
	1,	5,	7,
	1,	7, 3

};



GLfloat puckVertices[] = {
	-1.0f, 0.25f, 0.00000f, 
	-1.0f, 0.25f, 2.00000f, 
	-1.0f, 0.5f, 0.00000f, 
	-1.0f, 0.5f, 2.00000f, 
	1.00000f, 0.25f, 0.00000f, 
	1.00000f, 0.25f, 2.00000f, 
	1.00000f, 0.5f, 0.00000f, 
	1.00000f, 0.5f, 2.00000f

};

GLfloat puckColors[] = {
	1.0f, 0.0f, 0.0f, 1.0f, 
	1.0f, 0.0f, 0.0f, 1.0f, 
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f

};

GLuint puckIndices[] = {
	0,	6,	4,
	0,	2,	6,
	0,	3,	2,
	0,	1,	3,
	2,	7,	6,
	2,	3,	7,
	4,	6,	7,
	4,	7,	5,
	0,	4,	5,
	0,	5,	1,
	1,	5,	7,
	1,	7, 3

};



#endif //cube_H
