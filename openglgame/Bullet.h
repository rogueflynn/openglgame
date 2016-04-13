/*******************************************************
		Bullet
	This class is used to store all the bullet info.

*******************************************************/

#pragma once 
#include<glew.h>
#include<freeglut.h>
#include "Sprite.h"

class Bullet : public Sprite {
public:
	bool isVisible, collided;
	int enemiesKilled;
	Bullet(){
		isVisible = false;	//Set the bullet to not be seen
	}
	Bullet(float x, float y, float z) {
		enemiesKilled = 0;
		isVisible = false;	//Set the bullet to not be seen
		collided = false;
		position.Set(x, y, z);
	} 
	void init() {
		setPoint(0.10f, 0.10f, 0.0f);
		//Calculate size of square
		size = ((point.X * 10 * 2) + 1) * scale.X;
	}

	//Load
	void Draw() {
		if(isVisible) {
			glPushMatrix();
				glColor3f(1.0f, 0.0f, 0.0f);									//Set the color of the object
				glTranslatef(position.X, position.Y, position.Z);
				glScalef(scale.X, scale.X, scale.Z);
				glBegin(GL_QUADS);
					glVertex2d(-point.X, -point.Y);
					glVertex2d(point.X, -point.Y);
					glVertex2d(point.X, point.Y);
					glVertex2d(-point.X, point.Y);
				glEnd();
			glPopMatrix();
		}
	}

	/*BOX EDGE BLOCK*/
	//Right edge
	float Right() {
		return (position.X + point.X/20);
	}
	//Left edge
	float Left() {
		return (position.X - point.X/20);
	}
	//Bottom edge
	float Bottom() {
		return (position.Y - point.X/20);
	}
	//Top edge
	float Top() {
		return (position.Y + point.X/20);
	}
	void moveBullet() {
		if(isVisible)
			position.Y += 0.03f;
		else
			position.X = 5.0f;
	}
};