/******************************************************************************
			Class: Point
			Author: Victor Gonzalez
			Purpose:
			Used as the data structure for a point on the cartesian.
			No math operations are supported in this class. Made more for
			readability.
*********************************************************************************/
#pragma once

class Point {
public:
	float X;
	float Y;
	float Z;

	/********************************************************
				Point default constructor
		Supports setting default values for the x, y and z 
		coordinates
	*********************************************************/
	Point() {
		this->X = 0.0f;
		this->Y = 0.0f;
		this->Z = 0.0f;
	}

	/********************************************************
				Point overloaded constructor
		Supports setting only x and y coordinates
	*********************************************************/
	Point(float X, float Y) {
		this->X = X;
		this->Y = Y;
		this->Z = 0.0f;
	}

	/********************************************************
				Point overloaded constructor
		Supports setting x, y and z coordinates
	*********************************************************/
	Point(float X, float Y, float Z) {
		this->X = X;
		this->Y = Y;
		this->Z = Z;
	}

	/********************************************************
				overloaded = 
		Used to assign the values of one point to another.
		Ex.  Point p1(1.0f, 1.0f);
			 Point p2(0.0f, 2.0f);
			 p1 = p2;
			 p1.X will equal 0.0f 
			 p2.Y will equal 2.0f
	*********************************************************/
	void operator=(const Point &right) {
		this->X = right.X;
		this->Y = right.Y;
		this->Z = right.Z;
	}
};