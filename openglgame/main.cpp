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
#include "Model_OBJ.h"

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

//Objects
std::vector<Enemy*> enemies;		//Player
std::vector<Player*> player;		//Player vector
Box box;							//Used to check collision detection
Key keyPress;
Model_OBJ model;

//Prototypes
void render();
void key(unsigned char, int, int);
void mouse(int , int , int , int );
void playerMove();
void update(int);
void enemyCollision();
void initialize();
float rotX = 65.0f, rotY = 0.0f, rotZ = -0.7f;
void special(int key, int x, int y) {
	switch(key) {
		case GLUT_KEY_LEFT:
			rotY -= 1.1f;
			glutPostRedisplay();
			cout << "Y " << rotY << endl;
			break;
		case GLUT_KEY_RIGHT:
			rotY += 1.1f;
			glutPostRedisplay();
			cout << "Y " << rotY << endl;
			break;
		case GLUT_KEY_DOWN:
			rotX -= 1.1f;
			glutPostRedisplay();
			cout << "X " << rotX << endl;
			break;
		case GLUT_KEY_UP:
			rotX += 1.1f;
			glutPostRedisplay();
			cout << "X " << rotX << endl;
			break;
		case GLUT_KEY_PAGE_UP:
			rotZ += 1.1f;
			glutPostRedisplay();
			cout << "Z " << rotZ << endl;
			break;
		case GLUT_KEY_PAGE_DOWN:
			rotZ -= 1.1f;
			glutPostRedisplay();
			cout << "Z " << rotZ << endl;
			break;
	}
}
int main(int argc, char** argv) {
	// set window values
	win.width = 1200;
	win.height = 800;
	win.title = "Graphics Project";
	win.field_of_view_angle = 90;
	win.z_near = 1.0f;
	win.z_far = 500.0f;


	for(int i=0; i < 4; i++)
		enemies.push_back(new Enemy());
	player.push_back(new Player());

	//Create window
	glutInit(&argc, argv);											//Initialize glut
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);		//Specify the modes to render your window
	glutInitWindowSize(win.width, win.height);									//Set the window size 
	glutInitWindowPosition(100,100);								//Set the position of the window on the screen
	glutCreateWindow(win.title);							//Creates the window and sets the title
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
	enemies[0]->translate(-5.5f, 4.0f);
	enemies[1]->translate(5.5f, 4.0f);
	enemies[3]->translate(5.5f, -2.0f);
	enemies[2]->translate(-5.5f, -2.0f);

	//Call back functions
	glutDisplayFunc(render);										//Draws whatever is passed in to the screen
	glutKeyboardFunc(key);											//Executes event based on the key pressed
	//glutSpecialFunc(special);
	glutMouseFunc(mouse);											//Executes event based on mouse button pressed

	initialize();
	model.Load("ship.obj");
	model.Position(30.6f,-320.0f,-200.0f);
	player[0]->translate(player[0]->getX(), -3.75);
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
	glLoadIdentity();
	gluLookAt( 0,1,5, 0,0,-1, 0,1,0);
	//model.Rotation(rotX, rotY, rotZ);
	model.drawObject();
	if(!alive) {
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
	if(keyPress.Left()) {
			player[0]->moveLeft();
			model.posX -= 2.0f;
	} else if(keyPress.Right()) {
			player[0]->moveRight();
			model.posX += 2.0f;
	} else if(keyPress.Up()) {
			player[0]->moveUp();
			model.posY += 2.1f;
	} else if(keyPress.Down()) {
			player[0]->moveDown();
			model.posY -= 2.1f;
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
			enemies[i]->setColor(0.0f, 1.0f, 0.0f);	
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