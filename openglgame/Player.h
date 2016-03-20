/*********************************************************************************
		Player class:
		Author: Victor Gonzalez
		Purpose:
			Serves as the template for all player objects.  Stores all attributes
			for the player and handles the current status of the player.

**********************************************************************************/
#pragma once
#include "Sprite.h"

class Player : public Sprite {

/*PRIVATE BLOCK*/
private:
	//GLuint playerVbo, ebo, imageVbo;			//Stores vector buffer objects
	
/*PUBLIC BLOCK*/
public:
	~Player() {
		std::cout << "Destructor called.\n";
	}
	/**************************************************
					initialize the player
	***************************************************/
	void init() {
		GLuint textureBufferID = loadAndBufferImage("hitbox.png");
		setPoint(0.25f, 0.25f, -1.1f);
		
		//Calculate size of square
		size = ((point.X * 10 * 2) + 1) * scale.X;
		position.Set(-0.74, 0.3f,0.0f);
	}

	/**********************************************
		The draw function is used to draw objects
		to the screen.
	************************************************/
	void Draw() {
		glPushMatrix();
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