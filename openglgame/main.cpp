/**********************************************************
	Author: Victor Gonzalez
	Purpose:
		Main entry point for the game engine. 

************************************************************/
//#include <windows.h>
#include <glew.h>
#include <freeglut.h>
#include <iostream>
#include <stdlib.h>
#include "Player.h"
#include "shader.h"
#include "Enemy.h"
#include <math.h>
#include "Box.h"
#include "Key.h"
#include <vector>
#include "Camera.h"
#include "Sprite.h"
#include "Model_OBJ.h"
#include <ctime>
#include <cstdlib>
#include "Background.h"
#include "SceneTransition.h"
#include <string.h>
#include <fstream>
#include "Sound.h"


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

/************************************************************************
 Scene Changing enum & global variable
************************************************************************/
enum _game_scenes
{
	_scene_start_menu = 0,		//main menu
	_scene_new_game_intro = 1,	//scrolling storyline text
	_scene_level_one = 2,		//enum for level 1
	_scene_level_two = 3,
	_scene_game_over = 4,		//fade in text "game over" fade out to start menu
	_scene_high_score = 5,		//not used, TODO list
	_scene_exit = 6,			//exit the game
};
int Current_Game_Scene_Enum = 0; //init to start screen here

/*******************************
 _scene_level_one - Variables
********************************/
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
int killCount = 0;
//Vectors
std::vector<Enemy*> enemies;		//Enemy
std::vector<Player*> player;		//Player vector
std::vector<Model_OBJ> enemyModelObj;
//Objects
Box box;							//Used to check collision detection
Key keyPress;
Model_OBJ enemyModel;
Background background;
SceneTransition sceneTransition;
Sound gameSound;


/***********************************************
_scene_start_menu - Variables & text data
***********************************************/
char gameTitle[1][30];
int gameTitleLineCount = 1;
char startMenuInstructions[2][80];
int startInstructLineCount = 2;

/***********************************************
_scene_new_game_intro - Variables & quote data
***********************************************/
GLfloat UpwardsScrollVelocity = -40.0;
float view = 20.0;
char quote[8][80];
int numberOfQuotes = 8;
float deltaTimett = 1.0f;
float currentTimett, previousTimett;
float customScrollingCount = 0;				//custom calculations using deltatime
float decrementCount = 0;
const float CUSTOMTIME_TO_SCROLL = 60.0f;	//number of seconds game intro will play
bool firstPassTimerNGI = true;
bool firstPassTimerL1 = true;
bool fadeStarted = false;


/***********************************
 all scenes - Method Prototypes
************************************/
//Prototypes
void key(unsigned char, int, int);
void mouse(int , int , int , int );
void playerMove(float &);
void update(int data);		// currently used to update gameplay levels only
void timeTick(int data);	// currently used to update intro scene only
void renderIntroText();
void enemyCollision();
void render();				// render() decides which scene to render next
void initialize();			// initialize settings for all scenes


/***********************************
 all scenes - Helper Methods
************************************/
//deletes enemy objects
void deleteEnemies(int &index) {
		delete enemies[index];
		enemies.erase(enemies.begin() + index);

}
//ensures no data left behind by enemies
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


/**********************************************************
 MAIN
**********************************************************/
int main(int argc, char** argv) {
	srand(time(NULL));
	// set window values
	win.width = 1024;
	win.height = 600;
	win.title = "Graphics Project";
	win.field_of_view_angle = 90;
	win.z_near = 1.0f;
	win.z_far = 500.0f;


	//load start menu text here
	std::strcpy(gameTitle[0], "The Conquest of Sontar");

	std::strcpy(startMenuInstructions[0], "press 'Space' to start your New Conquest!");
	std::strcpy(startMenuInstructions[1], "press 'Escape' to exit like a coward.");


	//load new game intro text here
	std::strcpy(quote[0], "Hello Sontaran Protector... this is Sontar Fleet Command.");
	std::strcpy(quote[1], "Your oath to protect the empress with your life must be upheld.");
	std::strcpy(quote[2], "At this moment the Glock race has abducted the empress and fled.");
	std::strcpy(quote[3], "you must begin the treacherous journey of locating and saving the empress");
	std::strcpy(quote[4], "The Glock race could hide their important prisoners anywhere in the galaxy");
	std::strcpy(quote[5], "I dont have to tell you...every moment passed puts the empress’ life a risk.");
	std::strcpy(quote[6], "Good luck Protector,");
	std::strcpy(quote[7], "and may Zontar's divine wisdom of war and power guide you.");

	//load 2 models into memory
	enemyModel.Load("models/eShip3.obj");
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

	if (!(background.LoadGLTextures())) {							//load background for the levels
		printf("error loadin scrolling background");				//dont render game if it doesnt load
		return 1;
	}

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


/************************************************
			update() 
	All update logic goes in this function.
	update will keep track of the status and
	location of the player and tiles.
*************************************************/
void update(int data) {

	background.Scroll();
	//Calculates the frame rate
	previousTime = currentTime;
	currentTime = (float)glutGet(GLUT_ELAPSED_TIME);
	deltaTime = (currentTime - previousTime);

	enemyCollision();							//Check if the player has collided with an enemy
	playerMove(deltaTime);						//Move the player
	if (enemies.size() < 7 && enemyCount != 48)
		loadEnemies();

	if (enemies.size() == 1 && enemyCount == 48)
		cleanUp();

	/* KILL COUNT FOR LEVEL ! only */
	if (killCount == 5) {
		fadeStarted = true;
	}

	//cout << "X: " <<  player[0]->getX() << " Y:" << player[0]->getY() << "\n";		
	glutPostRedisplay();
}

/********************************************************
	render() called in main as: glutDisplayFunc(render);												
	The render function is used to draw objects
	to the screen.
********************************************************/
void render() {

	switch (Current_Game_Scene_Enum)
	{
	case _scene_start_menu:

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				//Clear the screen of previous drawn data
		
		//if player hits ENTER_KEY, change CURRENT_GAME_SCENE_ENUM == _scene_new_game_intro
		//glutTimerFunc(ms, startMenuKeypress, 0);

		glLoadIdentity();

		//camara
		gluLookAt(0, -1, 5, 0, 0, 0, 0, 1, 0);

		//draw BKGRND glQuad to the screen
		background.DrawStartMenuBackground();

		//render TITLE text to the screen
		//place TITLE infront of BKGRND
		glLineWidth((GLfloat) 3.0f);
		int lengthOfLine, gtl, i;

		for (gtl = 0;gtl<gameTitleLineCount;gtl++)
		{
			lengthOfLine = (int)strlen(gameTitle[gtl]);
			glPushMatrix();
				//glTranslatef(-(lengthOfLine * 37), -(gtl * 200), 0.0);
				//glTranslatef(0.0f, 0.0f, 0.0f);
				glRasterPos2f(-1.8f, 2);
				for (i = 0; i < lengthOfLine; i++)
				{
					glColor3f( 1.0f, 1.0f, 1.0f);	//stainless steel rgb code
					//glutStrokeCharacter(GLUT_STROKE_ROMAN, gameTitle[gtl][i]);
					glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, gameTitle[gtl][i]);
				}
			glPopMatrix();
		}

		//render NEWGAMECMND text to the screen
		//place NEWGAMECMND infront of BKGRND
		glLineWidth((GLfloat) 1.0f);
		int lengthOfLine2, gtl2, i2;

		for (gtl2 = 0;gtl2<startInstructLineCount;gtl2++)
		{
			lengthOfLine2 = (int)strlen(startMenuInstructions[gtl2]);
			glPushMatrix();
				glRasterPos2f( (float) -1.0f-(lengthOfLine2/30.0f), (float) -1.0f-(gtl2*0.75f) );
				for (i2 = 0; i2 < lengthOfLine2; i2++)
				{
					glColor3f((224.0f / 256.0f), (223.0f / 256.0f), (219.0f / 256.0f));	//stainless steel rgb code
					glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, startMenuInstructions[gtl2][i2]);
				}
			glPopMatrix();
		}

		//swap buffers to clear and redraw
		glutSwapBuffers();

		break;

	case _scene_new_game_intro:

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				//Clear the screen of previous drawn data

		glutTimerFunc(ms, timeTick, 0);									//update at 60 frames per second

		glLoadIdentity();

		gluLookAt(0.0, 30.0, 90.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

		renderIntroText();

		background.DrawNewGameIntroBackground();

		if (fadeStarted) {
			decrementCount = sceneTransition.FadeOut(decrementCount);
			//check shrinking count for fadeout finished = 0
			if (decrementCount <= 0) {
				//change scene for render scene switch
				Current_Game_Scene_Enum = _scene_level_one;
				//reset some variables
				customScrollingCount = 0;
				decrementCount = 0;
				//firstPassTimer = true;
				fadeStarted = false;
			}
		}
		
		glutSwapBuffers();

		break;

	case _scene_level_one:

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				//Clear the screen of previous drawn data

		glutTimerFunc(ms, update, 0);									//update at 60 frames per second

		glLoadIdentity();

		//Camera
		gluLookAt(0, -1, 5, 0, 0, 0, 0, 1, 0);
		background.Draw();
		//Draw player
		player[0]->setColor(1.0f, 0.0f, 0.0f);
		player[0]->Draw();

		//Draw the enemies

		for (unsigned int i = 0; i < enemies.size(); i++) {
			enemies[i]->Draw();
			enemyModel.Position(enemies[i]->getX(), enemies[i]->getY() - 0.5f, enemies[i]->getZ());
			enemyModel.drawObject();
		}

		enemySpawn();

		//after update checks for correct kill count, being fadeOut
		if (fadeStarted) {
			//init and update the decrement-count
			if (firstPassTimerL1) {
				decrementCount = sceneTransition.FadeOut(70.0f);
				firstPassTimerL1 = false;
			}
			else if (!firstPassTimerL1){
				decrementCount = sceneTransition.FadeOut(decrementCount);

				//check shrinking count for fadeout finished = 0
				if (decrementCount <= 0) {
					//change scene for render scene switch
					Current_Game_Scene_Enum = _scene_start_menu;	//-- goes back to start screen
																	//reset some variables
					customScrollingCount = 0;
					decrementCount = 0;
					//firstPassTimer = true;
					fadeStarted = false;
				}
			}
		}

		glutSwapBuffers();		//Swaps the buffers when double buffer is used

		break;

	case _scene_level_two:

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				//Clear the screen of previous drawn data

																		// update fucntion called
		glutTimerFunc(ms, update, 0);									//update at 60 frames per second

		glLoadIdentity();

		//Camera
		gluLookAt(0, -1, 5, 0, 0, 0, 0, 1, 0);
		background.Draw();
		//Draw player
		player[0]->setColor(1.0f, 0.0f, 0.0f);
		player[0]->Draw();

		//Draw the enemies

		for (unsigned int i = 0; i < enemies.size(); i++) {
			enemies[i]->Draw();
			enemyModel.Position(enemies[i]->getX(), enemies[i]->getY() - 0.5f, enemies[i]->getZ());
			enemyModel.drawObject();
		}

		enemySpawn();

		glutSwapBuffers();		//Swaps the buffers when double buffer is used

		break;

	case _scene_game_over:	//not implemented, here for later use
		break;

	case _scene_high_score:	//not implemented, here for later use
		break;

	case _scene_exit:
		exit(0);
		break;

	default:
		printf("FATAL ERROR: 'Current_Game_Scene_Enum' is not set to any of the current enum values");
		break;
	}

}


//void startMenuKeypress(int data) {
//
//	if (keyPress.Enter()) {
//		Current_Game_Scene_Enum = _scene_new_game_intro;
//	}
//	//if player hits ESC_KEY, change CURRENT_GAME_SCENE_ENUM == _scene_exit
//	if (keyPress.Escape()) {
//		Current_Game_Scene_Enum = _scene_exit;
//	}
//
//}


//*********************************************
//  draw the intro text to display window        
//*********************************************
void renderIntroText() {
	int l, lenghOfQuote, i;

	glTranslatef(0.0, -100.0, UpwardsScrollVelocity);
	glRotatef(-20, 1.0, 0.0, 0.0);
	glScalef(0.1f, 0.1f, 0.1f);

	for (l = 0;l<numberOfQuotes;l++)
	{
		lenghOfQuote = (int)strlen(quote[l]);
		glPushMatrix();
		glTranslatef((float) -(lenghOfQuote * 37), (float) -(l * 200), 0.0);
		for (i = 0; i < lenghOfQuote; i++)
		{
			glColor3f((UpwardsScrollVelocity / 10) + 300 + (l * 10), (UpwardsScrollVelocity / 10) + 300 + (l * 10), (UpwardsScrollVelocity / 10) + 300 + (l * 10));
			glutStrokeCharacter(GLUT_STROKE_ROMAN, quote[l][i]);
		}
		glPopMatrix();
	}
}


//*********************************************
//  glutTimerFunc(xx, timeTick, xx);         
//*********************************************
void timeTick(int data)
{
	//timer variables and  logic
	previousTimett = currentTimett;
	currentTimett = (float)glutGet(GLUT_ELAPSED_TIME);
	if (firstPassTimerNGI) {
		deltaTimett = 1.0f;
		firstPassTimerNGI = false;
	}
	else { 
		deltaTimett = (currentTime - previousTime);
	}

	//countdown to fadeOut and scene change logic
	if (customScrollingCount < CUSTOMTIME_TO_SCROLL) {
		//increment count before limit
		customScrollingCount += (deltaTime / 100.0f);
	}
	else if ( customScrollingCount >= CUSTOMTIME_TO_SCROLL) {

		if (!fadeStarted) {
		//decrement a copy of count after limit and before each call to fade
			decrementCount = customScrollingCount;
			fadeStarted = true;
		}

	}
	

	if (UpwardsScrollVelocity< -600)
		view -= 0.000011f;
	if (view < 0) { view = 20; UpwardsScrollVelocity = -10.0; }
	//  exit(0);
	UpwardsScrollVelocity -= 0.015f;
	glutPostRedisplay();

}


/*********************************************
	The key function takes in a key and runs
	the event associated with it.
**********************************************/
void key(unsigned char key, int x, int y) {
	switch(key) 
	{
	case 27:			//27 represents the escape key
		exit(0);		//Exit the progam when escape is pressed
		break;
		
	// glut key right-hand shift used on start screen only
	case 32:
		if (Current_Game_Scene_Enum == _scene_start_menu) 
		{
			Current_Game_Scene_Enum = _scene_new_game_intro;
			glutPostRedisplay();
		}
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
		gameSound.playShot();
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
	for(unsigned int i = 0; i < enemies.size(); i++) {

		if((box.intersect(*player[0], *enemies[i]))) {
			enemies[i]->setColor(0.0f, 1.0f, 0.0f);	
			//enemies.erase(enemies.begin() + i);
		}

		if(player[0]->bulletCollision(*enemies[i])) {
			index = i;
			deleteEnemies(index);
			//update global killCount variable here, to advance the game with
			killCount++;
			printf("Kill count is: %i", killCount);
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

	if (!gameSound.init("Sounds/background.raw", "Sounds/shot.raw"))
			printf("Sounds can't be loaded properly\n");
		else
			gameSound.playBackground();
}