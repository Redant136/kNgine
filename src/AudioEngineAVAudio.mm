#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import "EngineObjects.hpp"
#import "utils.hpp"
#import "AudioEngine.hpp"

namespace kNgine{
  AVAudioEngine*engine;
  AVAudioMixerNode*mixerNode;
  struct AVAudioBuffer:public BaseAudioBuffer
  {
  public:
    AVAudioPCMBuffer*buffer;
    AVAudioBuffer(const char* fileName){
      NSURL *inputFileURL = [NSURL fileURLWithPath:[NSString stringWithUTF8String:fileName]];
      NSString *str = inputFileURL.absoluteString;
      printf("%s\n",[str UTF8String]);
      NSError*nerror=[NSError alloc];
      AVAudioFile* file=[[AVAudioFile alloc]initForReading:[inputFileURL absoluteURL] error:&nerror];
      if(nerror){
        NSLog(@" error => %@ ", [nerror userInfo] );
      }
      buffer=[[AVAudioPCMBuffer alloc] initWithPCMFormat:file.processingFormat frameCapacity:UInt32(file.length)];
      [file readIntoBuffer:this->buffer error:&nerror];
      if(nerror){
        NSLog(@" error => %@ ", [nerror userInfo] );
      }
      [nerror release];
    }
    ~AVAudioBuffer(){
      [buffer release];
    }
  };

  // SoundListenerComponent::SoundListenerComponent(GameObject *object) : ObjectComponent(object){
  //   if(!engine){
  //     engine=[[AVAudioEngine alloc]init];
  //     NSUserDefaults* set=[NSUserDefaults standardUserDefaults];
  //     {
  //       AudioObjectPropertyAddress addr={
  //         kAudioHardwarePropertyDefaultOutputDevice,
  //         kAudioObjectPropertyScopeGlobal,
  //         kAudioObjectPropertyElementMaster
  //       };
  //       AudioObjectID deviceID=0;
  //       UInt32 size=sizeof(deviceID);
  //       OSStatus err=AudioObjectGetPropertyData(AudioObjectID(kAudioObjectSystemObject), 
  //                               &addr, 
  //                               0, 
  //                               NULL, 
  //                               &size, 
  //                               &deviceID);
  //       if(err==noErr && kAudioDeviceUnknown != deviceID){
  //         @try{
  //           [[[engine outputNode] AUAudioUnit] setDeviceID:deviceID error:nil];
  //         }@catch(NSException *e ){

  //         }
  //       }else{
  //         NSLog(@"ERROR: couldn't get default output device, ID = (deviceID), err = (err)");
  //       }
  //     }
  //     mixerNode=[[AVAudioMixerNode alloc] init];
  //     [engine attachNode:mixerNode];
  //     [engine connect:mixerNode to:[engine mainMixerNode] format:[[engine outputNode] outputFormatForBus:0]];
  //   }
  // }
  // SoundListenerComponent::~SoundListenerComponent(){
  //   [engine release];
  //   [mixerNode release];
  // }

  // SoundEmiterComponent::SoundEmiterComponent(GameObject *object, const char* fileName, SoundListenerComponent *player) : SoundEmiterComponent(object, new AVAudioBuffer(fileName),player){}
  // SoundEmiterComponent::SoundEmiterComponent(GameObject *object, BaseAudioBuffer *buffer, SoundListenerComponent *player) : ObjectComponent(object){}
  // void SoundEmiterComponent::play(){}

  AudioEngine::AudioEngine(){
    if(!engine){
      engine=[[AVAudioEngine alloc]init];
      NSUserDefaults* set=[NSUserDefaults standardUserDefaults];
      {
        AudioObjectPropertyAddress addr={
          kAudioHardwarePropertyDefaultOutputDevice,
          kAudioObjectPropertyScopeGlobal,
          kAudioObjectPropertyElementMaster
        };
        AudioObjectID deviceID=0;
        UInt32 size=sizeof(deviceID);
        OSStatus err=AudioObjectGetPropertyData(AudioObjectID(kAudioObjectSystemObject), 
                                &addr, 
                                0, 
                                NULL, 
                                &size, 
                                &deviceID);
        if(err==noErr && kAudioDeviceUnknown != deviceID){
          @try{
            [engine outputNode];
            // [[[engine outputNode] AUAudioUnit] setDeviceID:deviceID error:nil];
          }@catch(NSException *e ){

          }
        }else{
          NSLog(@"ERROR: couldn't get default output device, ID = (deviceID), err = (err)");
        }
      }
      mixerNode=[[AVAudioMixerNode alloc] init];
      [engine attachNode:mixerNode];
      // [engine connect:mixerNode to:[engine mainMixerNode] format:[[engine outputNode] outputFormatForBus:0]];
    }
  }
  void AudioEngine::play(const char* fileName){
    BaseAudioBuffer*buffer=createBuffer(fileName);
    play(buffer);
    delete buffer;
  }
  void AudioEngine::play(BaseAudioBuffer* buffer){
    // i32  index=this->queue.size();
    // this->queue.push_back({threaded_job([this, index]() {
    //   AVAudioPlayerNode*player = [[AVAudioPlayerNode alloc]init];
    //   [engine attachNode:player];
    //   [engine connect:player to:mixerNode format:nil];
    //   NSError*startError=[NSError alloc];
    //   [engine startAndReturnError:&startError];
    //   [player scheduleBuffer:((AVAudioBuffer*)this->queue[index].buffer)->buffer completionHandler:nil];
    //   [player play];
    //   while(engine.running && !this->queue[index].stop){

    //   }
    //   [player stop];
    //   [startError release];
    //   if(this->queue[index].discard){
    //     this->queue[index].job.stopNoJoin();
    //     this->queue.erase(this->queue.begin() + index, this->queue.begin() + index);
    //   }
    // }),buffer});
    // [engine prepare];
    // this->queue[index].job.start();
  }
  void AudioEngine::queueBuffer(BaseAudioBuffer *buffer, bool loop){
    // i32  index=this->queue.size();
    // this->queue.push_back({threaded_job([this, index]() {
    //   AVAudioPlayerNode*player = [[AVAudioPlayerNode alloc]init];
    //   [engine attachNode:player];
    //   [engine connect:player to:mixerNode format:nil];
    //   NSError*startError=[NSError alloc];
    //   [engine startAndReturnError:&startError];
    //   [player scheduleBuffer:((AVAudioBuffer*)this->queue[index].buffer)->buffer completionHandler:nil];
    //   [player play];
    //   while(engine.running && !this->queue[index].stop){

    //   }
    //   [player stop];
    //   [startError release];
    //   if(this->queue[index].discard){
    //     this->queue[index].job.stopNoJoin();
    //     this->queue.erase(this->queue.begin() + index, this->queue.begin() + index);
    //   }
    // }),buffer});
    // [engine prepare];
  }

  BaseAudioBuffer *createBuffer(const char* fileName){
    return new AVAudioBuffer(fileName);
  }

}
