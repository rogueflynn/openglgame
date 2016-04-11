/*************************************************************************
	The background class is used to 

***************************************************************************/

#pragma once
#include<gl/glew.h>
#include<gl/freeglut.h>
#include<gl/SOIL.h>
#include<iostream>

class Background {
private:
	GLuint	texture[1];				// Storage For One Texture 
	//Globals
	float rotCubeX, rotCubeY;
	float scrollY1, scrollY2;		//Stores the scroll values
	float top, bottom, center;

public:
	//Constructor
	Background() {
		top = 25.0f;
		bottom = -35.0f;
		center = -5.0f;
		rotCubeX = 0.0f, rotCubeY = 0.0f;
		scrollY1 = top, scrollY2 = center;
	}
	//Loads the texture into the texture buffer
	int LoadGLTextures()									// Load Bitmaps And Convert To Textures
	{
		/* load an image file directly as a new OpenGL texture */
		texture[0] = SOIL_load_OGL_texture
			(
			"Data/space.png",
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_INVERT_Y
			);

		if(texture[0] == 0)
			return false;


		// Typical Texture Generation Using Data From The Bitmap
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	return true;										// Return Success
	}
	
	//Draws
	void Draw() {
			
		glPushMatrix();
			if(scrollY1  < bottom) 
				scrollY1 = top;
			glBindTexture(GL_TEXTURE_2D, texture[0]);
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glTranslatef(0.0f, scrollY1 ,0.0f);
			//glRotatef(rotCubeX,1.0f,0.0f,0.0f);
			//glRotatef(rotCubeY,0.0f,1.0f,0.0f);
			glRotatef(90,0.0f,0.0f,1.0f);
			drawSquareOne();
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();

		glPushMatrix();
			if(scrollY2  < bottom) 
				scrollY2 = top;
			glBindTexture(GL_TEXTURE_2D, texture[0]);
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glTranslatef(0.0f, scrollY2,0.0f);
			//glRotatef(rotCubeX,1.0f,0.0f,0.0f);
			glRotatef(rotCubeY,0.0f,1.0f,0.0f);
			drawSquareTwo();
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();
		}
		
	//This method is used to store the vertices of the cube
	void drawSquareOne() {
		glPushMatrix();
		glBegin(GL_QUADS);
			// Front Face
			glColor3f(1.0f, 1.0f, 1.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-15.0f, -15.0f,  -1.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 15.0f, -15.0f,  -1.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 15.0f,  15.0f,  -1.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-15.0f,  15.0f,  -1.0f);
		glEnd();
		glPopMatrix();

	}
			
	//This method is used to store the vertices of the cube
	void drawSquareTwo() {
		glPushMatrix();
		glBegin(GL_QUADS);
			// Front Face
			glColor3f(1.0f, 1.0f, 1.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-15.0f, -15.0f,  -1.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 15.0f, -15.0f,  -1.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 15.0f,  15.0f,  -1.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-15.0f,  15.0f,  -1.0f);
		glEnd();
		glPopMatrix();

	}

	//Scrolls the background
	void Scroll() {
		scrollY1 -= 0.03f; 
		scrollY2 -= 0.03f;
	}
	protected:
};