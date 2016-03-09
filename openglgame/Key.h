/**********************************************************
	class Key
	Author: Victor Gonzalez
	Purpose:
		Helper class used to process key presses. Promotes
		loosely decoupled software

************************************************************/
#include<Windows.h>
#pragma once

class Key {
protected:
public:
	//Left arrow key pressed
	bool Left() {
		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
			return true;
		else 
			return false;
	}
	//Right arrow key pressed
	bool Right() {
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
			return true;
		else 
			return false;
	}
	//Up arrow key pressed
	bool Up() {
		if (GetAsyncKeyState(VK_UP) & 0x8000)
			return true;
		else 
			return false;
	}
	//Down arrow key pressed
	bool Down() {
		if (GetAsyncKeyState(VK_DOWN) & 0x8000)
			return true;
		else 
			return false;
	}	
private:
};