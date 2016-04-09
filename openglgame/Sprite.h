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
#include "Point.h"
#include "Model_OBJ.h"

class Sprite {

/*PRIVATE BLOCK*/
protected:
	Vec scale, position, color,rotation;		//Store scale, position, color and rotation values
	Point point;
	float size;									//stores the size of the scale
	float speed;								//Stores the speed of the player
	float offset;
	Model_OBJ model;

/*PUBLIC BLOCK*/
public:
	/*VIRTUAL METHODS*/
	//Have to override
	virtual void Draw(){}
	virtual void init(){}

	/***********************************************************
				Spirte constructor
			The sprite constructor sets all of the default 
			values for the class attributes.

	**************************************************************/
	Sprite() {
			//used to store buffers and vertex data
			scale.Set(1.0f, 1.0f, 1.0f);						//Set scale
			color.setColor(1.0f, 0.0f, 0.0f);					//sets the color
			speed = 0.01f;									//Sets the speed
			offset = 0.05f;
			point.X = point.Y = 0.3f;
	}
	/************************************************************
					Sprite Destructor
			Destroyer all dynamic memory instatiated 
			int this class
	*************************************************************/
	~Sprite() {
	}


	 //Used to load the image into a texture
	GLuint loadAndBufferImage(const char* imageName) {
		int w, h;		//Width and height
		/* load an image file directly as a new OpenGL texture */
		GLuint textureId;
		unsigned char* image = SOIL_load_image(imageName, &w, &h, 0, SOIL_LOAD_RGBA);
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		SOIL_free_image_data(image);

		return textureId;
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
	void moveRight(float deltaTime) {
		position.X +=  (speed * (float) deltaTime);
		model.posX +=  (speed * (float) deltaTime);
	}

	//Move left
	void moveLeft(float deltaTime) {
		position.X -= speed * (float) deltaTime;
		model.posX -= speed * (float) deltaTime;
	}

	//Move up
	void moveUp(float deltaTime) {
		position.Y += speed * (float) deltaTime;
		model.posY += speed * (float) deltaTime;
	}

	//Move down
	void moveDown(float deltaTime) {
		position.Y -= speed * (float) deltaTime;
		model.posY -= speed * (float) deltaTime;;
	}

	/*STOP MOVEMENT BLOCK*/

	//Stop moving left
	void stopMovingLeft(float loc) {
		position.X = loc+offset;
		model.posX = loc+offset;
	}

	//Stop moving right 
	void stopMovingRight(float loc) {
		position.X = loc-offset;
		model.posX = loc-offset;
	}

	//Stop moving up 
	void stopMovingUp(float loc) {
		position.Y = loc-offset;
		model.posY = loc-offset - 0.5f;
	}

	//Stop moving down 
	void stopMovingDown(float loc) {
		position.Y = loc+offset;
		model.posY = loc+offset - 0.5f;
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

	void setPoint(float X, float Y, float Z) {
		point.setPoint(X, Y, Z);
	}


/*PROTECTED BLOCK*/
private:
};