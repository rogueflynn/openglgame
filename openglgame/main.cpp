/**********************************************************
	Author: Victor Gonzalez
	Purpose:
		Main entry point for the game engine. 

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
#include<Windows.h>
#include<math.h>

//Global variables
int ms = (1/60) * 1000;  //Calculates 60fps (1 sec/60fps) * (1000ms / 1 sec)
float hd, vd;
//Objects
Player player;
Enemy enemy;
Shader *shader = 0;		//Use singleton pattern to set shader

//Prototypes
void render();
void key(unsigned char, int, int);
void mouse(int , int , int , int );
void playerMove();
void update(int);
bool checkCollision();
bool touchTop();
bool touchLeft();
bool touchRight();
bool touchBottom();

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
	glutMainLoop();													//Main loop of the glut program. Keeps window open
	player.cleanUp();
	enemy.cleanUp();
	return 0;
}


/***********************************************
			update() 
	All update logic goes in this function.
	update will keep track of the status and
	location of the player and enemies.
*************************************************/
void update(int data) {
	//Check for collision
	if(!checkCollision()) {
		player.setColor(1.0f, 0.0f, 0.0f);
		glutPostRedisplay();
	} else if(checkCollision()) {
		player.setColor(0.0f, 1.0f, 0.0f);
		glutPostRedisplay();
	}

	//Moves the player on the screen
	playerMove();
}

/**********************************************
			Render()			
	The render function is used to draw objects
	to the screen.
************************************************/
void render() {
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);				//Clear the screen of previous drawn data
	glutTimerFunc(ms, update, 1);									//Controls updating
	player.Draw();
	enemy.Draw();
	glutSwapBuffers();												//Swaps the buffers when double buffer is used
}


/****************************************************
		checkCollision()
	Uses the axis-aligned algorithm in order to 
	perform collision detection. 
*****************************************************/
bool checkCollision() {
	return !((player.getX() + player.getSize()/20)<= enemy.getX() - enemy.getSize()/20 ||
			 player.getX() - player.getSize()/20 >= enemy.getX() + enemy.getSize()/20 ||
			 player.getY() + player.getSize()/20  <= enemy.getY() - enemy.getSize()/20 ||
			 player.getY() - player.getSize()/20  >= enemy.getY() + enemy.getSize()/20); 
}

//Rectangle helper functions
//Used to determine if the character is colliding with the platform
bool touchTop() {
	return (checkCollision() && player.getY() > enemy.getY() && vd > hd);
}
bool touchBottom() {
	return (checkCollision() && player.getY() < enemy.getY() && vd > hd);
}
bool touchLeft() {
	return (checkCollision() && player.getX() < enemy.getX() && hd > vd);
}
bool touchRight() {
	return (checkCollision() && player.getX() > enemy.getX() && hd > vd);
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
				playerMove()
	The playerMove function is used to handle
	key presses that are associated with movement
	and objects that need to be redrawn to the 
	screen. Used GetAsyncKeyState(VK_KEY) to avoid
	the delay of the glutSpecial keyfunction.
**************************************************/
void playerMove() {
	//Logic to handle special key presses
	//Distance between x values |x1 - x2|
	//Distance between y values |y1 - y2|
	hd = abs(player.getX() - enemy.getX());			//Gets the horizontal distance
	vd = abs(player.getY() - enemy.getY());			//Gets the vertical distance

	if(GetAsyncKeyState(VK_LEFT) & 0x8000) {
		if(!(touchRight())) {	
			player.translate(player.getX()-0.00015f, player.getY());
			glutPostRedisplay();
		}
	} else if(GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		if(!(touchLeft())) {
			player.translate(player.getX()+0.00015f, player.getY());
			glutPostRedisplay();
		}
	} else if(GetAsyncKeyState(VK_UP) & 0x8000) {
		if(!(touchBottom())) {
			player.translate(player.getX(), player.getY()+0.00015f);
			glutPostRedisplay();
		}
	} else if(GetAsyncKeyState(VK_DOWN) & 0x8000) {
		if(!(touchTop())) {
			player.translate(player.getX(), player.getY()-0.00015f);
			glutPostRedisplay();
		} 
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

