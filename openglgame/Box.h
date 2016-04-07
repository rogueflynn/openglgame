/*********************************************************************************
		Box class:
		Author: Victor Gonzalez
		Purpose:
			This class is a helper class for hit boxes.  It is used to check
			for collision detection between hitboxes.  It is also used  to 
			determined the edges of a box and if there has been a collision 
			with either the left, right, top or bottom edge.  
**********************************************************************************/
#pragma once
#include "Sprite.h"
#include "Bullet.h"

class Box {
private:
	float hd, vd;		//Used to calculate the horizontal and vertical distance
	
	//Calculates the distance between points
	void calculateDistance(Sprite &boxOne, Sprite &boxTwo) {
		//Distance between x values |x1 - x2|
		//Distance between y values |y1 - y2|
		hd = abs(boxOne.getX() - boxTwo.getX());			//Gets the horizontal distance
		vd = abs(boxOne.getY() - boxTwo.getY());			//Gets the vertical distance
	}
public:

	/****************************************************
					intersect()
		Uses the axis-aligned bounding box in order to 
		perform collision detection. Determines the 
		intersection by seeing if either the right edge
		of one box has touched the left edge of another
		and so forth.
	*****************************************************/
	bool intersect(Sprite &boxOne, Sprite &boxTwo) {
		return !(boxOne.Right() <= boxTwo.Left()  ||
				 boxOne.Left() >= boxTwo.Right()  ||
				 boxOne.Top()  <= boxTwo.Bottom() ||
				 boxOne.Bottom() >= boxTwo.Top()); 
	}

	//Checks enemy intersection with bullet
	bool bulletIntersect(Bullet &boxOne, Sprite &boxTwo) {
		return !(boxOne.Right() <= boxTwo.Left()  ||
				 boxOne.Left() >= boxTwo.Right()  ||
				 boxOne.Top()  <= boxTwo.Bottom() ||
				 boxOne.Bottom() >= boxTwo.Top()); 
	}

	//Box helper functions
	//Used to determine if the character is colliding with a tile

	//Player is on top of a tile
	bool touchTop(Sprite &boxOne, Sprite &boxTwo) {
		calculateDistance(boxOne, boxTwo);
		return (intersect(boxOne, boxTwo) && boxOne.getY() > boxTwo.getY() && vd > hd);
	}

	//Player is underneath a tile
	bool touchBottom(Sprite &boxOne, Sprite &boxTwo) {
		calculateDistance(boxOne, boxTwo);
		return (intersect(boxOne, boxTwo) && boxOne.getY() < boxTwo.getY() && vd > hd);
	}

	//Player has touched the left side of a tile
	bool touchLeft(Sprite &boxOne, Sprite &boxTwo) {
		calculateDistance(boxOne, boxTwo);
		return (intersect(boxOne,boxTwo) && boxOne.getX() < boxTwo.getX() && hd > vd);
	}

	//Player has touched the right side of a tile
	bool touchRight(Sprite &boxOne, Sprite &boxTwo) {
		calculateDistance(boxOne, boxTwo);
		return (intersect(boxOne, boxTwo) && boxOne.getX() > boxTwo.getX() && hd > vd);
	}

protected:
};
