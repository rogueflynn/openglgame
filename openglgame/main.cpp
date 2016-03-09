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
#include<math.h>
#include "Box.h"
#include "Key.h"
#include<vector>
#include "Camera.h"
#include "Sprite.h"
#include "Tiles.h"

//Global variables
int ms = (1/60) * 1000;  //Calculates 60fps (1 sec/60fps) * (1000ms / 1 sec)
bool alive = true;

//Objects
std::vector<Tiles*> tiles;
std::vector<Enemy*> enemies;		//Player
std::vector<Player*> player;		//Player vector
Shader *shader = 0;					//Use singleton pattern to set shader
Box box;							//Used to check collision detection
Key keyPress;

//Prototypes
void render();
void key(unsigned char, int, int);
void mouse(int , int , int , int );
void playerMove();
void update(int);
void enemyCollision();

int main(int argc, char** argv) {
	for(int i=0; i < 10; i++)
		tiles.push_back(new Tiles());
	for(int i=0; i < 4; i++)
		enemies.push_back(new Enemy());
	player.push_back(new Player());

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

	//Initialize the player
	player[0]->init();
	for(unsigned int i = 0; i < enemies.size(); i++) {
		enemies[i]->init();
		enemies[i]->setColor(1.0f, 0.0f, 1.0f);
	}
	enemies[0]->translate(-0.8f, 0.8f);
	enemies[1]->translate(0.8f, 0.8f);
	enemies[3]->translate(0.8f, -0.8f);
	enemies[2]->translate(-0.8f, -0.8f);

	for(unsigned int i = 0; i < tiles.size(); i++)
		tiles[i]->init();
	tiles[1]->translate(0.0f, 0.5f);
	tiles[2]->translate(0.0f, -0.5f);
	tiles[3]->translate(-0.5f, 0.0f);
	tiles[4]->translate(-0.5f, -0.5f);
	tiles[5]->translate(-0.5f, 0.5f);
	tiles[6]->translate(0.5f, 0.5f);
	tiles[7]->translate(0.5f, -0.5f);
	tiles[8]->translate(0.5f, -0.8f);
	//Call back functions
	glutDisplayFunc(render);										//Draws whatever is passed in to the screen
	glutKeyboardFunc(key);											//Executes event based on the key pressed
	glutMouseFunc(mouse);											//Executes event based on mouse button pressed
	glutMainLoop();													//Main loop of the glut program. Keeps window open
	if(!player.empty()) 
		player[0]->cleanUp();

	for(unsigned int i = 0; i < tiles.size(); i++)
		tiles[i]->cleanUp();
	return 0;
}


/***********************************************
			update() 
	All update logic goes in this function.
	update will keep track of the status and
	location of the player and tiles.
*************************************************/
void update(int data) {
	player[0]->setColor(1.0f, 0.0f,0.0f);
	enemyCollision();					//Check if the player has collided with an enemy
	playerMove();						//Move the player
	glutPostRedisplay();				//Refresh the screen
}

/**********************************************
			Render()			
	The render function is used to draw objects
	to the screen.
************************************************/
void render() {
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);				//Clear the screen of previous drawn data
	if(!alive) {
		player[0]->cleanUp();
		player.pop_back();
		alive = true;
	}
	if(!player.empty()) {
		glutTimerFunc(ms, update, 1);									//Controls updating
		player[0]->Draw();
	}

	//Draw the enemies 
	for(unsigned int i = 0; i < enemies.size(); i++){
		enemies[i]->Draw();	
	}

	//Draw tiles
	for(unsigned int i = 0; i < tiles.size(); i++){
		tiles[i]->Draw();	
	}

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
				playerMove()
	The playerMove function is used to handle
	key presses that are associated with movement
	and objects that need to be redrawn to the 
	screen. Used GetAsyncKeyState(VK_KEY) to avoid
	the delay of the glutSpecial keyfunction.
**************************************************/
void playerMove() {
	//Logic to handle special key presses
	for(unsigned int i = 0; i < tiles.size(); i++){
		if(keyPress.Left()) {
			if(!(box.touchRight(*player[0],*tiles[i]))) {	
				player[0]->moveLeft();
			} else  {
				std::cout << "Collided with right\n";
				player[0]->stopMovingLeft(tiles[i]->Right());
			}
		} else if(keyPress.Right()) {
			if(!(box.touchLeft(*player[0],*tiles[i]))) {
				player[0]->moveRight();
			} else {
				std::cout << "Collided with left\n";
				player[0]->stopMovingRight(tiles[i]->Left());
			}
		} else if(keyPress.Up()) {
			if(!(box.touchBottom(*player[0], *tiles[i]))) {
				player[0]->moveUp();
			} else {
				std::cout << "Collided with bottom\n";
				player[0]->stopMovingUp(tiles[i]->Bottom());
			}
		} else if(keyPress.Down()) {
			if(!(box.touchTop(*player[0], *tiles[i]))) {
				player[0]->moveDown();
			} else {
				std::cout << "Collided with top\n";
				player[0]->stopMovingDown(tiles[i]->Top());
			}
		}
	}
}

/*******************************************************************
				enemyCollsion()
		Determines whether or not a player has collided with
		an enemy.
********************************************************************/
void enemyCollision() {
	int size = enemies.size();
	for(unsigned int i = 0; i < enemies.size(); i++){
		if((box.intersect(*player[0], *enemies[i]))){
			player[0]->setColor(0.0f, 1.0f, 0.0f);	
			//enemies.erase(enemies.begin() + i);
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

