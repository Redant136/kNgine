#pragma once

#include "EngineObjects.hpp"

namespace kNgine
{
  class SpriteMap : public EngineObject
  {
  public:
    std::vector<Sprite> list = std::vector<Sprite>();
    std::vector<u32> texIndex = std::vector<u32>();
    SpriteMap();
    virtual ~SpriteMap();
    void init(std::vector<EngineObject *> objects);
    void end(std::vector<EngineObject *> objects);
    void load();
    void unload();
    void offsetPixelsInSprites(cardinal8dir dir, u32 offset);
  };
  class SpriteMapAccessor : public SpriteAccessor
  {
  public:
    SpriteMap *spriteList;
    SpriteMapAccessor(ComponentGameObject *base);
    bool hasToSave() { return false; }
    virtual u32 getMapIndex() { return 0; };
  };
  class SpriteReferenceComponent : public SpriteMapAccessor
  {
  public:
    u32 mapIndex;
    v2 spriteDimension;
    SpriteReferenceComponent(ComponentGameObject *base, SpriteMap *spriteList, u32 index);
    SpriteReferenceComponent(ComponentGameObject *base, SpriteMap *spriteList, Sprite sprite);
    SpriteReferenceComponent(const SpriteComponent &base, SpriteMap *spriteList);
    SpriteReferenceComponent(const SpriteReferenceComponent &base);
    virtual ~SpriteReferenceComponent();
    virtual void update(std::vector<msg> msgs);
    u32 getMapIndex();
    Sprite *getSprite();
    v2 getSpriteDimensions();
  };
  
  class SpriteAnimation : public SpriteMapAccessor
  {
  private:
    std::vector<u32> spritesIndexes;
    u32 frame;
    f32 timeUntilNextFrame = 0;

  public:
    std::vector<v2> spriteDimensions;
    f32 frameLength;
    SpriteAnimation(ComponentGameObject *base, SpriteMap *spriteList, std::vector<u32> indexes,
                    f32 frameLength, v2 spriteDimension = v2(1.0f, 1.0f));
    SpriteAnimation(ComponentGameObject *base, SpriteMap *spriteList, std::vector<Sprite> sprites,
                    f32 frameLength, v2 spriteDimension = v2(1.0f, 1.0f));

    SpriteAnimation(ComponentGameObject *base, SpriteMap *spriteList, std::vector<u32> indexes,
                    f32 frameLength, std::vector<v2> spriteDimensions);
    SpriteAnimation(ComponentGameObject *base, SpriteMap *spriteList, std::vector<Sprite> sprites,
                    f32 frameLength, std::vector<v2> spriteDimensions);
    SpriteAnimation(const SpriteAnimation &base);
    virtual ~SpriteAnimation();
    virtual void update(std::vector<msg> msgs);
    u32 getMapIndex();
    Sprite *getSprite();
    v2 getSpriteDimensions();
  };

  // [RendererObject]
  // should really be extended, but not necessary
  // WILL FUCK UP IF MORE THAN ONE CAMERA EACH FRAME
  class RendererObject_base : public Renderable
  {
  public:
    u32 rendererObjectIndex; // will be incorrect until load is called

    RendererObject_base(ComponentGameObject *base);
    RendererObject_base(ComponentGameObject *base, size_t numShaders,
                        u32 *shaderIndex, size_t *numTriangles, f32 ***points[3]);

    virtual void updatePoints(m4 matrix) {}
    virtual std::vector<std::vector<u32>> getSpriteIndexes() { return std::vector<std::vector<u32>>(); }
    virtual void load();
    virtual void unload();
  };

  // WILL FUCK UP IF MORE THAN ONE CAMERA EACH FRAME
  class RendererObject : public RendererObject_base
  {
  private:
    struct
    {
      v3 pos;
      f32 isTex;
      v4 texCoord;
    } default_points[4];

  public:
    SpriteMapAccessor *renderable;

    RendererObject(ComponentGameObject *base, SpriteMapAccessor *renderable);
    RendererObject(ComponentGameObject *base, SpriteMap *spriteList, Sprite sprite);
    virtual ~RendererObject() { delete renderable; }
    virtual void updatePoints(m4 matrix);
    virtual std::vector<std::vector<u32>> getSpriteIndexes();
    virtual void init(std::vector<EngineObject *> objects) { renderable->init(objects); }
    virtual void load()
    {
      RendererObject_base::load();
      renderable->load();
    }
    virtual void update(std::vector<msg> msgs) { renderable->update(msgs); }
    virtual void unload()
    {
      RendererObject_base::unload();
      renderable->unload();
    }
    virtual void end(std::vector<EngineObject *> objects) { renderable->end(objects); }
  };

  //[animation_system]
  class SpriteAnimationSystem : public SpriteMapAccessor
  { //make sure that spriteanimation uses the same map
  protected:
    struct accessor_pair
    {
      std::string name;
      SpriteMapAccessor *accessor;
    };
    std::vector<accessor_pair> accessors;
    SpriteMapAccessor *active;

  public:
    v2 spriteDimension;
    SpriteAnimationSystem(ComponentGameObject *base, SpriteMap *spriteList) : SpriteMapAccessor(base)
    {
      this->spriteList = spriteList;
      active = NULL;
      spriteDimension = v2(1, 1);
    }
    void addSprite(SpriteMapAccessor *accessor, std::string name)
    {
      struct accessor_pair a = {name, accessor};
      accessors.push_back(a);
      accessor->object = this->object;
      if (active == NULL)
        active = accessor;
    }
    void setActive(std::string name)
    {
      for (u32 i = 0; i < accessors.size(); i++)
      {
        if (accessors[i].name == name)
        {
          this->active = accessors[i].accessor;
        }
      }
    }
    void setActive(u32 index)
    {
      this->active = accessors[index].accessor;
    }
    virtual void update(std::vector<msg> msgs) { active->update(msgs); }
    u32 getMapIndex() { return active->getMapIndex(); }
    Sprite *getSprite() { return active->getSprite(); }
    v2 getSpriteDimensions() { return v2(active->getSpriteDimensions().x * spriteDimension.x, active->getSpriteDimensions().y * spriteDimension.y); }
    v2 getSpriteLocation() { return active->getSpriteLocation(); }
  };

  //[Rend_SYS]
  class RenderableSystem : public Renderable
  {
  protected:
    struct accessor_pair
    {
      std::string name;
      Renderable *accessor;
    };
    std::vector<accessor_pair> accessors;

  public:
    Renderable *active;
    v2 spriteDimension;
    RenderableSystem(ComponentGameObject *base) : Renderable(base)
    {
      active = NULL;
      spriteDimension = v2(1, 1);
      this->label = "[Rend_SYS]";
    }
    void addSprite(Renderable *accessor, std::string name)
    {
      struct accessor_pair a = {name, accessor};
      accessors.push_back(a);
      accessor->object = this->object;
      if (active == NULL)
        active = accessor;
    }
    void setActive(std::string name)
    {
      for (u32 i = 0; i < accessors.size(); i++)
      {
        if (accessors[i].name == name)
        {
          setActive(i);
        }
      }
    }
    void setActive(u32 index)
    {
      this->active->unload();
      this->active = accessors[index].accessor;
      this->active->load();
    }
    virtual void init(std::vector<EngineObject *> objects)
    {
      for (u32 i = 0; i < accessors.size(); i++)
      {
        accessors[i].accessor->init(objects);
      }
    }
    virtual void load() { active->load(); }
    virtual void update(std::vector<msg> msgs) { active->update(msgs); }
    virtual void unload() { active->unload(); }
    virtual void end(std::vector<EngineObject *> objects)
    {
      for (u32 i = 0; i < accessors.size(); i++)
      {
        accessors[i].accessor->end(objects);
      }
    }
  };

  std::vector<Sprite> importSpriteSheet(const char *filename, i32 spriteWidth, i32 spriteHeight);
} // namespace kNgine
