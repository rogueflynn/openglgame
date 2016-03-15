/*********************************************************************************
		??? class:
		Author: Victor Gonzalez
		??? Purpose:
			Serves as the template for all player objects.  Stores all attributes
			for the player and handles the current status of the player.

**********************************************************************************/
#pragma once

#include "Sprite.h"

class Tiles : public Sprite {

/*PRIVATE BLOCK*/
private:
	GLuint *tileVbo; 
	GLuint *ebo;
	GLuint imageVbo;				//Stores vector objects

/*PUBLIC BLOCK*/
public:
	

	/***********************************************************
				Tiles constructor
			The player constructor sets all of the default 
			values for the class attributes.

	**************************************************************/
	Tiles() {
			//used to store buffers and vertex data
			//Global variables
			scale.Set(0.1f, 0.1f, 0.1f);						//Set scale
			color.setColor(0.0f, 0.0f, 1.0f);					//sets the color
			position.Set(0.5f,0.0f,0.0f);
			shader = Shader::getInstance();
	}

	/***********************************************************
				Initialize the enemy object 
	************************************************************/
	void init() {
		tileVbo = new GLuint;
		ebo = new GLuint;
		//glEnable(GL_TEXTURE_2D);
		//Create shaders object and load data into it
		setShader();	

		glEnableVertexAttribArray(posAttrib);		//Create the memory location that stores the position at location 0
		glEnableVertexAttribArray(colAttrib);		//Create the memory location that stores the color at location 1
		glEnableVertexAttribArray(texAttrib);		//Create the memory location that stores the texture at location 2

		//Specifies the order in which vertices are drawn
		GLuint elements[]  = {
			0,1,2,
			2,3,0
		};

		GLfloat tilesVert[] = {
			-0.5f,  0.5f, 0.0f, 0.0f, 0.0f,			// Top-left
			 0.5f,  0.5f, 0.0f, 0.0f, 0.0f,			// Top-right
			 0.5f, -0.5f, 0.0f, 0.0f, 0.0f,			// Bottom-right
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f			// Bottom-left
		};

		size = ((tilesVert[1] * 10 * 2) + 1) * scale.X;
		//upload the vertex data object to the graphcs card
		//Create a vertex buffer object

		glGenBuffers(1, tileVbo);					//Generate 1 buffer
		glBindBuffer(GL_ARRAY_BUFFER, *tileVbo);		//To upload actual data, make vbo the active object 
		
		//Now that vbo is the active array buffer, copy the vertex data into it
		//The first parameter references the active array buffer
		glBufferData(GL_ARRAY_BUFFER, sizeof(tilesVert), tilesVert, GL_STATIC_DRAW);

		/*
		glGenBuffers(1, &imageVbo);		//Generate 1 buffer
		glBindBuffer(GL_ARRAY_BUFFER, imageVbo);		//To upload actual data, make vbo the active object 
		
		//Now that vbo is the active array buffer, copy the vertex data into it
		//The first parameter references the active array buffer
		glBufferData(GL_ARRAY_BUFFER, sizeof(imageVert), imageVert, GL_STATIC_DRAW);
		*/

		//Element array buffer
		glGenBuffers(1, ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

	}

	/**********************************************
		The draw function is used to draw objects
		to the screen.
	************************************************/
	 void Draw() {
		glBindBuffer(GL_ARRAY_BUFFER, *tileVbo);
		glUniform3f(Scale, scale.X, scale.Y, scale.Z);									//Scales the object 
		glUniform3f(Position, position.X, position.Y, position.Z);						//Translates the object
		glUniform3f(Rotation, rotation.Pitch, rotation.Yaw, rotation.Roll);				//Rotation
		glUniform3f(uniformColor, color.R, color.G, color.B);							//Sets uniform color
		glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0); //5*sizeof(float) represents 5 floatig point # in array
		glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(2*sizeof(float))); 
		//glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
		 // Draw a rectangle from the 2 triangles using 6 indices
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
	void output() {
		 std::cout << "Box:  " << *tileVbo << " is stored at " << tileVbo<< std::endl;
	}


	/**********************************************
		The drawImage function is used to draw an image 
		to the screen.
	************************************************/
	void drawImage() {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniform3f(Scale, 0.5f, 0.5f, scale.Z);										//Scales the object 
		glUniform3f(Position, -0.7f, 0.85f, position.Z);								//Translates the object
		glUniform3f(Rotation, rotation.Pitch, rotation.Yaw, rotation.Roll);				//Rotation 
		glUniform3f(uniformColor, 1, 1, 1);												//Color
		glBindBuffer(GL_ARRAY_BUFFER, imageVbo);
		glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 7*sizeof(float), 0); //5*sizeof(float) represents 5 floatig point # in array
		glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 7*sizeof(float), (void*)(2*sizeof(float))); 
		glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDisable(GL_BLEND);
	}

	~Tiles() {
		delete tileVbo;
		delete ebo;
	}

/*PROTECTED BLOCK*/
protected:

};