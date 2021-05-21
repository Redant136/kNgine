#pragma once

#include <vector>
#include <string>
#include <functional>
#include "kutils.h"

namespace kNgine
{
  //msg sent from engine to specific objects
  struct msg
  {
    enum
    {
      TIME_ELAPSED,
      ASCII_KEY,
      NONASCII_KEY,
      CURSOR,
      WINDOW_SIZE
    } msgType;
    union
    {
      f32 time;
      u64 key;
      v2 cursorPos;
      iv2 window_size;
    };
  };
  enum ObjectFlags
  {
    NONE = 0 << 0,
    GAME_OBJECT = 1 << 0,    // object with position
    COMPONENT = 1 << 1,      // contains components
    PARENT = 1 << 2,         // contains child game object components
    RENDERER_LAYER = 1 << 3, // renderer
    RENDERABLE = 1 << 4,     // contains renderable component
    Physics = 1 << 5,        // contains physics component
    AUDIO = 1 << 6           // contains audio component
  };

#define kNgine_maxLayerOrder 32
  struct LayerOrder
  {
    size_t length;
    struct
    {
      u32 layerId;
      const char *name;
    } ids[kNgine_maxLayerOrder];
    u32 order[kNgine_maxLayerOrder];
  };
  static LayerOrder layerOrderInit() // have to allocate array
  {
    LayerOrder lo = {0};
    return lo;
  }
  static inline void addLayerOrderDef(LayerOrder *order, const char *layer)
  {
    assert(order->length < kNgine_maxLayerOrder);
    order->ids[order->length] = {(u32)order->length, layer};
    order->length++;
  }
  static inline void setLayerOrder(LayerOrder *order, u32 *o)
  {
    for (u32 i = 0; i < order->length; i++)
    {
      order->order[i] = o[i];
    }
  }
  static inline u32 layerO(LayerOrder order, const char *name)
  {
    for (u32 i = 0; i < order.length; i++)
    {
      if (std::string(order.ids[i].name) == name)
      {
        return order.ids[i].layerId;
      }
    }
    return 0;
  }
#define LayerOrder() layerOrderInit()
#define addLayerOrderDef(order, layer) addLayerOrderDef(&order, #layer)
#define setLayerOrder(order, o) setLayerOrder(&order, o)
#define layerO(order, layer) layerO(order, #layer)

  struct Sprite
  {
    u32 width, height, numChannels;
    u8 *buffer;
  };
  static Sprite SpriteInit(u32 width, u32 height, u32 numChannels, u8 *buffer)
  {
    Sprite spr = {width, height, numChannels, buffer};
    return spr;
  }
#define Sprite(width, height, channels, buffer) SpriteInit(width, height, channels, buffer)
  static inline Sprite fillSprite(u32 width, u32 height, rgbcolor fill)
  {
    u8 *colorMap = new u8[4UL * width * height];
    for (u32 i = 0; i < 4UL * width * height; i += 4)
    {
      colorMap[i + 0] = fill.r;
      colorMap[i + 1] = fill.g;
      colorMap[i + 2] = fill.b;
      colorMap[i + 3] = fill.a;
    }
    return Sprite(width, height, 4, colorMap);
  }
  static inline void freeSprite(Sprite sprite)
  {
    delete[] sprite.buffer;
  }
  static inline void offsetPixelsInSprite(Sprite *sprite, cardinal8dir dir, u32 offset)
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
      cut.y = -1*offset;
      break;
    case EAST:
      copyLine = iv2(sprite->width - 1, 0);
      cut.x = -1*offset;
      break;
    case NORTH_WEST:
      copyLine = iv2(0, 0);
      cut.y = offset;
      cut.x = offset;
      break;
    case NORTH_EAST:
      copyLine = iv2(sprite->width - 1, 0);
      cut.y = offset;
      cut.x = -1*offset;
      break;
    case SOUTH_WEST:
      copyLine = iv2(0, sprite->height - 1);
      cut.y = -1*offset;
      cut.x = offset;
      break;
    case SOUTH_EAST:
      copyLine = iv2(sprite->width - 1, sprite->height - 1);
      cut.y = -1*offset;
      cut.x = -1*offset;
      break;
    default:
      copyLine = iv2(0, 0);
      break;
    }

    u8 *newBuffer = new u8[(size_t)sprite->width * sprite->height * sprite->numChannels];
    for (u32 i = 0; i < sprite->height; i++)
    {
      i32 y = i - cut.y;
      if (y < 0 || y >= signed(sprite->height))
      {
        y = copyLine.y;
      }
      for (u32 j = 0; j < sprite->width; j++)
      {
        i32 x = j - cut.x;
        if (x < 0 || x >= signed(sprite->width))
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
    virtual void init(std::vector<EngineObject *> objects) {}
    virtual void load() {}
    virtual void update(std::vector<msg> msgs) {}
    virtual void unload() {}
    virtual void end(std::vector<EngineObject *> objects) {}
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
    virtual void init(std::vector<EngineObject *> objects);
    virtual void load();
    virtual void update(std::vector<msg> msgs);
    virtual void unload();
    virtual void end(std::vector<EngineObject *> objects);
    void addComponent(ObjectComponent *component);
    template <typename T = ObjectComponent>
    T *findComponent(u64 flags);
    template <typename T = ObjectComponent>
    T *findComponent(std::string label);
    void removeComponent(ObjectComponent *component);
    void removeComponent(u64 flags) { removeComponent(findComponent(flags)); }
    void removeComponent(std::string label) { removeComponent(findComponent(label)); }
  };
  // modular objects for implementations, define modules labels using [name]
  class ObjectComponent
  {
  public:
    ComponentGameObject *object;
    std::string label;
    u64 flags = 0;
    ObjectComponent(ComponentGameObject *base);
    ObjectComponent(const ObjectComponent &base);
    virtual ~ObjectComponent(){}
    virtual void init(std::vector<EngineObject *> objects){}
    virtual void load() {}
    virtual void update(std::vector<msg> msgs){}
    virtual void unload() {}
    virtual void end(std::vector<EngineObject *> objects){}
  };
  template <typename T>
  T *ComponentGameObject::findComponent(u64 flags)
  {
    for (ObjectComponent *mod : components)
    {
      if (mod->flags & flags)
      {
        return (T *)mod;
      }
    }
    return NULL;
  }
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

  class Renderable : public ObjectComponent
  {
  public:
    bool specialAccessor = false; // if the accessor requires something different before rendering
    Renderable(ComponentGameObject*base):ObjectComponent(base)
    {
      this->flags|=ObjectFlags::RENDERABLE;
    }
  };
  //[sprite]
  class SpriteAccessor : public Renderable
  {
  public:
    cardinal8dir spriteLocation;
    v3 offset = {{0.0f, 0.0f, 0.0f}}; // offset in game units
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
    u32 layer = 0; // to be used in conjunction with LayerOrder
    LayerRenderer() { this->flags |= ObjectFlags::RENDERER_LAYER; }
    LayerRenderer(const LayerRenderer &base) : GameObject(base) {}
    virtual void render() {}
  };
  static std::vector<LayerRenderer *> getRenderersAtLayer(std::vector<LayerRenderer *> renderers, u32 layer)
  {
    std::vector<LayerRenderer *> res;
    for (u32 i = 0; i < renderers.size(); i++)
    {
      if (renderers[i]->layer == layer)
      {
        res.push_back(renderers[i]);
      }
    }
    return res;
  }

  class NodeObjectComponent final : public ObjectComponent
  {
  private:
    v3 previousParentPos;

  public:
    GameObject *child;
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
  EngineEvent& getEvent(std::string name);
  void removeEvent(std::string name);

  template <class T = EngineObject, class I = EngineObject>
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
  std::vector<T *> findObject(Array<I*>objects,
                              u64 flags)
  {
    std::vector<EngineObject *> valid = std::vector<EngineObject *>();
    if (flags == ~ObjectFlags::NONE)
    {
      valid = std::vector<EngineObject *>(objects.arr, objects.arr + objects.length);
    }
    else
    {
      for (u32 i = 0; i < objects.length; i++)
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
    return findObject<T, I>({objects.size(), objects.data()}, flags);
  };

  Sprite importSprite(const char *filename);

  template <class T = GameObject, class I = GameObject>
  std::vector<T *> orderObjectsByZ(std::vector<I *> objects)
  {
    if (objects.size() == 0)
    {
      return std::vector<T *>();
    }
    for (u32 i = 0; i < objects.size() - 1; i++)
    {
      for (u32 j = 0; j < objects.size() - 1 - i; j++)
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
    for (u32 i = 0; i < objects.size(); i++)
    {
      res[i] = (T *)objects[i];
    }
    return res;
  };
} // namespace kNgine
