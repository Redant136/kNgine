#import <Foundation/Foundation.h>
#include <AppKit/NSSound.h>
#include "EngineObjects.hpp"
#include "utils.hpp"
#include "AudioEngine.hpp"

namespace kNgine{
  struct NSSoundBuffer:public BaseAudioBuffer
  {
  public:
    NSSound*sound;
    NSSoundBuffer(const char* fileName){
      NSURL *inputFileURL = [NSURL fileURLWithPath:[NSString stringWithUTF8String:fileName]];
      sound=[[NSSound alloc]initWithContentsOfURL:inputFileURL byReference:NO];
    }
    ~NSSoundBuffer(){
      [sound release];
    }
  };

  SoundListenerComponent::SoundListenerComponent(GameObject *object) : ObjectComponent(object){
    this->label = "[AudioPlayer]";
    object->flags.push_back(objectFlags::AUDIO);
    object->flags.push_back(objectFlags::AUDIOPLAYER);
    object->labels.push_back(label);
  }
  SoundListenerComponent::~SoundListenerComponent(){
  }

  SoundEmiterComponent::SoundEmiterComponent(GameObject *object, const char* fileName,audiofiletype type, SoundListenerComponent *player) : SoundEmiterComponent(object, createBuffer(fileName,type),player)
  {
  }
  SoundEmiterComponent::SoundEmiterComponent(GameObject *object, BaseAudioBuffer *buffer, SoundListenerComponent *player) : ObjectComponent(object)
  {
    this->label = "[AudioEmiter]";
    object->flags.push_back(objectFlags::AUDIO);
    object->labels.push_back(label);
    this->buffer = buffer;
    this->player = player;
  }
  void SoundEmiterComponent::play()
  {

  }

  AudioEngine::AudioEngine(){
    this->flags.push_back(objectFlags::AUDIO);
    this->labels.push_back("AudioEngine");
  }
  void AudioEngine::play(const char* fileName,audiofiletype type){
    BaseAudioBuffer*buffer=createBuffer(fileName,type);
    play(buffer);
  }
  void AudioEngine::play(BaseAudioBuffer* buffer){
    queueBuffer("",buffer,false);
    this->queue[this->queue.size()-1]->job.start();
  }
  void AudioEngine::queueBuffer(const char* name,BaseAudioBuffer *buffer, bool loop)
  {
    AudioQueue*current=new AudioQueue(name,threaded_job([this, index=this->queue.size(),loop]() {
      NSSoundBuffer*buffer=(NSSoundBuffer*)this->queue[index]->buffer;
      buffer->sound.loops=loop;
      [buffer->sound play];
      while(buffer->sound.playing && !this->queue[index]->stop){
      }
      [buffer->sound stop];
      if(this->queue[index]->discard){
        this->queue[index]->job.detach();
      }
    }),buffer);
    this->queue.push_back(current);
   }

  BaseAudioBuffer *createBuffer(const char* fileName,audiofiletype type){
    return new NSSoundBuffer(fileName);
  }
}
