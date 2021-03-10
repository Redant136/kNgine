#pragma once

#include <vector>
#include "utils.h"
#include "EngineObjects.hpp"

namespace kNgine
{
  class SpriteMap : public EngineObject
  {
  public:
    std::vector<Sprite> list = std::vector<Sprite>();
    std::vector<u32 > texIndex = std::vector<u32 >();
    SpriteMap();
    virtual ~SpriteMap();
    void init(std::vector<EngineObject *> objects);
    void end(std::vector<EngineObject *> objects);
    void load(std::vector<EngineObject *> objects);
    void unload(std::vector<EngineObject *> objects);
    void offsetPixelsInSprites(cardinal8dir dir, u32 offset);
  };
  class SpriteMapAccessor:public SpriteAccessor{
  public:
    SpriteMap* spriteList;
    SpriteMapAccessor(ComponentGameObject *base);
    bool hasToSave() { return false; }
    virtual u32 getMapIndex()=0;
  };
  class SpriteReferenceComponent : public SpriteMapAccessor
  {
  public:
    u32 mapIndex;
    v2 spriteDimension;
    SpriteReferenceComponent(ComponentGameObject *base, SpriteMap *spriteList, i32 index);
    SpriteReferenceComponent(ComponentGameObject *base, SpriteMap *spriteList, Sprite sprite);
    SpriteReferenceComponent(const SpriteComponent &base, SpriteMap *spriteList);
    SpriteReferenceComponent(const SpriteReferenceComponent &base);
    virtual ~SpriteReferenceComponent();
    virtual void update(std::vector<msg>msgs);
    u32 getMapIndex();
    Sprite *getSprite();
    v2 getSpriteDimensions();
  };
  class SpriteAnimation : public SpriteMapAccessor
  {
  private:
    std::vector<u32 >spritesIndexes;
    u32 frame;
    f32 timeUntilNextFrame=0;
  public:
    std::vector<v2>spriteDimensions;
    f32 frameLength;
    SpriteAnimation(ComponentGameObject *base, SpriteMap *spriteList, std::vector<u32 > indexes,
                    f32 frameLength, v2 spriteDimension = v2(1.0f, 1.0f));
    SpriteAnimation(ComponentGameObject *base, SpriteMap *spriteList, std::vector<Sprite> sprites,
                    f32 frameLength, v2 spriteDimension = v2(1.0f, 1.0f));

    SpriteAnimation(ComponentGameObject *base, SpriteMap *spriteList, std::vector<u32 > indexes,
                    f32 frameLength, std::vector<v2> spriteDimensions);
    SpriteAnimation(ComponentGameObject *base, SpriteMap *spriteList, std::vector<Sprite> sprites,
                    f32 frameLength, std::vector<v2> spriteDimensions);
    SpriteAnimation(const SpriteAnimation &base);
    virtual ~SpriteAnimation();
    virtual void update(std::vector<msg>msgs);
    u32 getMapIndex();
    Sprite *getSprite();
    v2 getSpriteDimensions();
  };
  //[animation_system]
  class SpriteAnimationSystem:public SpriteMapAccessor{//make sure that spriteanimation uses the same map
  protected:
    struct accessor{
      std::string name;
      SpriteMapAccessor*accessor;
    };
    std::vector<accessor>accessors;
    SpriteMapAccessor *active;
  public:
    v2 spriteDimension;
    SpriteAnimationSystem(ComponentGameObject *base, SpriteMap *spriteList) : SpriteMapAccessor(base)
    {
      this->spriteList=spriteList;
      active=NULL;
      spriteDimension=v2(1,1);
    }
    void addSprite(SpriteMapAccessor *accessor,std::string name){
      accessors.push_back({name,accessor});
      accessor->object=this->object;
      if(active==NULL)active=accessor;
    }
    void setActive(std::string name) {
      for(i32 i=0;i<accessors.size();i++){
        if(accessors[i].name==name){
          this->active=accessors[i].accessor;
        }
      }
    }
    void setActive(u32 index){
      this->active=accessors[index].accessor;
    }
    virtual void update(std::vector<msg> msgs){active->update(msgs);}
    u32 getMapIndex(){return active->getMapIndex();}
    Sprite *getSprite(){return active->getSprite();}
    v2 getSpriteDimensions(){return v2(active->getSpriteDimensions().x*spriteDimension.x,active->getSpriteDimensions().y*spriteDimension.y);}
    v2 getSpriteLocation(){return active->getSpriteLocation();}
  };

  //[sprite_list]
  class SpriteList : public SpriteAccessor{
  protected:
    std::vector<SpriteAccessor *> accessors;
  public:
    v2 spriteDimension;
    SpriteList(ComponentGameObject *base, SpriteMap *spriteList) : SpriteList(base, std::vector<SpriteAccessor *>(), spriteList)
    {
    }
    SpriteList(ComponentGameObject *base, std::vector<SpriteAccessor *> accessors, SpriteMap *spriteList) : SpriteAccessor(base)
    {
      this->label="[sprite_list]";
      this->accessors=accessors;
      spriteDimension = v2(1, 1);
    }
    virtual ~SpriteList()
    {
      for (SpriteAccessor *spr : accessors)
      {
        delete spr;
      }
    }
    void addAccessor(SpriteAccessor* accessor){accessors.push_back(accessor);}
    virtual void update(std::vector<msg> msgs) {
      for(SpriteAccessor*spr:accessors){
        spr->update(msgs);
      }
    }
    SpriteAccessor**getSpriteList(){return accessors.data();}
    i32 getSpriteListLength(){return accessors.size();}
    bool hasToSave(){return false;}
    Sprite *getSprite(){return accessors[0]->getSprite();}
    v2 getSpriteDimensions() { return accessors[0]->getSpriteLocation();}
    v2 getSpriteLocation(){return accessors[0]->getSpriteLocation();}
  };

  std::vector<Sprite> importSpriteSheet(const char *filename, i32 spriteWidth, i32 spriteHeight);
} // namespace kNgine
