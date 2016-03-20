/*********************************************************************************
		Player class:
		Author: Victor Gonzalez
		Purpose:
			Serves as the template for all player objects.  Stores all attributes
			for the player and handles the current status of the player.

**********************************************************************************/
#pragma once
#include "Sprite.h"

class Tiles : public Sprite {

/*PRIVATE BLOCK*/
private:
	//GLuint playerVbo, ebo, imageVbo;			//Stores vector buffer objects
	
/*PUBLIC BLOCK*/
public:
	~Tiles() {
		std::cout << "Destructor called.\n";
	}
	/**************************************************
					initialize the player
	***************************************************/
	void init() {
		GLuint textureBufferID = loadAndBufferImage("hitbox.png");
		setPoint(0.5f, 0.5f, -1.0f);
		setColor(0.0f, 0.0f, 1.0f);
		//Calculate size of square
		size = ((point.X * 10 * 2) + 1) * scale.X;
		
	}

	/**********************************************
		The draw function is used to draw objects
		to the screen.
	************************************************/
	void Draw() {
		/*
		glBindBuffer(GL_ARRAY_BUFFER, playerVbo);
		glUniform3f(Scale, scale.X, scale.Y, scale.Z);									//Scales the object 
		glUniform3f(Position, position.X, position.Y, position.Z);						//Translates the object
		glUniform3f(Rotation, rotation.Pitch, rotation.Yaw, rotation.Roll);				//Rotation
		glUniform3f(uniformColor, color.R, color.G, color.B);							//Sets uniform color
		glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0);	//5*sizeof(float) represents 5 floatig point # in array
		glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(2*sizeof(float))); 
		//glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
		 // Draw a rectangle from the 2 triangles using 6 indices
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
			*/
		glPushMatrix();
//			glEnable(GL_TEXTURE_2D);
//			glEnable(GL_BLEND);
//			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
//			glDisable(GL_TEXTURE_2D);
		glPopMatrix();

	}

/*Protected Block*/
protected:

};