#include <vector>
#include <string>
#include "EngineObjects.hpp"
#include "utils.hpp"
#include "AudioEngine.hpp"

#ifndef __APPLE__
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
#include "../extern/stb/stb_vorbis.c"

extern void stb_vorbis_dumpmem(void);
#else //just for debug purpose, should never be loaded
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <AL/alut.h>
#endif

#include <iostream>
#include <thread>

bool check_alc_errors(const std::string &filename, const std::uint_fast32_t line, ALCdevice *device)
{
  ALCenum error = alcGetError(device);
  if (error != ALC_NO_ERROR)
  {
    std::cerr << "***ERROR*** (" << filename << ": " << line << ")\n";
    switch (error)
    {
    case ALC_INVALID_VALUE:
      std::cerr << "ALC_INVALID_VALUE: an invalid value was passed to an OpenAL function";
      break;
    case ALC_INVALID_DEVICE:
      std::cerr << "ALC_INVALID_DEVICE: a bad device was passed to an OpenAL function";
      break;
    case ALC_INVALID_CONTEXT:
      std::cerr << "ALC_INVALID_CONTEXT: a bad context was passed to an OpenAL function";
      break;
    case ALC_INVALID_ENUM:
      std::cerr << "ALC_INVALID_ENUM: an unknown enum value was passed to an OpenAL function";
      break;
    case ALC_OUT_OF_MEMORY:
      std::cerr << "ALC_OUT_OF_MEMORY: an unknown enum value was passed to an OpenAL function";
      break;
    default:
      std::cerr << "UNKNOWN ALC ERROR: " << error;
    }
    std::cerr << std::endl;
    return false;
  }
  return true;
}

namespace kNgine
{
  ALCcontext *openALContext;
  ALCdevice *openALDevice;

  class OpenALBuffer:public BaseAudioBuffer
  {
  public:
    ALuint buffer;
    OpenALBuffer(const char* fileName,audiofiletype type)
    {
      if(type==audiofiletype::wav){
        buffer = alutCreateBufferFromFile(fileName);
        if (!check_alc_errors(__FILE__, __LINE__, openALDevice) || buffer == AL_NONE)
        {
          std::cerr << "ERROR: Could not load file" << std::endl;
        }
      }else if(type=audiofiletype::ogg){
        int chan, samplerate;
        short *output;
        int samples = stb_vorbis_decode_filename(fileName, &chan, &samplerate, &output);
        alGenBuffers(1, &buffer);
        alBufferData(buffer,AL_FORMAT_MONO16,output,samples*sizeof(short),samplerate);
        if (!check_alc_errors(__FILE__, __LINE__, openALDevice) || buffer == AL_NONE)
        {
          std::cerr << "ERROR: Could not load file" << std::endl;
        }
      }
    }
    ~OpenALBuffer(){
      alDeleteBuffers(1, &buffer);
    }
  };

  SoundPlayerComponent::SoundPlayerComponent(GameObject *object) : ObjectComponent(object)
  {
    if(!openALDevice){
      alutInit(nullptr, nullptr);
      openALDevice = alcOpenDevice(nullptr);
      if (!openALDevice)
      {
        std::cerr << "error" << std::endl;
      }
      openALContext = alcCreateContext(openALDevice, nullptr);
      if (!check_alc_errors(__FILE__, __LINE__, openALDevice))
      {
        std::cerr << "ERROR: Could not create audio context" << std::endl;
      }
      ALCboolean contextMadeCurrent = alcMakeContextCurrent(openALContext);
      if (!check_alc_errors(__FILE__, __LINE__, openALDevice) || contextMadeCurrent != ALC_TRUE)
      {
        std::cerr << "ERROR: Could not make audio context current" << std::endl;
      }
    }
  }
  SoundPlayerComponent::~SoundPlayerComponent(){
    alutExit();
    alcMakeContextCurrent(nullptr);
    if (!check_alc_errors(__FILE__, __LINE__, openALDevice))
    {
    }
    alcDestroyContext(openALContext);
    if (!check_alc_errors(__FILE__, __LINE__, openALDevice))
    {
    }
    alcCloseDevice(openALDevice);
    if (!check_alc_errors(__FILE__, __LINE__, openALDevice))
    {
    }
  }

  SoundEmiterComponent::SoundEmiterComponent(GameObject *object, const char *fileName, audiofiletype type, SoundPlayerComponent *player) : SoundEmiterComponent(object, createBuffer(fileName,type), player)
  {
  }
  SoundEmiterComponent::SoundEmiterComponent(GameObject *object, BaseAudioBuffer *buffer, SoundPlayerComponent *player) : ObjectComponent(object)
  {
    this->buffer = buffer;
    this->player = player;
    job.stop();
    job = threaded_job([this](){
      ALuint source;
      // store volume in player obj
      alGenSources(1, &source);
      alSourcef(source, AL_PITCH, 1);
      alSourcef(source, AL_GAIN, 1.0f);
      //change to set with player pos and current pos
      alSource3f(source, AL_POSITION, 0, 0, 0);
      //nope nope nope
      alSource3f(source, AL_VELOCITY, 0, 0, 0);
      alSourcei(source, AL_LOOPING, AL_FALSE);
      alSourcei(source, AL_BUFFER, ((OpenALBuffer*)this->buffer)->buffer);
      if (!check_alc_errors(__FILE__, __LINE__, openALDevice))
      {
        return;
      }

      alSourcePlay(source);
      ALint state = AL_PLAYING;
      while (state == AL_PLAYING)
      {
        alGetSourcei(source, AL_SOURCE_STATE, &state);
      }
      alDeleteSources(1, &source);
    });
  }
  void SoundEmiterComponent::play()
  {
    job.join();
    job.start();
  }

  AudioEngine::AudioEngine()
  {
    if(!openALDevice){
      // alutInit(NULL, NULL);
      alutInit(0, NULL);
      openALDevice = alcOpenDevice(nullptr);
      if (!openALDevice)
      {
        std::cerr << "error" << std::endl;
        return;
      }
      openALContext = alcCreateContext(openALDevice, nullptr);
      if (!check_alc_errors(__FILE__, __LINE__, openALDevice))
      {
        std::cerr << "ERROR: Could not create audio context" << std::endl;
        return;
      }
      ALCboolean contextMadeCurrent = alcMakeContextCurrent(openALContext);
      if (!check_alc_errors(__FILE__, __LINE__, openALDevice) || contextMadeCurrent != ALC_TRUE)
      {
        std::cerr << "ERROR: Could not make audio context current" << std::endl;
        return;
      }
    }
  }
  void AudioEngine::play(const char *fileName, audiofiletype type)
  {
    BaseAudioBuffer *buffer = createBuffer(fileName,type);
    play(buffer);
  }
  void AudioEngine::play(BaseAudioBuffer* buffer){
    queueBuffer("",buffer,false);
    this->queue[this->queue.size()-1]->job.start();
  }
  void AudioEngine::queueBuffer(const char* name,BaseAudioBuffer *buffer, bool loop){
    AudioQueue*current=new AudioQueue(name,threaded_job([this,index=this->queue.size(),loop]() {
        AudioQueue*current=this->queue[index];
        ALuint buffer=((OpenALBuffer *)current->buffer)->buffer;
        ALuint source;
        // store volume in player obj
        alGenSources(1, &source);
        alSourcef(source, AL_PITCH, 1);
        alSourcef(source, AL_GAIN, current->volume);
        alSource3f(source, AL_POSITION, 0, 0, 0);
        alSource3f(source, AL_VELOCITY, 0, 0, 0);
        if(current->loop){
          alSourcei(source, AL_LOOPING, AL_TRUE);
        }else{
          alSourcei(source, AL_LOOPING, AL_FALSE);
        }
        alSourcei(source, AL_BUFFER, buffer);
        if (!check_alc_errors(__FILE__, __LINE__, openALDevice))
        {
          std::cerr << "gen source" << std::endl;
        }
        alSourcePlay(source);
        ALint state = AL_PLAYING;
        while (state == AL_PLAYING && !current->stop)
        {
          alGetSourcei(source, AL_SOURCE_STATE, &state);
        }
        alDeleteSources(1, &source);
        if (current->discard)
        {
          current->job.detach();
        }
      }),buffer);
    queue.push_back(current);
  }

  BaseAudioBuffer *createBuffer(const char *fileName, audiofiletype type)
  {
    return new OpenALBuffer(fileName,type);
  }
}