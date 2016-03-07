/**********************************************************
	Author: Victor Gonzalez
	Usuage: Basic template for OpenGL programs. Used 
			to render both 3d and 2d objects.  Contains 
			the necessary function to get started in 
			writing an OpenGL program

	*NOTES:
		By default, OpenGL uses a Cartesian coordinate
		system. Both the x-axis lie between -1 and 1 with
		the origin lying in the middle of the screen at 
		0,0

		CARTESIAN PLANE
						   (1)
							|
							|
							|
							|
			(-1) -------------------------- (1)
							|
							|
							|
							|
						  (-1)

************************************************************/
#include<glew.h>
#include<freeglut.h>
#include<iostream>
#include<stdlib.h>
#include "Player.h"
#include<stack>
#include<ctime>
#include<cstdlib>
#include "shader.h"
#include "Enemy.h"

//Global variables

//Objects
Player player;
Enemy enemy;
Shader *shader = 0;		//Use singleton pattern to set shader

//Prototypes
void render();
void key(unsigned char, int, int);
void mouse(int , int , int , int );
void playerMove(int, int, int);
void timer(int);


int main(int argc, char** argv) {
	//Create window
	glutInit(&argc, argv);											//Initialize glut
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);		//Specify the modes to render your window
	glutInitWindowSize(640, 480);									//Set the window size 
	glutInitWindowPosition(100,100);								//Set the position of the window on the screen
	glutCreateWindow("Graphics Project");							//Creates the window and sets the title
	//Vertex data for a triagle
	//Initialize glew
	glewExperimental = GL_TRUE;										//Needed to make glew work
	glewInit();														//Initialize glew for communication with the gpu
											//Generate the maze using depth first search
	//Initialize the player
	player.init();
	enemy.init();

	//Call back functions
	glutDisplayFunc(render);										//Draws whatever is passed in to the screen
	glutKeyboardFunc(key);											//Executes event based on the key pressed
	glutMouseFunc(mouse);											//Executes event based on mouse button pressed
	glutSpecialFunc(playerMove);									//Execute event and redraws when a special key is pressed
	glutMainLoop();													//Main loop of the glut program. Keeps window open

	return 0;
}

/**********************************************
	The render function is used to draw objects
	to the screen.
************************************************/
void render() {
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);				//Clear the screen of previous drawn data
	
	player.Draw();
	enemy.Draw();
	glutSwapBuffers();												//Swaps the buffers when double buffer is used
}


/*********************************************
	The key function takes in a key and runs
	the event associated with it.
**********************************************/
void key(unsigned char key, int x, int y) {
	switch(key) {
	case 27:			//27 represents the escape key
		exit(0);		//Exit the progam when escape is pressed
		break;
	}
}

/*************************************************
	The playerMove function is used to handle
	key presses that are associated with movement
	and objects that need to be redrawn to the 
	screen.
**************************************************/
void playerMove(int key, int x, int y) {
	//Logic to handle special key presses
	switch(key) {
	case GLUT_KEY_LEFT:
		player.translate(player.getX()-0.05f, player.getY());
		glutPostRedisplay();
		break;
	case GLUT_KEY_RIGHT:
		player.translate(player.getX()+0.05f, player.getY());
		glutPostRedisplay();
		break;
	case GLUT_KEY_UP:
		player.translate(player.getX(), player.getY()+0.05f);
		glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN:
		player.translate(player.getX(), player.getY()-0.05f);
		glutPostRedisplay();
		break;
	}
}

/************************************************
	The mouse function takes in a button and runs
	the event associated with it.
**************************************************/
void mouse(int button, int state, int x, int y) {
	switch(button) {
	case GLUT_RIGHT_BUTTON:
		exit(0);
		break;
	}
}

