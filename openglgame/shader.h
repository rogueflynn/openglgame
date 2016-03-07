/***********************************************************************************
		Shader Class
		Author: Victor Gonzalez
		Purpose:
			This class is used to load the vertex and fragment shader.  The class 
			uses a singleton design pattern so that only one instance of the shader
			object exists in memory.  This is done because these shaders are shared
			between objects and it can become expensive to have to keep reloading 
			and instantiating shader.  The class also makes sure that the shader load
			only happens one time. The reason for this is the same as the reason
			for having a singleton.

**************************************************************************************/
#pragma once
#include<glew.h>
#include<iostream>
#include<freeglut.h>
#include<string>
#include<fstream>

class Shader {
private:
	static bool instanceFlag;					//Keeps track if there is an instance in memory
	static Shader *shader;						//The memory address of the shader object
	Shader() {}									//Empty to constructor.
public:
	//Variables
	GLuint shaderProgram;						//Stores the location of the shader program
	GLuint vertexShaderID;						//THe storage location for the vertex shader
	GLuint fragmentShaderID;					//The storage location for the fragment shader
	const GLchar* adapter[1];					
	std::string shaderFile;							//Used to store the path the shader files
	//Handles instances
	static Shader *getInstance();
	~Shader() {
		instanceFlag = false;
	}
		
	/*************************************************************
						readShaderCode()
				Reads in the fragment and vertex shader using
				fstream.
	**************************************************************/
	std::string readShaderCode(const char* fileName) {
		std::ifstream meInput(fileName);
		if(!meInput.good()) {
			std::cout << " File failed to load.." << fileName << std::endl;
		}
		return std::string(
			std::istreambuf_iterator<char>(meInput),
			std::istreambuf_iterator<char>());
	}

	/*************************************************************
						createShaders()
				Uses the readShaderCode function in order to 
				load in the shaders and then instatiate and link
				them to the min program.
	**************************************************************/
	void createShaders() {

			//If the size of the temporary string buffer is 0
			//The load the shaders into the program. If it's
			//not zero, then don't load them into the program.
			if(shaderFile.size() == 0) {
				vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
				fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

				//Setter function
				shaderFile = readShaderCode("shaders/vertexsource.glsl"); 
				adapter[0] = shaderFile.c_str();
				glShaderSource(vertexShaderID, 1, adapter, 0);
				shaderFile = readShaderCode("shaders/fragmentsource.glsl"); 
				adapter[0] = shaderFile.c_str();
				glShaderSource(fragmentShaderID, 1, adapter, 0);

				glCompileShader(vertexShaderID);
				glCompileShader(fragmentShaderID);

				shaderProgram = glCreateProgram();
				glAttachShader(shaderProgram, vertexShaderID);
				glAttachShader(shaderProgram, fragmentShaderID);
				glLinkProgram(shaderProgram);

				glDeleteShader(vertexShaderID);
				glDeleteShader(fragmentShaderID);

				glUseProgram(shaderProgram);
			} 
		}
		
	protected:
	};

bool Shader::instanceFlag = false;
Shader *Shader::shader = NULL;

/*************************************************************
				getInstance()
	The getInstance method checks to see if there is 
	an active instance of a shader object in memory. If there
	is an active instance, then the class will just return 
	the currently actively object. If there is not an active
	instance, then the class will instantiate a new shader
	object.

**************************************************************/
Shader *Shader::getInstance() {
	if(!instanceFlag) {
		shader = new Shader();
		instanceFlag = true;
		return shader;
	} else {
		return shader;
	}
}