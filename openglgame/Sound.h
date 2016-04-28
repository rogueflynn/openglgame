/*********************************************************************************
Sound class:
Author: Dennis Chime
Purpose:
Manage the sound
**********************************************************************************/
#pragma once
#include "AL\al.h"
#include "AL\alc.h"
#include "AL\alext.h"
#include "AL\alut.h"


#pragma comment(lib, "OpenAL32.lib")

#include <stdlib.h>
#include <string>

class Sound
{
private:
	ALCcontext *context;
	ALCdevice *device;
	ALuint* sources;
	ALuint* buffers;
	void* load(char *fname, long *bufsize);
public:
	Sound();
	~Sound();
	bool init(std::string filename, std::string shotFilename);
	void playOpening();
	void stopOpening();
	void playShot();
	void stopShot();
};
