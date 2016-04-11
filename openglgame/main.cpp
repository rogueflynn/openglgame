/**********************************************************
	Author: Victor Gonzalez
	Purpose:
		Main entry point for the game engine. 

************************************************************/
#include<gl/glew.h>
#include<gl/freeglut.h>
#include<iostream>
#include<stdlib.h>
#include "Player.h"
#include "shader.h"
#include "Enemy.h"
#include<math.h>
#include "Box.h"
#include "Key.h"
#include<vector>
#include "Camera.h"
#include "Sprite.h"
#include "Model_OBJ.h"
#include<ctime>
#include<cstdlib>
#include"Background.h"

/************************************************************************
  Window
 ************************************************************************/
 
typedef struct {
    int width;
	int height;
	char* title;
 
	float field_of_view_angle;
	float z_near;
	float z_far;
} glutWindow;

//Global variables
int ms = (1/60) * 1000;  //Calculates 60fps (1 sec/60fps) * (1000ms / 1 sec)
bool alive = true;
glutWindow win;
float deltaTime = 1.0f;
float currentTime, previousTime;
int randX = 0, randY;			//Used to randomize the ships falling 
float leftWall = -6.0f;
float righttWall = 6.0f;
float topWall = 3.0f;
float bottomWall = -4.0f;
float enemyCount = 0;

//Vectors
std::vector<Enemy*> enemies;		//Enemy
std::vector<Player*> player;		//Player vector
std::vector<Model_OBJ> enemyModelObj;

//Objects
Box box;							//Used to check collision detection
Key keyPress;
Model_OBJ enemyModel;
Background background;


//Prototypes
void render();
void key(unsigned char, int, int);
void mouse(int , int , int , int );
void playerMove(float &);
void update(int data);
void enemyCollision();
void initialize();
void deleteEnemies(int &index) {
		delete enemies[index];
		enemies.erase(enemies.begin() + index);
}

void cleanUp() {
	int index = -1;
	for(unsigned int i = 0; i < enemies.size(); i++) {
		index = i;
		deleteEnemies(index);
	}
	enemyCount = 0;
}

//Loads all of the enemies into the game world
void loadEnemies() {
	if(randX >= 0)
		randX = rand() % 5 - 5;    //Between -6 and 0
	else if(randX < 0)
		randX = rand() % 5 + 1;	   //Between 6 and 1
	randY = rand() % 5 + 7;
	Enemy *enemy = new Enemy();
	//cout << randX << " " << randY << "\n";
	enemy->init();
	enemy->translate((float) randX, (float) randY);
	enemy->setColor(1.0f, 0.0f, 1.0f);
	enemies.push_back(enemy);

	enemyCount++;
}

//Controls how the enemies spaw on the screen
void enemySpawn() { 
	int index = -1;
	for(unsigned int i = 0; i < enemies.size(); i++) {
		enemies[i]->translate(enemies[i]->getX(), enemies[i]->getY() - 0.005f * deltaTime);
		if(enemies[i]->getY() < -7.5f) {
			index = i;
			deleteEnemies(index);
		}
	}
}

int main(int argc, char** argv) {
	srand(time(NULL));
	// set window values
	win.width = 1024;
	win.height = 600;
	win.title = "Graphics Project";
	win.field_of_view_angle = 90;
	win.z_near = 1.0f;
	win.z_far = 500.0f;

	enemyModel.Load("eShip3.obj");

	player.push_back(new Player());

	currentTime = (float) glutGet(GLUT_ELAPSED_TIME);

	//Create window
	glutInit(&argc, argv);											//Initialize glut
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);		//Specify the modes to render your window
	glutInitWindowSize(win.width, win.height);						//Set the window size 
	glutInitWindowPosition(100,100);								//Set the position of the window on the screen
	glutCreateWindow(win.title);									//Creates the window and sets the title

	//Initialize glew
	glewExperimental = GL_TRUE;										//Needed to make glew work
	glewInit();														//Initialize glew for communication with the gpu

	if(!(background.LoadGLTextures()))
		return 1;

	//Initialize the player
	player[0]->init();

	//Call back functions
	glutDisplayFunc(render);										//Draws whatever is passed in to the screen
	glutKeyboardFunc(key);											//Executes event based on the key pressed
	glutMouseFunc(mouse);											//Executes event based on mouse button pressed
	initialize();
	glutMainLoop();													//Main loop of the glut program. Keeps window open

	return 0;
}


/***********************************************
			update() 
	All update logic goes in this function.
	update will keep track of the status and
	location of the player and tiles.
*************************************************/
void update(int data) {
		background.Scroll();
		//Calculates the frame rate
		previousTime = currentTime;
		currentTime = (float) glutGet(GLUT_ELAPSED_TIME);
		deltaTime = (currentTime - previousTime);

		enemyCollision();							//Check if the player has collided with an enemy
		playerMove(deltaTime);						//Move the player
		if(enemies.size() < 7 && enemyCount != 48)
			loadEnemies();

		if(enemies.size() == 1 && enemyCount == 48)
			cleanUp();

		//cout << "X: " <<  player[0]->getX() << " Y:" << player[0]->getY() << "\n";		

		glutPostRedisplay();
}

/**********************************************
			Render()			
	The render function is used to draw objects
	to the screen.
************************************************/
void render() {
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);				//Clear the screen of previous drawn data
	glutTimerFunc(ms, update, 0);									//update at 60 frames per second

	glLoadIdentity();

	//Camera
	gluLookAt( 0,-1,5, 0,0,0, 0,1,0);
	background.Draw();
	//Draw player
	player[0]->setColor(1.0f, 0.0f,0.0f);
	player[0]->Draw();

	//Draw the enemies
	
	for(unsigned int i = 0; i < enemies.size(); i++){
			enemies[i]->Draw();	
			enemyModel.Position(enemies[i]->getX(), enemies[i]->getY()-0.5f, enemies[i]->getZ());
			enemyModel.drawObject();
	}

	enemySpawn();

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
void playerMove(float &deltaTime) {

	//Logic to handle special key presses
	if(keyPress.Left()) {
		if(player[0]->getX() > leftWall)			//If the player has not hit the left wall, move left
			player[0]->moveLeft(deltaTime);
		else
			player[0]->stopMovingLeft(player[0]->getX());
	} 
	if(keyPress.Right()) {
		if(player[0]->getX() < righttWall)			//If the player has not hit the right wall, move right
			player[0]->moveRight(deltaTime);
		else
			player[0]->stopMovingRight(player[0]->getX());
	} 
	if(keyPress.Up()) {
		if(player[0]->getY()  < topWall)			//If the player has not hit the top wall, move up
			player[0]->moveUp(deltaTime);
		else
			player[0]->stopMovingUp(player[0]->getY());
	} 
	if(keyPress.Down()) {
		if(player[0]->getY() > bottomWall)			//If the player has not hit the bottom wall, move down
			player[0]->moveDown(deltaTime);
		else 
			player[0]->stopMovingDown(player[0]->getY());
	} 
	if(keyPress.Space()) {							//Player shoots
		player[0]->shoot();
	} 

	//Boundary collision
	
}

/*******************************************************************
				enemyCollsion()
		Determines whether or not a player has collided with
		an enemy.
********************************************************************/
void enemyCollision() {
	int index = -1;
	int size = enemies.size();
	for(unsigned int i = 0; i < enemies.size(); i++){
		if((box.intersect(*player[0], *enemies[i]))){
			enemies[i]->setColor(0.0f, 1.0f, 0.0f);	
			//enemies.erase(enemies.begin() + i);
		} 
		if(player[0]->bulletCollision(*enemies[i])) {
			index = i;
			deleteEnemies(index);
			break;
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

//Initialize all of the lighting and camera view 
void initialize () 
{
    glMatrixMode(GL_PROJECTION);
	glViewport(0, 0, win.width, win.height);
	GLfloat aspect = (GLfloat) win.width / win.height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective(win.field_of_view_angle, aspect, win.z_near, win.z_far);
    glMatrixMode(GL_MODELVIEW);
    glShadeModel( GL_SMOOTH );
    glClearColor( 0.0f, 0.1f, 0.0f, 0.5f );
    glClearDepth( 1.0f );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

    GLfloat amb_light[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    GLfloat diffuse[] = { 0.6f, 0.6f, 0.6f, 1.0f };
    GLfloat specular[] = { 0.7f, 0.7f, 0.3f, 1.0f };
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, amb_light );
    glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse );
    glLightfv( GL_LIGHT0, GL_SPECULAR, specular );
    glEnable( GL_LIGHT0 );
    glEnable( GL_COLOR_MATERIAL );
    glShadeModel( GL_SMOOTH );
    glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE );
    glDepthFunc( GL_LEQUAL );
    glEnable( GL_DEPTH_TEST );
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0); 
}