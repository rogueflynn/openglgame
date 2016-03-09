/******************************************************************************
			Class: Vec
			Author: Victor Gonzalez
			Purpose:
			Used as the data structure for a vector on the cartesian plane.
			Math (vector)operations are supported in this class. Will also 
			keep track of angle (direction).
			If no overloaded constructor is called, all values will default
			to zero.
*********************************************************************************/
#pragma once

class Vec {
public:
	float X, Y, Z;				//Used to store coordinates
	float R, G, B;				//Used to store color info.
	float Roll, Yaw, Pitch;		//Used to store rotation info.

	/********************************************************
				Vec default constructor
		Supports setting default values for the x, y and z 
		coordinates
	*********************************************************/
	Vec() {
		this->X = 0.0f;
		this->Y = 0.0f;
		this->Z = 0.0f;
		this->R = 0.0f;
		this->G = 0.0f;
		this->B = 0.0f;
		this->Roll = 0.0f;
		this->Yaw = 0.0f;
		this->Pitch = 0.0f;
	}

	/********************************************************
				Vec overloaded constructor
		Supports setting only x and y coordinates
	*********************************************************/
	Vec(float X, float Y) {
		this->X = X;
		this->Y = Y;
		this->Z = 0.0f;
		this->R = 0.0f;
		this->G = 0.0f;
		this->B = 0.0f;
		this->Roll = 0.0f;
		this->Yaw = 0.0f;
		this->Pitch = 0.0f;
	}

	/********************************************************
				set overloaded constructor
		Supports setting x, y and z coordinates
	*********************************************************/
	Vec(float X, float Y, float Z) {
		this->X = X;
		this->Y = Y;
		this->Z = Z;
		this->R = 0.0f;
		this->G = 0.0f;
		this->B = 0.0f;
		this->Roll = 0.0f;
		this->Yaw = 0.0f;
		this->Pitch = 0.0f;
	}
	/********************************************************
	

	/********************************************************
					set()	
		Supports setting only x and y coordinates
	*********************************************************/
	void Set(float X, float Y) {
		this->X = X;
		this->Y = Y;
		this->Z = 0.0f;
	}

	/********************************************************
				set
		Supports setting x, y and z coordinates
	*********************************************************/
	void Set(float X, float Y, float Z) {
		this->X = X;
		this->Y = Y;
		this->Z = Z;
	}
	/********************************************************
					setColor
		Supports setting R, G and B values.
		Used just to aid in readability. 
	*********************************************************/
	void setColor(float R, float G, float B) {
		this->R = R;
		this->G= G;
		this->B = B;
	}
	/********************************************************
					setRotation
		Supports setting Roll, Yaw and Pitch values.
		Used just to aid in readability. 
	*********************************************************/
	void setRotation(float Roll, float Yaw, float Pitch) {
		this->Roll = Roll;
		this->Yaw= Yaw;
		this->Pitch = Pitch;
	}

	/**********************************************************
				OVERLOADED OPERATORS BLOCK
	***********************************************************/

	/********************************************************
				overloaded = 
		Used to assign the values of one point to another.
		Ex.  Point p1(1.0f, 1.0f);
			 Point p2(0.0f, 2.0f);
			 p1 = p2;
			 p1.X will equal 0.0f 
			 p2.Y will equal 2.0f
	*********************************************************/
	Vec operator=(const Vec &right) {
		this->X = right.X;
		this->Y = right.Y;
		this->Z = right.Z;
		return *this;
	}

	/********************************************************
				overloaded + 
		Used to add the values of one point to another.
		Ex.  Point p1(1.0f, 1.0f);
			 Point p2(0.0f, 2.0f);
			 p1 = p1 + p2;
			 p1.X will equal 2.0f 
			 p2.Y will equal 3.0f
	*********************************************************/
	Vec operator+(const Vec &right) {
		this->X = this->X + right.X;
		this->Y = this->Y + right.Y;
		this->Z = this->Z + right.Z;
		return *this;
	}
	/********************************************************
				overloaded - 
		Used to subtract the values of one point to another.
		Ex.  Point p1(1.0f, 1.0f);
			 Point p2(0.0f, 2.0f);
			 p1 = p1 - p2;
			 p1.X will equal 1.0f 
			 p2.Y will equal -1.0f
	*********************************************************/
	Vec operator-(const Vec &right) {
		this->X = this->X - right.X;
		this->Y = this->Y - right.Y;
		this->Z = this->Z - right.Z;
		return *this;
	}

};