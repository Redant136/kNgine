#include <vector>
#include "utils.h"
#include "EngineObjects.hpp"
#include "Renderer.hpp"
#include "SpriteUtils.hpp"
#include "../extern/stb/stb_image.h"

#include <iostream>
namespace kNgine
{
  SpriteMap::SpriteMap() { this->flags.push_back(objectFlags::Sprite_List); }
  SpriteMap::~SpriteMap(){
    for (i32 i = 0; i < list.size(); i++)
    {
      freeSprite(list[i]);
    }
  }
  void SpriteMap::init(std::vector<EngineObject*>objects){
    if(loadOnInit){
      load();
    }
  }
  void SpriteMap::end(std::vector<EngineObject*>objects){
    if(loadOnInit){
      unload();
    }
  }
  void SpriteMap::load(){
    texIndex = std::vector<u32 >();
    for (i32 i = 0; i < list.size(); i++) {
      u32 texture, VBO;
      renderer::bindTexture(&texture, list[i].colorMap, v2(0, 0), 1, 1,
                            list[i].width, list[i].height, list[i].numChannels);
      texIndex.push_back(texture);
    }
  }
  void SpriteMap::unload(){
    for (i32 i = 0; i < list.size(); i++) {
      renderer::unbindTexture(texIndex[i]);
    }
  }

  SpriteMapAccessor::SpriteMapAccessor(GameObject *base):SpriteAccessor(base)
  {
  }

  SpriteReferenceComponent::SpriteReferenceComponent(ComponentGameObject *base, SpriteMap *spriteList,
                                                     i32 index)
      : SpriteMapAccessor(base)
  {
    this->spriteLocation = CENTER;
    this->spriteDimension = v2(1, 1);
    this->mapIndex = index;
    this->spriteList = spriteList;
  }
  SpriteReferenceComponent::SpriteReferenceComponent(ComponentGameObject *base, SpriteMap *spriteList,
                                                     Sprite sprite)
      : SpriteMapAccessor(base)
  {
    this->spriteList = spriteList;
    this->mapIndex = spriteList->list.size();
    this->spriteList->list.push_back(sprite);
    this->spriteLocation = CENTER;
    this->spriteDimension = v2(1, 1);
  }
  SpriteReferenceComponent::SpriteReferenceComponent(const SpriteComponent &base, SpriteMap *spriteList) : SpriteMapAccessor(base.object)
  {
    this->spriteList = spriteList;
    this->mapIndex = spriteList->list.size();
    this->spriteList->list.push_back(base.sprite);
    this->spriteLocation = base.spriteLocation;
    this->spriteDimension = base.spriteDimension;
  }
  SpriteReferenceComponent::SpriteReferenceComponent(const SpriteReferenceComponent &base)
      : SpriteMapAccessor(base.object)
  {
    this->spriteList = base.spriteList;
    this->mapIndex = base.mapIndex;
    this->spriteLocation = base.spriteLocation;
    this->spriteDimension = base.spriteDimension;
    this->label = base.label;
  }
  SpriteReferenceComponent::~SpriteReferenceComponent() {}
  void SpriteReferenceComponent::update(std::vector<msg>msgs) {}
  u32 SpriteReferenceComponent::getMapIndex() {return mapIndex;}
  Sprite *SpriteReferenceComponent::getSprite() {
    return &(spriteList->list[mapIndex]);
  }
  v2 SpriteReferenceComponent::getSpriteDimensions() { return spriteDimension; }

  SpriteAnimation::SpriteAnimation(ComponentGameObject *base, SpriteMap *spriteList, std::vector<u32 > indexes,
                                   f32 frameLength, v2 spriteDimension) : SpriteMapAccessor(base)
  {
    this->spriteLocation = CENTER;
    this->spriteList = spriteList;
    this->frame=0;
    this->frameLength=frameLength;
    this->spritesIndexes=indexes;
    this->spriteDimensions=std::vector<v2>(indexes.size());
    for(i32 i=0;i<indexes.size();i++){
      this->spriteDimensions[i]=spriteDimension;
    }
  }
  SpriteAnimation::SpriteAnimation(ComponentGameObject *base, SpriteMap *spriteList, std::vector<Sprite> sprites,
                                   f32 frameLength, v2 spriteDimension) : SpriteMapAccessor(base)
  {
    this->spriteLocation = CENTER;
    this->spriteList = spriteList;
    this->frame = 0;
    this->frameLength = frameLength;
    this->spritesIndexes=std::vector<u32 >(sprites.size());
    this->spriteDimensions=std::vector<v2>(sprites.size());
    for(i32 i=0;i<sprites.size();i++){
      this->spritesIndexes[i]=spriteList->list.size();
      this->spriteDimensions[i]=spriteDimension;
      this->spriteList->list.push_back(sprites[i]);
    }
  }
  SpriteAnimation::SpriteAnimation(ComponentGameObject *base, SpriteMap *spriteList, std::vector<u32 > indexes,
                                  f32 frameLength, std::vector<v2> spriteDimensions) : SpriteMapAccessor(base)
  {
    this->spriteLocation = CENTER;
    this->spriteList = spriteList;
    this->frame = 0;
    this->frameLength = frameLength;
    this->spritesIndexes=indexes;
    this->spriteDimensions=spriteDimensions;
  }
  SpriteAnimation::SpriteAnimation(ComponentGameObject *base, SpriteMap *spriteList, std::vector<Sprite> sprites,
                                   f32 frameLength, std::vector<v2> spriteDimensions) : SpriteMapAccessor(base)
  {
    this->spriteLocation = CENTER;
    this->spriteList = spriteList;
    this->frame = 0;
    this->frameLength = frameLength;
    this->spriteDimensions = spriteDimensions;
    for (i32 i = 0; i < sprites.size(); i++)
    {
      this->spritesIndexes[i] = this->spriteList->list.size();
      this->spriteList->list.push_back(sprites[i]);
    }
  }
  SpriteAnimation::SpriteAnimation(const SpriteAnimation &base) : SpriteMapAccessor(base)
  {
    this->spriteLocation = base.spriteLocation;
    this->spriteList = base.spriteList;
    this->frame = base.frame;
    this->frameLength = base.frameLength;
    this->spriteDimensions = base.spriteDimensions;
    this->spritesIndexes = base.spritesIndexes;
  }
  SpriteAnimation::~SpriteAnimation(){

  }
  void SpriteAnimation::update(std::vector<msg> msgs){
    f32 timeElapsed=0;
    for(i32 i=0;i<msgs.size();i++){
      if(msgs[i].msgType==msg::TIME_ELAPSED){
        timeElapsed=msgs[i].time;
        break;
      }
    }
    timeUntilNextFrame+=timeElapsed;
    if(timeUntilNextFrame>=frameLength){
      timeUntilNextFrame=0;
      frame++;
      if(frame>=spritesIndexes.size()){
        frame=0;
      }
    }
  }
  u32 SpriteAnimation::getMapIndex() { return spritesIndexes[frame]; }
  Sprite* SpriteAnimation::getSprite(){
    return &(spriteList->list[spritesIndexes[frame]]);
  }
  v2 SpriteAnimation::getSpriteDimensions(){
    return spriteDimensions[frame];
  }

  std::vector<Sprite> importSpriteSheet(const char *filename, i32 spriteWidth,
                                        i32 spriteHeight) {
    i32 width, height, numChannels;
    unsigned char *data = stbi_load(filename, &width, &height, &numChannels, 0);

    i32 numSpritesHor=width/spriteWidth;
    i32 numSpritesVert=height/spriteHeight;
    std::vector<Sprite>sprites=std::vector<Sprite>();
    for(i32 i=0;i<numSpritesVert;i++){
      for(i32 j=0;j<numSpritesHor;j++){
        //alloc error
        // std::vector<unsigned char>sprite=std::vector<unsigned char>();
        u8 *sprite = new u8[spriteWidth * spriteHeight * numChannels];
        for (i32 y = 0; y < spriteHeight; y++) 
        {
          for (i32 x = 0; x < spriteWidth; x++)
          {
            for(i32 c = 0;c < numChannels; c++){
              // sprite.push_back(
              //     data[((x + j * spriteWidth) + (y + i * spriteHeight) * width) * numChannels + c]);
              // sprite[(x+y*width)*numChannels+c]=data[((x + j * spriteWidth) + (y + i * spriteHeight) * width) * numChannels + c];
              sprite[(x + y * spriteWidth) * numChannels + c] = data[((x + j * spriteWidth) + (y + i * spriteHeight) * width) * numChannels + c];
            }
          }
        }
        sprites.push_back(Sprite(spriteWidth,spriteHeight,numChannels,sprite));
      }
    }
    return sprites;
  }
} // namespace kNgine
