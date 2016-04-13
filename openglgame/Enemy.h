/*********************************************************************************
		Enemy class:
		Author: Victor Gonzalez
		Purpose:
			Serves as the template for all enemy objects.  Stores all attributes
			for the enemies and handles the current status of all enemies.

**********************************************************************************/
#pragma once
#include "Sprite.h"
#include<iostream>

class Enemy : public Sprite {

/*PRIVATE BLOCK*/
private:
	Model_OBJ enemyModel;
	//GLuint playerVbo, ebo, imageVbo;			//Stores vector buffer objects
	GLuint textureBufferID;	
/*PUBLIC BLOCK*/
public:
	/**************************************************
					initialize the  enemy
	***************************************************/
	void init() {
		//Used to make the hitbox transparent
		textureBufferID = loadAndBufferImage("hitbox.png");
		setPoint(0.4f, 0.4f, 0.0f);
		//Calculate size of square
		size = ((point.X * 10 * 2) + 1) * scale.X;
		
	}

	/**********************************************
		The draw function is used to draw objects
		to the screen.
	************************************************/
	void Draw() {
		
		glPushMatrix();
			glBindTexture(GL_TEXTURE_2D, textureBufferID);
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glColor3f(color.R, color.G, color.B);
			glTranslatef(position.X, position.Y, position.Z);
			glScalef(scale.X, scale.X, scale.Z);
			glRotatef(rotation.Pitch,1.0f, 0.0f, 0.0f);			//X
			glRotatef(rotation.Yaw,0.0f, 1.0f, 0.0f);			//Y
			glRotatef(rotation.Roll,0.0f, 0.0f, 1.0f);			//Z
			glBegin(GL_QUADS);
					glVertex3f( point.X, point.Y, point.Z);    // Top Right Of The Quad (Front)
					glVertex3f(-point.X, point.Y, point.Z);    // Top Left Of The Quad (Front)
					glVertex3f(-point.X,-point.Y, point.Z);    // Bottom Left Of The Quad (Front)
					glVertex3f( point.X,-point.Y, point.Z);    // Bottom Right Of The Quad (Front
			glEnd();
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();

	}

/*Protected Block*/
protected:

};