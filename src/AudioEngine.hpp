#pragma once

#include <vector>
#include <string>
#include "EngineObjects.hpp"
#include "utils.h"
#include "kThread.h"

namespace kNgine
{
  enum audiofiletype{wav,ogg};

  struct BaseAudioBuffer
  { //to be extended
  protected:
    BaseAudioBuffer() {}
  public:
    virtual ~BaseAudioBuffer(){}
  };

  //[AudioPlayer]
  // to be attached to main character or camera
  class SoundListenerComponent : public ObjectComponent // there can only be one sound player
  {
  public:
    float globalVolume=1.0f;
    SoundListenerComponent(GameObject *object);
    virtual ~SoundListenerComponent();
  };

  //[AudioEmiter]
  // attach to anything making sound, just short sound effects
  class SoundEmiterComponent : public ObjectComponent
  {
  private:
    SoundListenerComponent *player;
    bool playing=false;
    float volume=1.0f;
    threaded_job job=threaded_job([](){});
  public:
    BaseAudioBuffer *buffer;
    SoundEmiterComponent(GameObject *object, const char *fileName, audiofiletype type, SoundListenerComponent *player);
    SoundEmiterComponent(GameObject *object, BaseAudioBuffer *buffer,SoundListenerComponent*player);
    bool isPlaying(){return playing;}
    void play();
  };

  // background music or stuff
  class AudioEngine : public EngineObject
  {
  private:
    struct AudioQueue{
      std::string name;
      threaded_job job;
      BaseAudioBuffer*buffer;
      bool discard=true;
      bool loop=false;
      float volume=1.0f;
      bool stop=false;
      AudioQueue(std::string name,threaded_job job,BaseAudioBuffer*buffer):job(job){
        this->name=name;
        this->buffer=buffer;
      }
      AudioQueue(const AudioQueue& base):job(base.job){
        this->buffer=base.buffer;
        this->discard=base.discard;
        this->loop=base.loop;
        this->volume=base.volume;
        this->stop=base.stop;
      }
      ~AudioQueue()
      {
        job.join();
        job.stop();
        if(discard)delete buffer;
      }
    };
    std::vector<AudioQueue*>queue;
    SoundListenerComponent *player=NULL;
  public:
    AudioEngine();
    virtual ~AudioEngine();
    void play(const char *fileName, audiofiletype type);
    void play(BaseAudioBuffer* buffer);
    void queueBuffer(const char*name,BaseAudioBuffer*buffer,bool loop=false);

    void play(unsigned int index=0){queue[index]->job.start();}
    void setLoop(bool loop,unsigned int index = 0) { queue[index]->loop = loop; }
    void setVolume(float volume,unsigned int index = 0) { queue[index]->volume = volume; }
    void terminate(unsigned int index=0){queue[index]->stop=true;}
    void init(std::vector<EngineObject*>objects){
      for(EngineObject*obj:objects){

      }
    }
  };

  BaseAudioBuffer *createBuffer(const char* fileName,audiofiletype type);
} // namespace kNgine
