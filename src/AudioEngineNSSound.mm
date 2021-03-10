#import <Foundation/Foundation.h>
#include <AppKit/NSSound.h>
#include "EngineObjects.hpp"
#include "utils.h"
#include "kThread.h"
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
    this->queue.push_back(AudioQueue(name,buffer));
  }
  void AudioEngine::load(std::vector<EngineObject *> objects)
  {
  }
  void AudioEngine::update(std::vector<msg> msgs)
  {
    for(u32 i=0;i<queue.size();i++){
      AudioQueue q=queue[i];
      NSSoundBuffer*buffer=(NSSoundBuffer*)q.buffer;
      buffer->sound.loops=q.loop;
      buffer->sound.volume=q.volume;

      bool bufferPlaying=buffer->sound.playing;
      if(q.isPlaying&&!bufferPlaying){
        [buffer->sound play];
      }
      if(q.stop&&q.isPlaying){
        [buffer->sound stop];
        // [buffer->sound rewind];
        q.isPlaying=false;
      }
      if(q.isPlaying){
        q.isPlaying=buffer->sound.isPlaying;
      }
      if (q.discard&&!q.isPlaying)
      {
        delete buffer;
        queue.erase(queue.begin()+i);
      }
    }
  }
  void AudioEngine::unload(std::vector<EngineObject *> objects)
  {
  }

  BaseAudioBuffer *createBuffer(const char* fileName,audiofiletype type){
    return new NSSoundBuffer(fileName);
  }
}
