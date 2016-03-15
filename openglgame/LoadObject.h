/*********************************************************************************
*		LoadObject Class:
*		Author:		Class Provided via D2L
*		Editor:		Jordan Walsh	(adapted example class for this project)
*		Purpose:	Class loads and display an Wavefront '.OBJ' file using triangles 
*		and normals as static object. [Edits: The purpose of editing this class is 
*		to add the fuctionality of texture mapping to simple objects.]
*
*		User Notes:		OBJ file must be triangulated to load into openGL project. 
*		The OBJ model should be at the same location of this header file.
*
**********************************************************************************/

#pragma once
#include <windows.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <GL\SOIL.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include "shader.h"
#include "Vec.h"

#define POINTS_PER_VERTEX 3
#define TOTAL_FLOATS_IN_TRIANGLE 9

using namespace std;


class Model_OBJ
{
public:
	Model_OBJ();
	//~Model_OBJ();

	float* Model_OBJ::calculateNormal(float* coord1,float* coord2,float* coord3 );
    bool Model_OBJ::Load(char *filename);	// Loads the model
	void Model_OBJ::Draw();					// Draws the model on the screen
	void Model_OBJ::Release();				// Release the model

	float* normals;							// Stores the normals
	float* Faces_Triangles;					// Stores the triangles
	float* vertexBuffer;					// Stores the points which make the object
	long TotalConnectedPoints;				// Stores the total number of connected verteces
	long TotalConnectedTriangles;			// Stores the total number of connected triangles

private:

};

Model_OBJ::Model_OBJ()
{
	this->TotalConnectedTriangles = 0;
	this->TotalConnectedPoints = 0;
}

//Model_OBJ::~Model_OBJ()
//{
//}

inline float * Model_OBJ::calculateNormal(float * coord1, float * coord2, float * coord3)
{
	/* calculate Vector1 and Vector2 */
	float va[3], vb[3], vr[3], val;
	va[0] = coord1[0] - coord2[0];
	va[1] = coord1[1] - coord2[1];
	va[2] = coord1[2] - coord2[2];

	vb[0] = coord1[0] - coord3[0];
	vb[1] = coord1[1] - coord3[1];
	vb[2] = coord1[2] - coord3[2];

	/* cross product */
	vr[0] = va[1] * vb[2] - vb[1] * va[2];
	vr[1] = vb[0] * va[2] - va[0] * vb[2];
	vr[2] = va[0] * vb[1] - vb[0] * va[1];

	/* normalization factor */
	val = sqrt(vr[0] * vr[0] + vr[1] * vr[1] + vr[2] * vr[2]);

	float norm[3];
	norm[0] = vr[0] / val;
	norm[1] = vr[1] / val;
	norm[2] = vr[2] / val;


	return norm;
}

inline bool Model_OBJ::Load(char * filename)
{
	string line;
	ifstream objFile(filename);

	if (!objFile) {
		cout << "\nFile NOT FOUND:" << filename << endl;
		return false;
	}


	if (objFile.is_open())					// If obj file is open, continue
	{
		objFile.seekg(0, ios::end);			// Go to end of the file, 
		long fileSize = objFile.tellg();	// get file size
		objFile.seekg(0, ios::beg);			// we'll use this to register memory for our 3d model

		vertexBuffer = (float*)malloc(fileSize);					// Allocate memory for the verteces
		Faces_Triangles = (float*)malloc(fileSize*sizeof(float));	// Allocate memory for the triangles
		normals = (float*)malloc(fileSize*sizeof(float));			// Allocate memory for the normals

		int triangle_index = 0;				// Set triangle index to zero
		int normal_index = 0;				// Set normal index to zero

		/* Start reading data lines stored in OBJ file */
		while (!objFile.eof())
		{
			getline(objFile, line);			// Get line from file

			/* The first character is a v: on this line is a vertex stored. */
			if (line.c_str()[0] == 'v')	
			{
				line[0] = ' ';				// Set first character to 0. This will allow us to use sscanf

				sscanf(line.c_str(), "%f %f %f ",					// Read floats from the line: v X Y Z
						&vertexBuffer[TotalConnectedPoints],
						&vertexBuffer[TotalConnectedPoints + 1],
						&vertexBuffer[TotalConnectedPoints + 2]);

				TotalConnectedPoints += POINTS_PER_VERTEX;			// Add 3 to the total connected points
			}

			/* The first character is an 'f': on this line is a point stored */
			if (line.c_str()[0] == 'f')	
			{
				line[0] = ' ';				// Set first character to 0. This will allow us to use sscanf

				int vertexNumber[4] = { 0, 0, 0 };
				sscanf(line.c_str(), "%i%i%i",					// Read integers from the line:  f 1 2 3
						&vertexNumber[0],						// First point of our triangle. 
						&vertexNumber[1],						// This is a pointer to our vertexBuffer list,
						&vertexNumber[2]);						// each point represents an X,Y,Z.

				vertexNumber[0] -= 1;							// OBJ file starts counting from 1
				vertexNumber[1] -= 1;							// OBJ file starts counting from 1
				vertexNumber[2] -= 1;							// OBJ file starts counting from 1


				/********************************************************************
				* Create triangles (f 1 2 3) from points: (v X Y Z) (v X Y Z) (v X Y Z).
				* The vertexBuffer contains all verteces
				* The triangles will be created using the verteces we read previously
				*/
				int tCounter = 0;
				for (int i = 0; i < POINTS_PER_VERTEX; i++)
				{
					Faces_Triangles[triangle_index + tCounter] = vertexBuffer[3 * vertexNumber[i]];
					Faces_Triangles[triangle_index + tCounter + 1] = vertexBuffer[3 * vertexNumber[i] + 1];
					Faces_Triangles[triangle_index + tCounter + 2] = vertexBuffer[3 * vertexNumber[i] + 2];
					tCounter += POINTS_PER_VERTEX;
				}


				/*********************************************************************
				* Calculate all normals, used for lighting
				*/
				float coord1[3] = { Faces_Triangles[triangle_index], Faces_Triangles[triangle_index + 1],Faces_Triangles[triangle_index + 2] };
				float coord2[3] = { Faces_Triangles[triangle_index + 3],Faces_Triangles[triangle_index + 4],Faces_Triangles[triangle_index + 5] };
				float coord3[3] = { Faces_Triangles[triangle_index + 6],Faces_Triangles[triangle_index + 7],Faces_Triangles[triangle_index + 8] };
				float *norm = this->calculateNormal(coord1, coord2, coord3);

				tCounter = 0;
				for (int i = 0; i < POINTS_PER_VERTEX; i++)
				{
					normals[normal_index + tCounter] = norm[0];
					normals[normal_index + tCounter + 1] = norm[1];
					normals[normal_index + tCounter + 2] = norm[2];
					tCounter += POINTS_PER_VERTEX;
				}

				triangle_index += TOTAL_FLOATS_IN_TRIANGLE;
				normal_index += TOTAL_FLOATS_IN_TRIANGLE;
				TotalConnectedTriangles += TOTAL_FLOATS_IN_TRIANGLE;
			}
		}
		objFile.close();	//Close OBJ file
		return true;
	}
	else
	{
		cout << "Unable to open file";
	}
	return false;
}

inline void Model_OBJ::Draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);						// Enable vertex arrays
	glEnableClientState(GL_NORMAL_ARRAY);						// Enable normal arrays
	glVertexPointer(3, GL_FLOAT, 0, Faces_Triangles);			// Vertex Pointer to triangle array
	glNormalPointer(GL_FLOAT, 0, normals);						// Normal pointer to normal array
	glDrawArrays(GL_TRIANGLES, 0, TotalConnectedTriangles);		// Draw the triangles
	glDisableClientState(GL_VERTEX_ARRAY);						// Disable vertex arrays
	glDisableClientState(GL_NORMAL_ARRAY);						// Disable normal arrays
}

inline void Model_OBJ::Release()
{
	free(this->Faces_Triangles);
	free(this->normals);
	free(this->vertexBuffer);
}



/*********************************************************************************
	//Example Main that loads OBJ into a openGL C++ project:


	int main(int argc, char **argv) 
	{
		// set window values
		win.width = 1200;
		win.height = 800;
		win.title = "OpenGL/GLUT OBJ Loader.";
		win.field_of_view_angle = 45;
		win.z_near = 1.0f;
		win.z_far = 500.0f;
 
		// initialize and run program
		glutInit(&argc, argv);                                      
		glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );  
		glutInitWindowSize(win.width,win.height);					
		glutCreateWindow(win.title);								
		glutDisplayFunc(display);							// register Display Function
		glutIdleFunc( display );							// register Idle Function
		glutKeyboardFunc( keyboard );						// register Keyboard Handler
		initialize();

		//prompt for a file to load									
		char filename[200] = "monkey.obj";		// <! enter EXACT file name and extension of OBJ file here !>
		//cout<<"\nEnter onject file name:";						
		//cin>>filename;									// for debugging wrong file search

		if(obj.Load(filename)){
			glutMainLoop();									// run GLUT mainloop
		}
		return 0;
	}

**********************************************************************************/
