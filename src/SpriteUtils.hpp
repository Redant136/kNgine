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
    bool loadOnInit = true;
    SpriteMap();
    virtual ~SpriteMap();
    void init(std::vector<EngineObject *> objects);
    void end(std::vector<EngineObject *> objects);
    void enable(){if(!enabled){load();enabled=true;}}
    void disable(){if(enabled){unload();enabled=false;}}
    void load();
    void unload();
  };
  class SpriteMapAccessor:public SpriteAccessor{
  public:
    SpriteMap* spriteList;
    SpriteMapAccessor(GameObject *base);
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
    std::vector<SpriteMapAccessor *> accessors;
    std::vector<std::string>names;
    SpriteMapAccessor *active;
  public:
    v2 spriteDimension;
    SpriteAnimationSystem(GameObject *base, SpriteMap *spriteList) : SpriteMapAccessor(base)
    {
      this->spriteList=spriteList;
      active=NULL;
      spriteDimension=v2(1,1);
    }
    void addSprite(SpriteMapAccessor *accessor,std::string name){
      i32 index = std::max(accessors.size(),names.size());
      accessors.resize(index+1);
      names.resize(index+1);
      accessors[index]=accessor;
      names[index]=name;
      if(active==NULL)active=accessor;
    }
    void setActive(std::string name) {
      for(i32 i=0;i<names.size();i++){
        if(names[i]==name){
          this->active=accessors[i];
        }
      }
    }
    void setActive(u32 index){
      this->active=accessors[index];
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
    SpriteList(GameObject *base, SpriteMap *spriteList) : SpriteList(base,std::vector<SpriteAccessor*>(),spriteList)
    {
    }
    SpriteList(GameObject *base, std::vector<SpriteAccessor *> accessors, SpriteMap *spriteList) : SpriteAccessor(base)
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
