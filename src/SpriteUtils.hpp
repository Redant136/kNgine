#pragma once

#include <vector>
#include "utils.hpp"
#include "EngineObjects.hpp"

#include <iostream>
namespace kNgine
{
  class SpriteMap : public EngineObject
  {
  public:
    std::vector<Sprite> list = std::vector<Sprite>();
    std::vector<unsigned int> texIndex = std::vector<unsigned int>();
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
    virtual unsigned int getMapIndex()=0;
  };
  class SpriteReferenceComponent : public SpriteMapAccessor
  {
  public:
    unsigned int mapIndex;
    v2 spriteDimension;
    std::string spriteLocation;
    SpriteReferenceComponent(ComponentGameObject *base, SpriteMap *spriteList, int index);
    SpriteReferenceComponent(ComponentGameObject *base, SpriteMap *spriteList, Sprite sprite);
    SpriteReferenceComponent(const SpriteComponent &base, SpriteMap *spriteList);
    SpriteReferenceComponent(const SpriteReferenceComponent &base);
    virtual ~SpriteReferenceComponent();
    virtual void update(std::vector<msg>msgs);
    unsigned int getMapIndex();
    Sprite *getSprite();
    v2 getSpriteDimensions();
    v2 getSpriteOffset();//where the sprite will be started to be drawn from
  };
  class SpriteAnimation : public SpriteMapAccessor
  {
  private:
    std::vector<unsigned int>spritesIndexes;
    unsigned int frame;
    float timeUntilNextFrame=0;
  public:
    std::vector<v2>spriteDimensions;
    std::string spriteLocation;
    float frameLength;
    SpriteAnimation(ComponentGameObject *base, SpriteMap *spriteList, std::vector<unsigned int> indexes,
                    float frameLength, v2 spriteDimension = v2(1.0f, 1.0f));
    SpriteAnimation(ComponentGameObject *base, SpriteMap *spriteList, std::vector<Sprite> sprites,
                    float frameLength, v2 spriteDimension = v2(1.0f, 1.0f));

    SpriteAnimation(ComponentGameObject *base, SpriteMap *spriteList, std::vector<unsigned int> indexes,
                    float frameLength, std::vector<v2> spriteDimensions);
    SpriteAnimation(ComponentGameObject *base, SpriteMap *spriteList, std::vector<Sprite> sprites,
                    float frameLength, std::vector<v2> spriteDimensions);
    SpriteAnimation(const SpriteAnimation &base);
    virtual ~SpriteAnimation();
    virtual void update(std::vector<msg>msgs);
    unsigned int getMapIndex();
    Sprite *getSprite();
    v2 getSpriteDimensions();
    v2 getSpriteOffset();
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
      int index = std::max(accessors.size(),names.size());
      accessors.resize(index+1);
      names.resize(index+1);
      accessors[index]=accessor;
      names[index]=name;
      if(active==NULL)active=accessor;
    }
    void setActive(std::string name) {
      for(int i=0;i<names.size();i++){
        if(names[i]==name){
          this->active=accessors[i];
        }
      }
    }
    void setActive(unsigned int index){
      this->active=accessors[index];
    }
    virtual void update(std::vector<msg> msgs){active->update(msgs);}
    unsigned int getMapIndex(){return active->getMapIndex();}
    Sprite *getSprite(){return active->getSprite();}
    v2 getSpriteDimensions(){return v2(active->getSpriteDimensions().x*spriteDimension.x,active->getSpriteDimensions().y*spriteDimension.y);}
    v2 getSpriteOffset(){return active->getSpriteOffset();}
  };

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
    int getSpriteListLength(){return accessors.size();}
    bool hasToSave(){return false;}
    Sprite *getSprite(){return accessors[0]->getSprite();}
    v2 getSpriteDimensions() { return accessors[0]->getSpriteOffset();};
    v2 getSpriteOffset(){return accessors[0]->getSpriteOffset();};
  };

  std::vector<Sprite> importSpriteSheet(const char *filename, int spriteWidth, int spriteHeight);
} // namespace kNgine
