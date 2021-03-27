#pragma once

#include <vector>
#include <string>
#include <functional>
#include "../kFramework/utils.h"

namespace kNgine
{
  //msg sent from engine to specific objects
  struct msg
  {
    enum
    {
      TIME_ELAPSED,
      KEY,
      CURSOR
    } msgType;
    union
    {
      f32 time;
      Key key;
      v2 cursorPos;
    };
  };
  enum ObjectFlags
  {
    NONE            = 0 << 0,
    GAME_OBJECT     = 1 << 0,
    COMPONENT       = 1 << 1,
    PARENT          = 1 << 2,
    SPRITE          = 1 << 3,
    Sprite_List     = 1 << 4,
    RENDERER_LAYER  = 1 << 5,
    Physics         = 1 << 6,
    AUDIO           = 1 << 7
  };

  #define kNgine_maxLayerOrder 64
  struct LayerOrder{
     size_t length;
     struct {u32 layerId;const char*name;} ids[kNgine_maxLayerOrder];
     u32 order[kNgine_maxLayerOrder];
  };
  static LayerOrder layerOrderInit()// have to allocate array
  {
    LayerOrder lo = {0, NULL, NULL};
    return lo;
  }
  static void addLayerOrderDef(LayerOrder*order, const char *layer)
  {
    assert(order->length < kNgine_maxLayerOrder);
    order->ids[order->length] = {(u32)order->length, layer};
    order->length++;
  }
  static void setLayerOrder(LayerOrder*order,u32*o){
    for (u32 i = 0; i < order->length; i++)
    {
      order->order[i]=o[i];
    }
  }
  static u32 layerO(LayerOrder order,const char*name)
  {
    for (u32 i = 0; i < order.length; i++)
    {
      if(order.ids[i].name==name){
        return order.ids[i].layerId;
      }
    }
    return 0;
  }
#define LayerOrder() layerOrderInit()
#define addLayerOrderDef(order, layer) addLayerOrderDef(&order, #layer)
#define setLayerOrder(order, o) setLayerOrder(&order, o)
#define layerO(order, layer) layerO(order, #layer)

  typedef struct
 {
   u32 width, height, numChannels;
   u8 *buffer;
 } Sprite;
 inline Sprite SpriteInit(u32 width, u32 height, u32 numChannels, u8 *buffer)
 {
   Sprite spr = {width, height, numChannels, buffer};
   return spr;
 }
#define Sprite(width, height, channels, buffer) SpriteInit(width, height, channels, buffer)
  inline Sprite fillSprite(u32 width, u32 height, rgbcolor fill)
  {
    u8 *colorMap = new u8[width * height * 4];
    for (int i = 0; i < width * height * 4; i += 4)
    {
      colorMap[i + 0] = fill.r;
      colorMap[i + 1] = fill.g;
      colorMap[i + 2] = fill.b;
      colorMap[i + 3] = fill.a;
    }
    return Sprite(width, height, 4, colorMap);
  }
  inline void freeSprite(Sprite sprite)
  {
    delete[] sprite.buffer;
  }
  inline void offsetPixelsInSprite(Sprite *sprite, cardinal8dir dir, u32 offset)
  {
    if (dir == CENTER)
    {
      return;
    }
    iv2 copyLine;
    iv2 cut = iv2(0, 0);
    switch (dir)
    {
    case NORTH:
      copyLine = iv2(0, 0);
      cut.y = offset;
      break;
    case WEST:
      copyLine = iv2(0, 0);
      cut.x = offset;
      break;
    case SOUTH:
      copyLine = iv2(0, sprite->height - 1);
      cut.y = -offset;
      break;
    case EAST:
      copyLine = iv2(sprite->width - 1, 0);
      cut.x = -offset;
      break;
    case NORTH_WEST:
      copyLine = iv2(0, 0);
      cut.y = offset;
      cut.x = offset;
      break;
    case NORTH_EAST:
      copyLine = iv2(sprite->width - 1, 0);
      cut.y = offset;
      cut.x = -offset;
      break;
    case SOUTH_WEST:
      copyLine = iv2(0, sprite->height - 1);
      cut.y = -offset;
      cut.x = offset;
      break;
    case SOUTH_EAST:
      copyLine = iv2(sprite->width - 1, sprite->height - 1);
      cut.y = -offset;
      cut.x = -offset;
      break;
    default:
      copyLine = iv2(0, 0);
      break;
    }

    u8 *newBuffer = new u8[sprite->width * sprite->height * sprite->numChannels];
    for (u32 i = 0; i < sprite->height; i++)
    {
      i32 y = i - cut.y;
      if (y < 0 || y >= sprite->height)
      {
        y = copyLine.y;
      }
      for (u32 j = 0; j < sprite->width; j++)
      {
        i32 x = j - cut.x;
        if (x < 0 || x >= sprite->width)
        {
          x = copyLine.x;
        }
        for (u32 c = 0; c < sprite->numChannels; c++)
        {
          newBuffer[(j + i * sprite->width) * sprite->numChannels + c] = sprite->buffer[(x + y * sprite->width) * sprite->numChannels + c];
        }
      }
    }
    delete[] sprite->buffer;
    sprite->buffer = newBuffer;
  }

  // objects for engine
  class EngineObject
  {
  protected:
    bool enabled = true;
  public:
    std::vector<std::string> labels = std::vector<std::string>();
    u64 flags = 0;
    EngineObject()
    {
      labels = std::vector<std::string>();
    }
    EngineObject(const EngineObject &base)
    {
      this->labels = base.labels;
      this->flags = base.flags;
      this->enabled = base.enabled;
    }
    virtual ~EngineObject() {}
    bool isEnabled() { return enabled; }
    virtual void enable() { enabled = true; }
    virtual void disable() { enabled = false; }
    virtual void update(std::vector<msg> msgs) {}
    virtual void init(std::vector<EngineObject *> objects) {}
    virtual void end(std::vector<EngineObject *> objects) {}
    virtual void load(std::vector<EngineObject *> object) {}
    virtual void unload(std::vector<EngineObject *> object) {}
  };
  // object to be positionned in game
  class GameObject : public EngineObject
  {
  public:
    v3 position = v3(0, 0, 0);
    v3 rotation = v3(0, 0, 0); // v3(yz,xz,xy)
    GameObject();
    GameObject(const GameObject &base);
    virtual ~GameObject() {}
  };

  class ObjectComponent;
  class ComponentGameObject : public GameObject
  {
  public:
    std::vector<ObjectComponent *> components;
    ComponentGameObject();
    ComponentGameObject(const ComponentGameObject &base);
    virtual ~ComponentGameObject();
    void update(std::vector<msg> msgs);
    void addComponent(ObjectComponent *component);
    template <typename T = ObjectComponent>
    T *findComponent(std::string flag);
    void removeComponent(ObjectComponent *component);
    void removeComponent(std::string flag) { removeComponent(findComponent(flag)); }
  };
  // modular objects for implementations, define modules labels using [name]
  class ObjectComponent
  {
  public:
    ComponentGameObject *object;
    std::string label;
    u64 flags=0;
    ObjectComponent(ComponentGameObject *base);
    ObjectComponent(const ObjectComponent &base);
    virtual ~ObjectComponent();
    virtual void update(std::vector<msg> msgs);
  };
  template <typename T>
  T *ComponentGameObject::findComponent(std::string flag)
  {
    for (ObjectComponent *mod : components)
    {
      if (mod->label == flag)
      {
        return (T *)mod;
      }
    }
    return NULL;
  }

  //[sprite]
  class SpriteAccessor : public ObjectComponent
  {
  public:
    cardinal8dir spriteLocation;
    v2 offset = {0.0f, 0.0f}; // offset in game units
    SpriteAccessor(ComponentGameObject *base);
    virtual bool hasToSave() = 0;
    virtual Sprite *getSprite() = 0; //pointer for not having to call copy
    virtual v2 getSpriteDimensions() = 0;
    virtual v2 getSpriteLocation()
    {
      switch (spriteLocation)
      {
      case CENTER:
        return v2(-0.5, -0.5);
      case NORTH_WEST:
        return v2(0, 0);
      case NORTH_EAST:
        return v2(-1, 0);
      case SOUTH_WEST:
        return v2(0, -1);
      case SOUTH_EAST:
        return v2(-1, -1);
      default:
        return v2(0, 0);
      }
    }
  };
  // component representing that the class has a sprite
  class SpriteComponent : public SpriteAccessor
  {
  public:
    Sprite sprite;
    // in game units
    v2 spriteDimension;
    SpriteComponent(ComponentGameObject *base);
    SpriteComponent(ComponentGameObject *base, Sprite sprite);
    SpriteComponent(const SpriteComponent &base);
    virtual ~SpriteComponent();
    virtual void update(std::vector<msg> msgs);
    bool hasToSave() { return true; }
    Sprite *getSprite();
    v2 getSpriteDimensions();
  };

  class LayerRenderer : public GameObject
  {
  public:
    u32 layer=0;// to be used in conjunction with LayerOrder
    LayerRenderer() { this->flags |= ObjectFlags::RENDERER_LAYER; }
    LayerRenderer(const LayerRenderer &base) : GameObject(base) {}
    virtual void updateWindowSize(i32 windowWidth, i32 windowHeight){}
    virtual void render(){}
  };
  static std::vector<LayerRenderer*>getRenderersAtLayer(std::vector<LayerRenderer*>renderers,u32 layer){
    std::vector<LayerRenderer*>res;
    for(u32 i=0;i<renderers.size();i++){
      if(renderers[i]->layer==layer){
        res.push_back(renderers[i]);
      }
    }
    return res;
  }

  class NodeObjectComponent final: public ObjectComponent
  {
  private:
    v3 previousParentPos;
  public:
    GameObject* child;
    NodeObjectComponent(ComponentGameObject *parent, GameObject *child);
    void update(std::vector<msg> msgs);
  };

  struct EngineEvent
  {
    std::string name;
    std::function<void *(void *)> event;
  };
  void addEvent(EngineEvent event);
  void *callEvent(std::string name, void *arg = NULL);

  template <class T = EngineObject,class I = EngineObject>
  std::vector<T *> findObject(std::vector<I *> objects,
                              std::string label)
  {
    std::vector<EngineObject *> valid = std::vector<EngineObject *>();
    if (label == "ALL")
    {
      valid = objects;
    }
    else
    {
      for (EngineObject *obj : objects)
      {
        for (std::string s : obj->labels)
        {
          if (s == "ALL" || s == label)
          {
            valid.push_back(obj);
          }
        }
      }
    }
    std::vector<T *> res = std::vector<T *>();
    for (EngineObject *obj : valid)
    {
      res.push_back((T *)obj);
    }
    return res;
  };
  template <class T = EngineObject, class I = EngineObject>
  std::vector<T *> findObjectBlacklist(std::vector<I *> objects,
                                       std::string label) // returns all objects that dont have the label
  {
    std::vector<EngineObject *> valid = std::vector<EngineObject *>();
    if (label == "ALL")
    {
      return std::vector<T *>();
    }
    else
    {
      for (EngineObject *obj : objects)
      {
        for (std::string s : obj->labels)
        {
          if (!(s == "ALL" || s == label))
          {
            valid.push_back(obj);
          }
        }
      }
    }
    std::vector<T *> res = std::vector<T *>();
    for (EngineObject *obj : valid)
    {
      res.push_back((T *)obj);
    }
    return res;
  };
  template <class T = EngineObject, class I = EngineObject>
  std::vector<T *> findObject(I **objects, u32 objectsSize,
                              u64 flags)
  {
    std::vector<EngineObject *> valid = std::vector<EngineObject *>();
    if (flags == ~ObjectFlags::NONE)
    {
      valid = std::vector<EngineObject *>(objects, objects + objectsSize);
    }
    else
    {
      for (u32 i = 0; i < objectsSize; i++)
      {
        if (objects[i]->flags & flags)
        {
          valid.push_back(objects[i]);
        }
      }
    }
    std::vector<T *> res = std::vector<T *>();
    for (EngineObject *obj : valid)
    {
      res.push_back((T *)obj);
    }
    return res;
  };
  template <class T = EngineObject, class I = EngineObject>
  std::vector<T *> findObject(std::vector<I *> objects,
                              u64 flags)
  {
    return findObject<T>(objects.data(), objects.size(), flags);
  };

  Sprite importSprite(const char *filename);

  template <class T = GameObject, class I = GameObject>
  std::vector<T *> orderObjectsByZ(std::vector<I *> objects)
  {
    if (objects.size() == 0)
    {
      return std::vector<T *>();
    }
    for (i32 i = 0; i < objects.size() - 1; i++)
    {
      for (i32 j = 0; j < objects.size() - 1 - i; j++)
      {
        if (objects[j]->position.z > objects[j + 1]->position.z)
        {
          I *temp = objects[j];
          objects[j] = objects[j + 1];
          objects[j + 1] = temp;
        }
      }
    }
    std::vector<T *> res = std::vector<T *>(objects.size());
    for (i32 i = 0; i < objects.size(); i++)
    {
      res[i] = (T *)objects[i];
    }
    return res;
  };
} // namespace kNgine
