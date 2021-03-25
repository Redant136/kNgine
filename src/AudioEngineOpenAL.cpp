#include <vector>
#include <string>
#include "EngineObjects.hpp"
#include "utils.h"
#include "AudioEngine.hpp"
#include "../extern/stb/stb_vorbis.c"

#ifndef __APPLE__
#include <AL/al.h>
#include <AL/alc.h>
#include <wave/file.h>
#else //just for debug purpose, should never be loaded
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <wave/file.h>
#endif

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
    assert(0);
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
    ALuint source=0;
    OpenALBuffer(const char* fileName,audiofiletype type)
    {
      if(type==audiofiletype::wav){
        // buffer = alutCreateBufferFromFile(fileName);
        alGenBuffers(1,&buffer);
        wave::File file;
        wave::Error err = file.Open(fileName, wave::kIn);
        if (err)
        {
          std::cout << "Something went wrong in in open" << std::endl;
        }
        std::vector<float> content;
        err = file.Read(&content);
        if (err)
        {
          std::cout << "Something went wrong in read" << std::endl;
        }
        i32 chan=file.channel_number();
        i32 samplerate=file.sample_rate();
        i32 samples=content.size();
        // AudioFile<double> audioFile;
        // audioFile.load(fileName);
        // i32 samplerate = audioFile.getSampleRate();
        // i32 samples = audioFile.getNumSamplesPerChannel();
        // i32 chan = audioFile.getNumChannels();
        ALenum format;
        if (chan == 1 && samplerate == 8)
          format = AL_FORMAT_MONO8;
        else if (chan == 1 && samplerate == 16)
          format = AL_FORMAT_MONO16;
        else if (chan == 2 && samplerate == 8)
          format = AL_FORMAT_STEREO8;
        else if (chan == 2 && samplerate == 16)
          format = AL_FORMAT_STEREO16;
        else
        {
          std::cerr
              << "ERROR: unrecognised wave format: "
              << chan << " channels, "
              << samplerate << " bps" << std::endl;
          std::cout<<samples<<std::endl;
        }
        // std::vector<u_int16_t>pcm_data=audioFile.get;
        alBufferData(buffer, format, content.data(), samples * sizeof(float), samplerate);
        if (!check_alc_errors(__FILE__, __LINE__, openALDevice) || buffer == AL_NONE)
        {
          std::cerr << "ERROR: Could not load file" << std::endl;
        }
      }else if(type=audiofiletype::ogg){
        i32 chan, samplerate;
        short *output;
        i32 samples = stb_vorbis_decode_filename(fileName, &chan, &samplerate, &output);
        alGenBuffers(1, &buffer);
        alBufferData(buffer, AL_FORMAT_MONO16, output, samples * sizeof(short), samplerate);
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

  SoundListenerComponent::SoundListenerComponent(ComponentGameObject *object) : ObjectComponent(object)
  {
    this->label = "[AudioPlayer]";
    this->flags|=ObjectFlags::AUDIO;
    if(!openALDevice){
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
  SoundListenerComponent::~SoundListenerComponent(){
    if(openALDevice){
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
  }

  SoundEmiterComponent::SoundEmiterComponent(ComponentGameObject *object, const char *fileName, audiofiletype type, SoundListenerComponent *player) : SoundEmiterComponent(object, createBuffer(fileName, type), player)
  {
  }
  SoundEmiterComponent::SoundEmiterComponent(ComponentGameObject *object, BaseAudioBuffer *buffer, SoundListenerComponent *player) : ObjectComponent(object)
  {
    this->label = "[AudioEmiter]";
    this->flags|=ObjectFlags::AUDIO;
    this->buffer = buffer;
    this->player = player;
    // job.stop();
    // job = threaded_job([this](){
    //   ALuint source;
    //   alGenSources(1, &source);
    //   alSourcef(source, AL_PITCH, 1);
    //   // store volume in player obj
    //   alSourcef(source, AL_GAIN, this->volume);
    //   alSource3f(source, AL_POSITION, this->object->position.x - this->player->object->position.x, this->object->position.y - this->player->object->position.y, this->object->position.z - this->player->object->position.z);
    //   //nope nope nope
    //   alSource3f(source, AL_VELOCITY, 0, 0, 0);
    //   alSourcei(source, AL_LOOPING, AL_FALSE);
    //   alSourcei(source, AL_BUFFER, ((OpenALBuffer*)this->buffer)->buffer);
    //   if (!check_alc_errors(__FILE__, __LINE__, openALDevice))
    //   {
    //     return;
    //   }
    //   alSourcePlay(source);
    //   ALint state = AL_PLAYING;
    //   while (state == AL_PLAYING)
    //   {
    //     alGetSourcei(source, AL_SOURCE_STATE, &state);
    //   }
    //   alDeleteSources(1, &source);
    // });
  }
  void SoundEmiterComponent::play()
  {
    // job.join();
    // job.start();
  }

  AudioEngine::AudioEngine()
  {
    this->flags|=ObjectFlags::AUDIO;
    this->labels.push_back("AudioEngine");
    if(!openALDevice){
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
  AudioEngine::~AudioEngine(){
    for (u32 i = 0; i < queue.size(); i++)
    {
      delete queue[i].buffer;
    }
    if (!(openALDevice))
    {
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
  }
  
  void AudioEngine::play(const char *fileName, audiofiletype type)
  {
    BaseAudioBuffer *buffer = createBuffer(fileName,type);
    play(buffer);
  }
  void AudioEngine::play(BaseAudioBuffer* buffer){
    queueBuffer("",buffer,false);
    play(queue.size()-1);
  }
  void AudioEngine::queueBuffer(const char* name,BaseAudioBuffer *buffer, bool loop){
    this->queue.push_back(AudioQueue(name,buffer));
  }
  void AudioEngine::load(std::vector<EngineObject *> objects)
  {
    for(AudioQueue q:queue){
      ALuint albuffer = ((OpenALBuffer *)q.buffer)->buffer;
      ALuint source;
      alGenSources(1, &source);
      alSourcef(source, AL_PITCH, 1);
      alSource3f(source, AL_POSITION, 0, 0, 0);
      alSource3f(source, AL_VELOCITY, 0, 0, 0);
      alSourcei(source, AL_BUFFER, albuffer);
      if (!check_alc_errors(__FILE__, __LINE__, openALDevice))
      {
        std::cerr << "gen source" << std::endl;
      }
      ((OpenALBuffer *)q.buffer)->source=source;
    }
  }
  void AudioEngine::update(std::vector<msg> msgs)
  {
    for(u32 i=0;i<queue.size();i++){
      AudioQueue*q=&queue[i];
      OpenALBuffer*buffer=(OpenALBuffer*)q->buffer;
      alSourcef(buffer->source, AL_GAIN, q->volume);
      if (q->loop)
      {
        alSourcei(buffer->source, AL_LOOPING, AL_TRUE);
      }
      else
      {
        alSourcei(buffer->source, AL_LOOPING, AL_FALSE);
      }
      if (!check_alc_errors(__FILE__, __LINE__, openALDevice))
      {
        std::cerr << "gen source" << std::endl;
      }
      ALint state = AL_PLAYING;
      alGetSourcei(buffer->source, AL_SOURCE_STATE, &state);
      if(q->start&&state==AL_STOPPED){
        alSourcePlay(buffer->source);
        q->start=false;
        state = AL_PLAYING;
      }
      if(q->stop&&q->isPlaying){
        alSourceStop(buffer->source);
        alSourceRewind(buffer->source);
        q->isPlaying=false;
      }else{
        alGetSourcei(buffer->source, AL_SOURCE_STATE, &state);
        q->isPlaying=state==AL_PLAYING;
      }
      if (q->discard&&!q->isPlaying)
      {
        alDeleteSources(1, &buffer->source);
        buffer->source = 0;
        delete buffer;
        queue.erase(queue.begin()+i);
      }
    }
  }
  void AudioEngine::unload(std::vector<EngineObject *> objects)
  {
    for(AudioQueue q:queue){
      OpenALBuffer *buffer = (OpenALBuffer *)q.buffer;
      alDeleteSources(1, &buffer->source);
      buffer->source=0;
    }
  }

  BaseAudioBuffer *createBuffer(const char *fileName, audiofiletype type)
  {
    return new OpenALBuffer(fileName,type);
  }
}