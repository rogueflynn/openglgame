#include "Sound.h"

Sound::Sound()
{

}

Sound::~Sound()
{
	alDeleteSources(2, sources);
	alDeleteBuffers(2, buffers);
	alcDestroyContext(context);
	alcCloseDevice(device);
}

/* load a file into memory, returning the buffer and
* setting bufsize to the size-in-bytes */
void* Sound::load(char *fname, long *bufsize)
{
	FILE* fp = fopen(fname, "rb");
	fseek(fp, 0L, SEEK_END);
	long len = ftell(fp);
	rewind(fp);
	void *buf = malloc(len);
	fread(buf, 1, len, fp);
	fclose(fp);
	*bufsize = len;
	return buf;
}

bool Sound::init(std::string filename,  std::string shotFilename)
{
	
	float curr[3] = { 0., 0., -1. };
	float targ[3] = { 0., 0., -1. };

	/* initialize OpenAL context, asking for 44.1kHz to match HRIR data */
	device = alcOpenDevice(NULL);
	if (device == NULL)
		return false;

	context = alcCreateContext(device, NULL);
	alcMakeContextCurrent(context);

	alGetError();

	/* listener at origin, facing down -z (ears at 1.5m height) */
	alListener3f(AL_POSITION, 0., 1.5, 0.);
	alListener3f(AL_VELOCITY, 0., 0., 0.);
	float orient[6] = { /*fwd:*/ 0., 0., -1., /*up:*/ 0., 1., 0. };
	alListenerfv(AL_ORIENTATION, orient);

	/* this will be the source of ghostly footsteps... */
	sources = new ALuint[2];
	alGenSources(2, sources);
	alSource3f(sources[0], AL_POSITION, curr[0], curr[1], curr[2]);
	alSource3f(sources[1], AL_POSITION, curr[0], curr[1], curr[2]);

	alSourcei(sources[0], AL_LOOPING, AL_TRUE);
	alSourcei(sources[1], AL_LOOPING, AL_FALSE);

	buffers = new ALuint[2];
	alGenBuffers(2, buffers);
	{
		long dataSize;
		const ALvoid* data = load((char *)filename.c_str(), &dataSize);
		/* for simplicity, assume raw file is signed-16b at 44.1kHz */
		alBufferData(buffers[0], AL_FORMAT_MONO16, data, dataSize, 44100);
		free((void*)data);

		data = load((char *)shotFilename.c_str(), &dataSize);
		alBufferData(buffers[1], AL_FORMAT_MONO16, data, dataSize, 44100);
		free((void*)data);
	}
	alSourcei(sources[0], AL_BUFFER, buffers[0]);
	alSourcei(sources[1], AL_BUFFER, buffers[1]);

	return true;
}

void Sound::playOpening()
{
	//play
	alSourcePlay(sources[0]);
}
void Sound::stopOpening() {
	alSourceStop(sources[0]);	
}
void Sound::playShot()
{
	//play
	alSourcePlay(sources[1]);
}

void Sound::stopShot()
{
	//to stop
	alSourceStop(sources[1]);
}
