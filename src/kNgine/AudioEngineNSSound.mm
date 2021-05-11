#import <Foundation/Foundation.h>
#include <AppKit/NSSound.h>
#include "EngineObjects.hpp"
#include "../kFramework/utils.h"
#include "AudioEngine.hpp"

#include <stdio.h>

namespace kNgine{
  struct NSSoundBuffer:public BaseAudioBuffer
  {
  public:
    int number=0;
    NSSound*sound;
    NSSoundBuffer(const char* fileName){
      number=1;
      NSURL *inputFileURL = [NSURL fileURLWithPath:[NSString stringWithUTF8String:fileName]];
      sound=[[NSSound alloc]initWithContentsOfURL:inputFileURL byReference:NO];
    }
    ~NSSoundBuffer(){
      [sound release];
    }
  };

  SoundListenerComponent::SoundListenerComponent(ComponentGameObject *object) : ObjectComponent(object){
    this->label = "[AudioPlayer]";
    object->flags|=AUDIO;
    object->labels.push_back(label);
  }
  SoundListenerComponent::~SoundListenerComponent(){
  }

  SoundEmiterComponent::SoundEmiterComponent(ComponentGameObject *object, const char* fileName,audiofiletype type, SoundListenerComponent *player) : SoundEmiterComponent(object, createBuffer(fileName,type),player)
  {
  }
  SoundEmiterComponent::SoundEmiterComponent(ComponentGameObject *object, BaseAudioBuffer *buffer, SoundListenerComponent *player) : ObjectComponent(object)
  {
    this->label = "[AudioEmiter]";
    this->flags|=AUDIO;
    this->buffer = buffer;
    this->player = player;
  }
  void SoundEmiterComponent::play()
  {

  }

  AudioEngine::AudioEngine(){
    this->flags|=AUDIO;
    this->labels.push_back("AudioEngine");
  }
  AudioEngine::~AudioEngine()
  {
    for(u32 i=0;i<queue.size();i++){
      delete queue[i].buffer;
    }
  }

  void AudioEngine::play(const char* fileName,audiofiletype type){
    BaseAudioBuffer*buffer=createBuffer(fileName,type);
    play(buffer);
  }
  void AudioEngine::play(BaseAudioBuffer* buffer){
    queueBuffer("",buffer,false);
    play(queue.size()-1);
  }
  void AudioEngine::queueBuffer(const char* name,BaseAudioBuffer *buffer, bool loop)
  {
    queue.push_back(AudioQueue(name,buffer));
    queue[queue.size()-1].loop=loop;
  }
  void AudioEngine::init(std::vector<EngineObject *> obj){
  }
  void AudioEngine::load()
  {
  }
  void AudioEngine::update(std::vector<msg> msgs)
  {
    for(u32 i=0;i<queue.size();i++){
      AudioQueue*q=&queue[i];
      NSSoundBuffer*buffer=(NSSoundBuffer*)q->buffer;
      buffer->sound.loops=q->loop;
      buffer->sound.volume=q->volume;

      bool bufferPlaying=false;
      if(buffer->sound.playing){// i hate objective-c
        bufferPlaying=true;
      }else{
        bufferPlaying=false;
      }
      if(q->start&&!bufferPlaying){
        [buffer->sound play];
        q->start=false;
      }
      if(q->stop&&q->isPlaying){
        [buffer->sound stop];
        q->isPlaying=false;
      }
      if(buffer->sound.playing){// i hate objective-c
        q->isPlaying=true;
      }else{
        q->isPlaying=false;
      }
      if (q->discard&&!q->isPlaying)
      {
        delete buffer;
        queue.erase(queue.begin()+i);
      }
    }
  }
  void AudioEngine::unload()
  {
  }

  BaseAudioBuffer *createBuffer(const char* fileName,audiofiletype type){
    return new NSSoundBuffer(fileName);
  }
}
