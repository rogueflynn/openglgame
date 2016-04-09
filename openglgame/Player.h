/*********************************************************************************
		Player class:
		Author: Victor Gonzalez
		Purpose:
			Serves as the template for all player objects.  Stores all attributes
			for the player and handles the current status of the player.

**********************************************************************************/
#pragma once
#include "Sprite.h"
#include "Bullet.h"
#include "Box.h"
class Player : public Sprite {

/*PRIVATE BLOCK*/
private:
	Box box;
	GLuint textureBufferID;
/*PUBLIC BLOCK*/
public:
	std::vector<Bullet> bullets;

	~Player() {
		std::cout << "Destructor called.\n";
	}
	/**************************************************
					initialize the player
	***************************************************/
	void init() {
		textureBufferID = loadAndBufferImage("hitbox.png");
		setPoint(0.25f, 0.25f, 0.0f);
		
		//Calculate size of square
		size = ((point.X * 10 * 2) + 1) * scale.X;
		position.Set(0.0f, -3.5f,0.0f);
		model.Load("ship.obj");
	}

	/**********************************************
		The draw function is used to draw objects
		to the screen.
	************************************************/
	void Draw() {
		model.drawObject();
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

		//Draw The bullets
		for(unsigned int i = 0; i < bullets.size(); i++) {
			bullets[i].Draw();
		}

		updateBullets();
	}

//This method instatiates a new bullet and adds
//it to the vector. The max that the vector can hava
//is 5.
void shoot() {
	Bullet bullet;					//Instantiate a new bullet
	bullet.isVisible = true;		//Set the bullet to be seen
	bullet.position.X = position.X;				//Set the position
	bullet.position.Y = position.Y+0.7f;

	if(bullets.size() < 5)			//If there is not 5 bullets on the screen, the player can shoot.
		bullets.push_back(bullet);	//Add a bullet to the vector

}

//This function is used to move the bullets across the screen
//If the bullet is outside the boundary of the game world
//the bullet will be erased from the vector.
void updateBullets() {
		//Moves the bullet across the screen
		for(unsigned int i = 0; i < bullets.size(); i++) {
			bullets[i].position.Y += 0.04f;	//Increment each bullet
			if(bullets[i].position.Y > 4)		//If the bullet is outside the boundary
				bullets[i].isVisible = false;		//Set to false
			glutPostRedisplay();
		}
		
		//If the bullet is not visible than erase from the vector 
		//Might be able to refactor
		for(unsigned int i = 0; i < bullets.size(); i++) {
			if(!(bullets[i].isVisible)) {				//If the bullet is not visible
				bullets.erase(bullets.begin() + i);		//Erase the bullet
			}
		}

}		

//Checks to see if the bullet has collided with an enemy.
bool bulletCollision(Sprite &enemy) {
	for(unsigned int i = 0; i < bullets.size(); i++) {
		if(box.bulletIntersect(bullets[i], enemy)) {
			return true;
		}
	}	
	return false;
}



/*Protected Block*/
protected:

};