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
      printf("%s\n",[inputFileURL.absoluteString UTF8String]);
      sound=[[NSSound alloc]initWithContentsOfURL:inputFileURL byReference:NO];
    }
    ~NSSoundBuffer(){
      [sound release];
    }
  };

  SoundPlayerComponent::SoundPlayerComponent(GameObject *object) : ObjectComponent(object){

  }
  SoundPlayerComponent::~SoundPlayerComponent(){
  }

  SoundEmiterComponent::SoundEmiterComponent(GameObject *object, const char* fileName,audiofiletype type, SoundPlayerComponent *player) : SoundEmiterComponent(object, new NSSoundBuffer(fileName),player)
  {

  }
  SoundEmiterComponent::SoundEmiterComponent(GameObject *object, BaseAudioBuffer *buffer, SoundPlayerComponent *player) : ObjectComponent(object)
  {

  }
  void SoundEmiterComponent::play()
  {

  }

  AudioEngine::AudioEngine(){

  }
  void AudioEngine::play(const char* fileName,audiofiletype type){
    BaseAudioBuffer*buffer=createBuffer(fileName);
    play(buffer);
  }
  void AudioEngine::play(BaseAudioBuffer* buffer){
    queueBuffer("",buffer,false);
    printf("%s\n","queued");
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
