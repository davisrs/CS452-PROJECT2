#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include "MathHelper.h"		// NEW!
#include "cube.h"
#include "shader.h"



#define NUM_VERTICES num_vertices
#define NUM_INDICES num_indices	

// From http://www.opengl.org/registry/specs/EXT/pixel_buffer_object.txt 
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

GLuint shaderProgramRink, shaderProgramPuck;
GLuint vao[2];
GLuint vaoPuck;
GLuint vaoRink;
GLuint vbo;

GLuint vboPuck;


GLuint positionID, colorID, indexBufferID; 
GLuint positionID2, colorID2, indexBufferID2; 

GLuint	perspectiveMatrixID, viewMatrixID, modelMatrixID;	// IDs of variables mP, mV and mM in the shader
GLuint	perspectiveMatrixID2, viewMatrixID2, modelMatrixID2;

GLfloat* rotXMatrix;	
GLfloat* rotYMatrix;	
GLfloat* rotZMatrix;	
GLfloat* transMatrix;	
GLfloat* scaleMatrix;	
GLfloat* tempMatrix1;	
GLfloat* tempMatrix2;	
GLfloat* M;				

GLfloat* V;				
GLfloat* P;			

GLfloat* transMatrixPuck;



float horizontal = 0.65f;
float vertical = -1.0f;
float thetaY = 1.60f;
float thetaX = 0.00f;
float scaleAmount = 1.0f;

void initMatrices() {

	scaleAmount = 0.5f;
	// Allocate memory for the matrices and initialize them to the Identity matrix
	rotXMatrix = new GLfloat[16];	MathHelper::makeIdentity(rotXMatrix);
	rotYMatrix = new GLfloat[16];	MathHelper::makeIdentity(rotYMatrix);
	rotZMatrix = new GLfloat[16];	MathHelper::makeIdentity(rotZMatrix);
	transMatrix = new GLfloat[16];	MathHelper::makeIdentity(transMatrix);
	scaleMatrix = new GLfloat[16];	MathHelper::makeIdentity(scaleMatrix);
	tempMatrix1 = new GLfloat[16];	MathHelper::makeIdentity(tempMatrix1);
	tempMatrix2 = new GLfloat[16];	MathHelper::makeIdentity(tempMatrix2);

	M = new GLfloat[16];			MathHelper::makeIdentity(M);
	V = new GLfloat[16];			MathHelper::makeIdentity(V);
	P = new GLfloat[16];			MathHelper::makeIdentity(P);

	MathHelper::makePerspectiveMatrix(P, 60.0f, 1.0f, 1.0f, 1000.0f);
}

void changeViewport(int w, int h){
	glViewport(0, 0, w, h);
}

// Here is the function that gets called each time the window needs to be redrawn.
// It is the "paint" method for our program, and is set up from the glutDisplayFunc in main
void render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	//glUseProgram(shaderProgramRink);
	glBindVertexArray(vaoRink);

	// Fill the matrices with valid data
	
	MathHelper::makeScale(scaleMatrix, scaleAmount, scaleAmount, scaleAmount);	
	MathHelper::makeRotateY(rotYMatrix, thetaY);						
	MathHelper::makeRotateX(rotXMatrix, thetaX);						
	MathHelper::makeTranslate(transMatrix, horizontal, vertical, -2.0f);

	
	MathHelper::matrixMult4x4(tempMatrix1, rotXMatrix, scaleMatrix);	
	MathHelper::matrixMult4x4(tempMatrix2, rotYMatrix, tempMatrix1);	
	MathHelper::matrixMult4x4(M, transMatrix, tempMatrix2);
		
	glUniformMatrix4fv(modelMatrixID, 1, GL_TRUE, M);
	glUniformMatrix4fv(viewMatrixID, 1, GL_TRUE, V);
	glUniformMatrix4fv(perspectiveMatrixID, 1, GL_TRUE, P);
	
	

	glPolygonMode(GL_FRONT_AND_BACK, GL_QUADS);
	glDrawElements (GL_TRIANGLES, NUM_INDICES, GL_UNSIGNED_INT, NULL);

	//glBindVertexArray(0);

	
/*
	MathHelper::makeScale(scaleMatrix, scaleAmount, scaleAmount, scaleAmount);	
	MathHelper::makeRotateY(rotYMatrix, thetaY);						
	MathHelper::makeRotateX(rotXMatrix, thetaX);						
	MathHelper::makeTranslate(transMatrix, horizontal, vertical, -2.0f);

	
	MathHelper::matrixMult4x4(tempMatrix1, rotXMatrix, scaleMatrix);	
	MathHelper::matrixMult4x4(tempMatrix2, rotYMatrix, tempMatrix1);	
	MathHelper::matrixMult4x4(M, transMatrix, tempMatrix2);
		
	glUniformMatrix4fv(modelMatrixID, 1, GL_TRUE, M);
	glUniformMatrix4fv(viewMatrixID, 1, GL_TRUE, V);
	glUniformMatrix4fv(perspectiveMatrixID, 1, GL_TRUE, P);
	
	*/


	//glUseProgram(shaderProgramPuck);
	//glBindVertexArray(vaoPuck);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_QUADS);
	//glDrawElements (GL_TRIANGLES, NUM_INDICES, GL_UNSIGNED_INT, NULL);



	glutSwapBuffers();
	glutPostRedisplay();
}


void keyboard( unsigned char key, int x, int y ){
    /*update the variables based on key press*/
    switch( key ) {
	case 'a':
		horizontal -= 0.1;
		break;
	case 'd':
		horizontal += 0.1;
		break;
	case 'k':
		thetaY += 0.1;
		break;
	case 'l':
		thetaY -= 0.1;
		break;
	case 'e':
		scaleAmount -= 0.1;
		if(scaleAmount<0)
			scaleAmount=0;
		break;
	case 'q':
		scaleAmount += 0.1;
		break;
	case 033:
	    exit( EXIT_SUCCESS );
	    break;
    }
}

void initRink(){
	

	shaderProgramRink = createShaderRink();
	glUseProgram(shaderProgramRink);
	
	glGenVertexArrays(1, &vaoRink);
	glBindVertexArray(vaoRink);
	
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// Create the buffer, but don't load anything yet
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors)*sizeof(vertices), NULL, GL_STATIC_DRAW);
	// Load the vertex points
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
	// Load the colors right after that
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices),sizeof(colors), colors);

	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, NUM_INDICES*sizeof(GLuint), indices, GL_STATIC_DRAW);
	
	positionID = glGetAttribLocation(shaderProgramRink, "s_vPosition");
	colorID = glGetAttribLocation(shaderProgramRink, "s_vColor");
	
	perspectiveMatrixID = glGetUniformLocation(shaderProgramRink, "mP");
	viewMatrixID = glGetUniformLocation(shaderProgramRink, "mV");
	modelMatrixID = glGetUniformLocation(shaderProgramRink, "mM");

	glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertices)));

}


void initPuck(){

	shaderProgramPuck = createShaderPuck();
	glUseProgram(shaderProgramPuck);
	glGenVertexArrays(1, &vaoPuck);
	glBindVertexArray(vaoPuck);
	
	glGenBuffers(1, &vboPuck);
	glBindBuffer(GL_ARRAY_BUFFER, vboPuck);

	// Create the buffer, but don't load anything yet
	glBufferData(GL_ARRAY_BUFFER, sizeof(puckColors)*sizeof(puckVertices), NULL, GL_STATIC_DRAW);
	// Load the vertex points
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(puckVertices), puckVertices); 
	// Load the colors right after that
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(puckVertices),sizeof(puckColors), puckColors);

	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, NUM_INDICES*sizeof(GLuint), indices, GL_STATIC_DRAW);
	
	positionID2 = glGetAttribLocation(shaderProgramPuck, "s_vPosition");
	colorID2 = glGetAttribLocation(shaderProgramPuck, "s_vColor");
	
	perspectiveMatrixID2 = glGetUniformLocation(shaderProgramPuck, "mP");
	viewMatrixID2 = glGetUniformLocation(shaderProgramPuck, "mV");
	modelMatrixID2 = glGetUniformLocation(shaderProgramPuck, "mM");

	glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(puckVertices)));

}

int main (int argc, char** argv) {
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Index Buffers");
	glutReshapeFunc(changeViewport);
	glutDisplayFunc(render);
	glewInit();

	/*set up the matricies*/
	initMatrices(); 

	
	initRink();


	//initPuck();
	

	glutKeyboardFunc(keyboard);

	glUseProgram(shaderProgramRink);
	glEnableVertexAttribArray(positionID);
	glEnableVertexAttribArray(colorID);
	
	glutMainLoop();
	
	return 0;
}

