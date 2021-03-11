#pragma once

#include <vector>
#include <string>
#include "EngineObjects.hpp"
#include "utils.h"

#include <iostream>

namespace kNgine
{
  enum audiofiletype
  {
    wav,
    ogg
  };

  struct BaseAudioBuffer
  { //to be extended, contains all library specific options
  protected:
    BaseAudioBuffer() {}

  public:
    virtual ~BaseAudioBuffer() {}
  };

  //[AudioPlayer]
  // to be attached to main character or camera
  class SoundListenerComponent : public ObjectComponent // there can only be one sound player
  {
  public:
    f32 globalVolume = 1.0f;
    SoundListenerComponent(ComponentGameObject *object);
    virtual ~SoundListenerComponent();
  };

  //[AudioEmiter]
  // attach to anything making sound, just short sound effects
  class SoundEmiterComponent : public ObjectComponent
  {
  private:
    SoundListenerComponent *player;
    bool playing = false;
    f32 volume = 1.0f;
  public:
    BaseAudioBuffer *buffer;
    SoundEmiterComponent(ComponentGameObject *object, const char *fileName, audiofiletype type, SoundListenerComponent *player);
    SoundEmiterComponent(ComponentGameObject *object, BaseAudioBuffer *buffer, SoundListenerComponent *player);
    bool isPlaying() { return playing; }
    void play();
  };

  // background music or stuff
  class AudioEngine final: public EngineObject
  {
  private:
    struct AudioQueue
    {
      std::string name;
      bool isPlaying=false;
      BaseAudioBuffer *buffer;
      bool discard = true;
      bool loop = false;
      f32 volume = 1.0f;
      bool stop = false;
      AudioQueue(std::string name, BaseAudioBuffer *buffer)
      {
        this->name = name;
        this->buffer = buffer;
      }
      AudioQueue(const AudioQueue &base)
      {
        this->buffer = base.buffer;
        this->discard = base.discard;
        this->loop = base.loop;
        this->volume = base.volume;
        this->stop = base.stop;
      }
      ~AudioQueue()
      {
        if (discard)
          delete buffer;
      }
    };
    std::vector<AudioQueue*> queue;
  public:
    AudioEngine();
    virtual ~AudioEngine();
    void play(const char *fileName, audiofiletype type);
    void play(BaseAudioBuffer *buffer);
    void queueBuffer(const char *name, BaseAudioBuffer *buffer, bool loop = false);

    void play(u32 index = 0) { queue[index]->isPlaying=true; }
    void play(const char*name);// TODO(AC) implement this
    bool isPlaying(u32 index = 0) { return queue[index]->isPlaying; }
    void setLoop(bool loop, u32 index = 0) { queue[index]->loop = loop; }
    void setVolume(f32 volume, u32 index = 0) { queue[index]->volume = volume; }
    void terminate(u32 index = 0) { queue[index]->stop = true; }
    virtual void load(std::vector<EngineObject *> objects);
    virtual void update(std::vector<msg>msgs);
    virtual void unload(std::vector<EngineObject *> objects);
  };

  BaseAudioBuffer *createBuffer(const char *fileName, audiofiletype type);
} // namespace kNgine
