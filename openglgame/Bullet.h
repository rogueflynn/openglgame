/*******************************************************
		Bullet
	This class is used to store all the bullet info.

*******************************************************/

#pragma once 
#include<gl/glew.h>
#include<gl/freeglut.h>
#include "Point.h"
#include "Vec.h"
class Bullet {
public:
	float  x;
	float  y;
	Vec position;
	Point point;
	float velocity;
	bool isVisible;

	Bullet() {
		isVisible = false;	//Set the bullet to not be seen
		position.Set(0.0f, 0.0f, 0.0f);
		point.setPoint(0.10f, 0.10f, 0.0f);
	} 

	//Load
	void Draw() {
		if(isVisible) {
			glPushMatrix();
				glColor3f(1.0f, 0.0f, 0.0f);									//Set the color of the object
				glTranslatef(position.X, position.Y, position.Z);
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
};