//headers
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <stdio.h>
#include <vector>
#include <string.h>
#include <fstream>
#include <iostream>
#include <SOIL.h>

#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream> 

#include "MathHelper.h"		// NEW!
#include "cube.h"
#include "shader.h"

#define ARRAY_COUNT( array ) (sizeof( array ) / (sizeof( array[0] ) * (sizeof( array ) != sizeof(void*) || sizeof( array[0] ) <= sizeof(void*))))

// From http://www.opengl.org/registry/specs/EXT/pixel_buffer_object.txt 
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

GLuint shaderProgramID;
GLuint vao = 0;
GLuint vbo;
GLuint positionID, colorID;
GLuint indexBufferID; 

GLuint	perspectiveMatrixID, viewMatrixID, modelMatrixID;	// IDs of variables mP, mV and mM in the shader

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

float horizontal = -0.35f;
float vertical = -0.75f;
float thetaY = 0.0f;
float thetaX = 0.0f;
float scaleAmount = 0.5f;
float puckLR = 0.5f;
float puckFB = 2.0f;
float speed = 0.01f;
float depth = -2.0f;

int score=0;
int highScore=0;
int start = 0;

void initMatrices() {

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

void checkGoal(){

	if(puckFB<-2.0f){

		if(puckLR < -0.25 || puckLR > (0.25-0.1)){
			
			speed = 0.01f;
			if(score>highScore)
				highScore = score;
			score = 0;

			printf("\nNO GOAL!\n\nGame over.....\n\n");
			printf("Your high score is: %d\n", highScore);
			
			printf("\nRestarting the game... Good luck!\n");

		}
		else{
			printf("\nGOAL!! Time to speed up the puck!!\n");
			speed += 0.005f;
			score++;
		
		}

		//reset everything
		puckLR = -1.0 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(2.0)));
		puckFB = 2.0f;
		depth = -2.0f;
		start = 0;

	}

}


void AdjustVertexData(float x, float z)
{
    std::vector<float> fNewData(ARRAY_COUNT(vertices));
    memcpy(&fNewData[0], vertices, sizeof(vertices));
   
    for(int i = 24; i < 48; i += 3)
    {
    	fNewData[i] += x;
    	fNewData[i + 2] += z;
            
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), &fNewData[0]);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}


// Here is the function that gets called each time the window needs to be redrawn.
// It is the "paint" method for our program, and is set up from the glutDisplayFunc in main
void render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	if(start){
		depth  += speed/2.0;
		puckFB -= speed;		
		checkGoal();
	}

	AdjustVertexData(puckLR, puckFB);

	glUseProgram(shaderProgramID);

	// Fill the matrices with valid data
	MathHelper::makeScale(scaleMatrix, scaleAmount, scaleAmount, scaleAmount);	
	MathHelper::makeRotateY(rotYMatrix, thetaY);						
	MathHelper::makeRotateX(rotXMatrix, thetaX);						
	MathHelper::makeTranslate(transMatrix, horizontal, vertical, depth);

	/*multiply the matricies*/
	MathHelper::matrixMult4x4(tempMatrix1, rotXMatrix, scaleMatrix);	
	MathHelper::matrixMult4x4(tempMatrix2, rotYMatrix, tempMatrix1);	
	MathHelper::matrixMult4x4(M, transMatrix, tempMatrix2);
		
	glUniformMatrix4fv(modelMatrixID, 1, GL_TRUE, M);
	glUniformMatrix4fv(viewMatrixID, 1, GL_TRUE, V);
	glUniformMatrix4fv(perspectiveMatrixID, 1, GL_TRUE, P);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_QUADS);
	glDrawElements (GL_TRIANGLES, ARRAY_COUNT( indices ), GL_UNSIGNED_INT, NULL);
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
	case 'w':
		vertical += 0.1;
		break;
	case 's':
		vertical -= 0.1;
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
	case 'z':
		depth += 0.1;
		break;
	case 'q':
		scaleAmount += 0.1;
		break;
	case 033:
	    exit( EXIT_SUCCESS );
	    break;
    }

}


void SpecialInput(int key, int x, int y)
{
	switch(key){
		case GLUT_KEY_UP:
			start = 1;
			break;
		case GLUT_KEY_LEFT:
			if(start)
				puckLR -= 0.05;
			break;
		case GLUT_KEY_RIGHT:
			if(start)
				puckLR += 0.05;
			break;
	}
	
	glutPostRedisplay();

}

void printWelcome(){

	printf("Welcome to hot shot!\n");
	printf("You want to get as many goals as possible\n");
	printf("Each time you get a goal, the shots get faster!\n");
	printf("Your score is the number of goals you get\n");
	printf("Good luck!!\n");
	printf("Starting in 3.... 2.... 1....\n");
	sleep(1);

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

	// Make a shader
	shaderProgramID = createShaders();

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	positionID = glGetAttribLocation(shaderProgramID, "s_vPosition");
	colorID = glGetAttribLocation(shaderProgramID, "s_vColor");
	
	perspectiveMatrixID = glGetUniformLocation(shaderProgramID, "mP");
	viewMatrixID = glGetUniformLocation(shaderProgramID, "mV");
	modelMatrixID = glGetUniformLocation(shaderProgramID, "mM");

	glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(colorID, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertices)));
	
	glutKeyboardFunc(keyboard);

	glUseProgram(shaderProgramID);
	glEnableVertexAttribArray(positionID);
	glEnableVertexAttribArray(colorID);
	glutSpecialFunc(SpecialInput);


	printWelcome();

	glutMainLoop();
	
	return 0;
}

