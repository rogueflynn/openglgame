#pragma once
/**********************************************************
Authors: Jordan C Walsh, Victor Gonzalez
Purpose:
scene transtion effects, fade in/out using alpha values on 
glQuads render over the current scene, also sets timers for
how long to wait for each scene to change

************************************************************/
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/SOIL.h>
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

class SceneTransition {

	/************************************************************************
	Scene Changing enum & global variable
	************************************************************************/
	//enum _game_scenes
	//{
	//	_scene_start_menu = 0,		//main menu
	//	_scene_new_game_intro = 1,	//scrolling storyline text
	//	_scene_level_one = 2,		//enum for level 1
	//	_scene_level_two = 3,
	//	_scene_game_over = 4,		//fade in text "game over" fade out to start menu
	//	_scene_high_score = 5,		//not used, TODO list
	//	_scene_exit = 6,			//exit the game
	//};
private:
	GLuint	textureST[1];		// Storage For One Texture 

	/* Global variables */
	//color variables R,G,B changing from 1 to 0 to turn quad black
	float alphaST = 0.0f;
	float red = 1.0f, green = 1.0f, blue = 1.0f;
	//timer stuff
	int msST = (1 / 60) * 1000;  //Calculates 60fps (1 sec/60fps) * (1000ms / 1 sec)
	float deltaTimeST = 1.0f;
	float currentTimeST, previousTimeST;
	//scene enum
	int currentSceneEnum;
	float rotCubeXst, rotCubeYst;
	float scrollY1ST, scrollY2ST;		//Stores the scroll values
	float topST, bottomST, centerST;

public:

	//Constructor
	int SceneTransiton() {
		topST = 25.0f;
		bottomST = -35.0f;
		centerST = -5.0f;
		rotCubeXst = 0.0f, rotCubeYst = 0.0f;
		scrollY1ST = topST, scrollY2ST = centerST;
	}


	//Loads the texture into the texture buffer
	int LoadGLTextures()									// Load Bitmaps And Convert To Textures
	{
		/* load an image file directly as a new OpenGL texture */
		textureST[0] = SOIL_load_OGL_texture
			(
				"Data/NeHe.bmp",
				SOIL_LOAD_AUTO,
				SOIL_CREATE_NEW_ID,
				SOIL_FLAG_INVERT_Y
				);

		if (textureST[0] == 0)
			printf("false");
		return false;


		// Typical Texture Generation Using Data From The Bitmap
		glBindTexture(GL_TEXTURE_2D, textureST[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		return true;										// Return Success
	}


	/* call FadeOut on scene with float count of 120.0 or less
	count will be decremented and returned to timeTick() to
	continue animation and fade at same time
	*/

	float FadeOut(float count) {

		float countCpy = count;
		//subtracted from count
		float rateFadeOut = 0.15f;

		// count should be decremented at same FPS as game display rendering
		if (count > 0.0f) {

			//update count to return decremented count
			//also decrements floats in RBG color of quad
			count -= rateFadeOut;

			//updated each time game calls this fucntion to get darker
			alphaST += (rateFadeOut/countCpy);
		}
		//else {
		//	//just to ensure black out happens when count is done
		//	alphaST = 1.0f;
		//}

		glPushMatrix();

			//glBindTexture(GL_TEXTURE_2D, textureST[0]);
			//glEnable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);
			//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDisable(GL_DEPTH_TEST);

			//adjusted specifically for new game intro scene
			glTranslatef(0.0f, 0.0f, 15.0f);
			glScalef(12.0f, 12.0f, 12.0f);
			//glRotated(10, 1.0f, 0.0f, 0.0f);
		/*	red = 3.0f;
			green = 0;
			blue = 0;*/
			glColor4f(0.0f, 0.0f, 0.0f, alphaST);		//updated each time game calls to be darker
			drawSquare();
			//glDisable(GL_TEXTURE_2D);
			glEnable(GL_DEPTH_TEST);
			//glDisable(GL_BLEND);
		glPopMatrix();

		return count;

	}

	//int FadeIn() {
	//
	//}

	//This method is used to store the vertices of the cube
	void drawSquare() {
		glPushMatrix();
			glBegin(GL_QUADS);
				// Front Face
				//glTexCoord2f(0.0f, 0.0f); 
				glVertex3f(-400.0f, -400.0f, -1.0f);
				//glTexCoord2f(1.0f, 0.0f); 
				glVertex3f(400.0f, -400.0f, -1.0f);
				//glTexCoord2f(1.0f, 1.0f); 
				glVertex3f(400.0f, 400.0f, -1.0f);
				//glTexCoord2f(0.0f, 1.0f); 
				glVertex3f(-400.0f, 400.0f, -1.0f);
			glEnd();
		glPopMatrix();

	}

	//void SetTimer() {
	//
	//	switch (currentSceneEnum)
	//	{
	//	case _scene_start_menu:
	//
	//		break;
	//	
	//	case _scene_new_game_intro:
	//		break;
	//
	//	case _scene_level_one:
	//		break;
	//
	//	case _scene_game_over:
	//		break;
	//
	//	case _scene_high_score:
	//		break;
	//
	//	case _scene_exit:
	//		break;
	//
	//	default:
	//		break;
	//	}
	//
	//}
protected:

};