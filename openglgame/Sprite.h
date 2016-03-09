/*********************************************************************************
		Sprite class:
		Author: Victor Gonzalez
		Purpose:
			Serves as the base class for all sprite bjects.  Stores all attributes
			for the sprite and handles the current status of the sprite. There are
			virtual functions that have to be overriden in order for another class
			to inherit from this class.
**********************************************************************************/
#pragma once
#include<glew.h>
#include<freeglut.h>
#include<iostream>
#include<string>
#include<fstream>
#include<SOIL.h>
#include "shader.h"
#include "Vec.h"

class Sprite {

/*PRIVATE BLOCK*/
protected:
	Vec scale, position, color,rotation;		//Store scale, position, color and rotation values
	GLint Scale, Position, Rotation;			//Uniform variables passed into the shaders
	GLint uniformColor;							//Uniform color variable
	GLint colAttrib, posAttrib, texAttrib;		//The color, position and texture attributes
	GLuint playerVbo, ebo, imageVbo;			//Stores vector buffer objects
	Shader *shader;								//Points to the single instance of the shader object
	float size;									//stores the size of the scale
	float speed;								//Stores the speed of the player
	float offset;
/*PUBLIC BLOCK*/
public:
	/*VIRTUAL METHODS*/
	//Have to override
	virtual void Draw(){}
	virtual void init(){}
	virtual void drawImage(){}

	/***********************************************************
				Spirte constructor
			The sprite constructor sets all of the default 
			values for the class attributes.

	**************************************************************/
	Sprite() {
			//used to store buffers and vertex data
			scale.Set(0.1f, 0.1f, 0.1f);						//Set scale
			color.setColor(1.0f, 0.0f, 0.0f);					//sets the color
			speed = 0.00005f;									//Sets the speed
			offset = 0.05f;
			shader = Shader::getInstance();
	}
	/************************************************************
					Sprite Destructor
			Destroyer all dynamic memory instatiated 
			int this class
	*************************************************************/
	~Sprite() {
		delete shader;
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
		Scale			=	glGetUniformLocation(shader->shaderProgram, "s");
		Position		=	glGetUniformLocation(shader->shaderProgram, "t");
		Rotation		=	glGetUniformLocation(shader->shaderProgram, "r");
		uniformColor	=	glGetUniformLocation(shader->shaderProgram, "ucolor");
	}

	

	/*BOX EDGE BLOCK*/
	//Right edge
	float Right() {
		return (getX() + getSize()/20);
	}
	//Left edge
	float Left() {
		return (getX() - getSize()/20);
	}
	//Bottom edge
	float Bottom() {
		return (getY() - getSize()/20);
	}
	//Top edge
	float Top() {
		return (getY() + getSize()/20);
	}

	/***********************************************
				MOVEMENT BLOCK
			RIGHT, LEFT, UP, and DOWN
	************************************************/
	//Move right
	void moveRight() {
		position.X += speed;
	}

	//Move left
	void moveLeft() {
		position.X -= speed;
	}

	//Move up
	void moveUp() {
		position.Y += speed;
	}

	//Move down
	void moveDown() {
		position.Y -= speed;
	}

	/*STOP MOVEMENT BLOCK*/

	//Stop moving left
	void stopMovingLeft(float loc) {
		position.X = loc+offset;
	}

	//Stop moving right 
	void stopMovingRight(float loc) {
		position.X = loc-offset;
	}

	//Stop moving up 
	void stopMovingUp(float loc) {
		position.Y = loc-offset;
	}

	//Stop moving down 
	void stopMovingDown(float loc) {
		position.Y = loc+offset;
	}

	//Speeds the speed the player moves
	void setSpeed(float speed) {
		this->speed = speed;	
	}

	/*Sets the color of an object*/
	void setColor(float red, float green, float blue) {
		color.setColor(red, green, blue);
	}

	/*MATRIX OPERTATIONS*/

	//Performs translation on the x and y access
	void translate(float x, float y) {
		position.X = x;
		position.Y = y;
	}
	

	//Controls the scale 
	void ScaleVector(float x, float y) {
		scale.X = x;
		scale.Y = y;
	}
	
	//Controls rotation along the x-axis
	void rotateX(float x) {
		rotation.Pitch = x;
	}

	//Controls rotation along the y-axis
	void rotateY(float y) {
		rotation.Yaw = y;
	}

	//Controls rotation along the z-axis
	void rotateZ(float z) {
		rotation.Roll = z;
	}

	/*GETTERS*/

	//Returns the current X position
	float getX() {
		return position.X;
	}

	//Returns the current Y position
	float getY() {
		return position.Y;
	}

	//Returns the current Z position
	float getZ() {
		return position.Z;
	}

	float getSize() {
		return size;
	}

	//Deletes all buffer routines
	void cleanUp() {
		glDeleteProgram(shader->shaderProgram);
		glDeleteProgram(shader->vertexShaderID);
		glDeleteProgram(shader->fragmentShaderID);

		glDeleteBuffers(1, &ebo);
		glDeleteBuffers(1, &playerVbo);
		glDeleteBuffers(1, &imageVbo);
		std::cout << "Clean Up called\n";
	}

/*PROTECTED BLOCK*/
private:
};