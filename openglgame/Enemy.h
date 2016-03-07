/*********************************************************************************
		Player class:
		Author: Victor Gonzalez
		Purpose:
			Serves as the template for all player objects.  Stores all attributes
			for the player and handles the current status of the player.

**********************************************************************************/
#pragma once
#include<glew.h>
#include<freeglut.h>
#include<iostream>
#include<string>
#include<fstream>
#include<SOIL.h>
#include "shader.h"

class Enemy {

/*PRIVATE BLOCK*/
private:
	float scaleX, scaleY, scaleZ;				//Stores all the scale variables
	float transX, transY, transZ;				//Stores all of the translation variables
	float r, g, b;								//Stores the rgb values
	GLint scale, trans, rot;					//Uniform variables passed into the shaders
	GLint uniformColor;							//Uniform color variable
	float roll, yaw, pitch;						//Stores the x,y, and z rotation
	GLuint shaderProgram;						//Stores the location of the shader program
	GLuint vertexShaderID;						//THe storage location for the vertex shader
	GLuint fragmentShaderID;					//The storage location for the fragment shader
	GLint colAttrib, posAttrib, texAttrib;		//The color, position and texture attributes
	GLuint squareVbo, ebo, imageVbo;			//Stores vector objects
	Shader *shader;								

/*PUBLIC BLOCK*/
public:
	

	/***********************************************************
				Player constructor
			The player constructor sets all of the default 
			values for the class attributes.

	**************************************************************/
	Enemy() {
			//used to store buffers and vertex data
			//Global variables
			scaleX = 0.1f, scaleY = 0.1f, scaleZ = 0.1f;		//Scale variables
			transX = 0.5f, transY = 0.0f, transZ = 0.0f;		//Translation variables
			r = 0.0f, g = 0.0f, b = 1.0f;
			scale, trans, rot;
			roll = 0.0f;										//Controls rotation along the z-axis
			yaw = 0.0f;										//Controls rotation along the y-axis
			pitch = 0.0f;										//Controls rotation along the x-axis
			shader = Shader::getInstance();
	}

	
	/*************************************************************
						createShaders()
				Uses the readShaderCode function in order to 
				load in the shaders and then instatiate and link
				them to the min program.
	**************************************************************/
	void setShader() {
		shader->createShaders();
		colAttrib		=	glGetAttribLocation(shader->shaderProgram, "color");		
		posAttrib		=	glGetAttribLocation(shader->shaderProgram, "position");
		//texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
		scale			=	glGetUniformLocation(shader->shaderProgram, "s");
		trans			=	glGetUniformLocation(shader->shaderProgram, "t");
		rot				=	glGetUniformLocation(shader->shaderProgram, "r");
		uniformColor	=	glGetUniformLocation(shader->shaderProgram, "ucolor");
	}


	void init() {
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

		GLfloat squareVert[] = {
			-0.5f,  0.5f, 0.0f, 0.0f, 0.0f,			// Top-left
			 0.5f,  0.5f, 0.0f, 0.0f, 0.0f,			// Top-right
			 0.5f, -0.5f, 0.0f, 0.0f, 0.0f,			// Bottom-right
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f			// Bottom-left
		};

		
		//upload the vertex data object to the graphcs card
		//Create a vertex buffer object

		glGenBuffers(1, &squareVbo);					//Generate 1 buffer
		glBindBuffer(GL_ARRAY_BUFFER, squareVbo);		//To upload actual data, make vbo the active object 
		
		//Now that vbo is the active array buffer, copy the vertex data into it
		//The first parameter references the active array buffer
		glBufferData(GL_ARRAY_BUFFER, sizeof(squareVert), squareVert, GL_STATIC_DRAW);

		/*
		glGenBuffers(1, &imageVbo);		//Generate 1 buffer
		glBindBuffer(GL_ARRAY_BUFFER, imageVbo);		//To upload actual data, make vbo the active object 
		
		//Now that vbo is the active array buffer, copy the vertex data into it
		//The first parameter references the active array buffer
		glBufferData(GL_ARRAY_BUFFER, sizeof(imageVert), imageVert, GL_STATIC_DRAW);
		*/

		//Element array buffer
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

	}

	/**********************************************
		The draw function is used to draw objects
		to the screen.
	************************************************/
	void Draw() {
		glBindBuffer(GL_ARRAY_BUFFER, squareVbo);
		glUniform3f(scale, scaleX, scaleY, scaleZ);						//Scales the object 
		glUniform3f(trans, transX, transY, transZ);						//Translates the object
		glUniform3f(rot, pitch, yaw, roll);							    //Rotate along the z-axis	(roll)
		glUniform3f(uniformColor, r, g, b);							    //Rotate along the z-axis	(roll)
		glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0); //5*sizeof(float) represents 5 floatig point # in array
		glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(2*sizeof(float))); 
		//glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
		 // Draw a rectangle from the 2 triangles using 6 indices
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}


	/**********************************************
		The drawName function is used to draw my name 
		to the screen.
	************************************************/
	void drawName() {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniform3f(scale, 0.5f, 0.5f, scaleZ);						//Scales the object 
		glUniform3f(trans, -0.7f, 0.85f, transZ);						//Translates the object
		glUniform3f(rot, pitch, yaw, roll);							    //Rotate along the z-axis	(roll)
		glUniform3f(uniformColor, 1, 1, 1);							    //Rotate along the z-axis	(roll)
		glBindBuffer(GL_ARRAY_BUFFER, imageVbo);
		glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 7*sizeof(float), 0); //5*sizeof(float) represents 5 floatig point # in array
		glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 7*sizeof(float), (void*)(2*sizeof(float))); 
		glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDisable(GL_BLEND);
	}


	/*Sets the color of an object*/
	void setColor(float red, float green, float blue) {
		r = red;
		g = green;
		b = blue;
	}

	/*MATRIX OPERTATIONS*/

	//Performs translation on the x and y access
	void translate(float x, float y) {
		transX = x;
		transY = y;
	}

	//Controls the scale 
	void Scale(float x, float y) {
		scaleX = x;
		scaleY = y;
	}

	//Controls rotation along the x-axis
	void rotateX(float x) {
		pitch = x;
	}

	//Controls rotation along the y-axis
	void rotateY(float y) {
		yaw = y;
	}

	//Controls rotation along the z-axis
	void rotateZ(float z) {
		roll = z;
	}


	/*GETTERS*/

	//Returns the current X position
	float getX() {
		return transX;
	}

	//Returns the current Y position
	float getY() {
		return transY;
	}

	//Returns the current Z position
	float getZ() {
		return transZ;
	}


	//Deletes all buffer routines
	void cleanUp() {
		glDeleteBuffers(1, &ebo);
		glDeleteBuffers(1, &squareVbo);
		glDeleteBuffers(1, &imageVbo);
	}

/*PROTECTED BLOCK*/
protected:

};