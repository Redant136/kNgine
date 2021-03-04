#pragma once

#include <vector>
#include <string>
#include <functional>
#include "utils.h"

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
    } msgBody;
  };
  enum objectFlags
  {
    ALL,
    GAME_OBJECT,
    COMPONENT,
    PARENT,
    CHILD,
    //----
    SPRITE,
    Sprite_List,
    BACKGROUND,
    CAMERA,
    UI,
    //----
    Physics,
    PhysicsEngine,
    //----
    AUDIO,
    AUDIOPLAYER
  };

  // an object containing important info to draw image for the engine
  struct Sprite
  {
    // in pixels
    i32  width, height, numChannels;
    std::vector<unsigned char> colorMap;
    Sprite();
    Sprite(i32  width, i32  height, rgbcolor colorFill);
    Sprite(i32  width, i32  height, unsigned char *colorMap);
    Sprite(i32  width, i32  height, i32  numChannels, unsigned char *colorMap);
    Sprite(i32  width, i32  height, i32  numChannels,
           std::vector<unsigned char> colorMap);
    Sprite(const Sprite &base);
    ~Sprite();
    void resize(i32  newWidth, i32  newHeight);
  };
  // objects for engine
  class EngineObject
  {
  protected:
    bool enabled=true;
  public:
    std::vector<std::string> labels;
    std::vector<objectFlags> flags;
    EngineObject(){ labels = std::vector<std::string>(); flags=std::vector<objectFlags>();}
    EngineObject(const EngineObject&base){this->labels=base.labels;this->flags=base.flags;this->enabled=base.enabled;}
    virtual ~EngineObject(){}
    bool isEnabled(){return enabled;}
    virtual void enable(){enabled=true;}
    virtual void disable(){enabled=false;}
    virtual void update(std::vector<msg> msgs){}
    virtual void init(std::vector<EngineObject *> objects){}
    virtual void end(std::vector<EngineObject *> objects){}
  };
  // object to be positionned in game
  class GameObject : public EngineObject
  {
  public:
    v3 position=v3(0,0,0);
    v3 rotation=v3(0,0,0); // v3(yz,xz,xy)
    GameObject();
    GameObject(const GameObject&base);
    virtual ~GameObject(){}
  };
  // modular objects for implementations, define modules labels using [name]
  class ObjectComponent
  {
  public:
    GameObject *object;
    std::string label;
    ObjectComponent(GameObject *base);
    ObjectComponent(const ObjectComponent &base);
    virtual ~ObjectComponent();
    virtual void update(std::vector<msg> msgs);
  };
  class ComponentGameObject : public GameObject
  {
  protected:
    std::vector<ObjectComponent *> components;
  public:
    ComponentGameObject();
    ComponentGameObject(const ComponentGameObject &base);
    virtual ~ComponentGameObject();
    void update(std::vector<msg> msgs);
    void addComponent(ObjectComponent *component)
    {
      components.push_back(component);
    }
    template <typename T = ObjectComponent>
    T *findComponent(std::string flag)
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
    void removeComponent(ObjectComponent *component);
    void removeComponent(std::string flag) { removeComponent(findComponent(flag));}
  };
  //[sprite]
  class SpriteAccessor : public ObjectComponent
  {
  public:
    enum
    {
      CENTER,
      TOP_LEFT,
      TOP_RIGHT,
      BOT_LEFT,
      BOT_RIGHT,
      TOP,
      BOT,
      RIGHT,
      LEFT
    } spriteLocation;
    v2 offset={0.0f,0.0f};// offset in game units
    SpriteAccessor(GameObject *base);
    virtual bool hasToSave() = 0;
    virtual Sprite *getSprite() = 0; //pointer for not having to call copy
    virtual v2 getSpriteDimensions() = 0;
    virtual v2 getSpriteLocation()
    {
      switch (spriteLocation)
      {
      case CENTER:
        return v2(-0.5, -0.5);
      case TOP_LEFT:
        return v2(0, 0);
      case TOP_RIGHT:
        return v2(-1, 0);
      case BOT_LEFT:
        return v2(0, -1);
      case BOT_RIGHT:
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

  class LayerRenderer:public EngineObject{
  public:
    LayerRenderer();
    LayerRenderer(const LayerRenderer&base):EngineObject(base){}
    virtual void render();
  };
  // do not implement, is automatically generated when starting engine for the
  // parent object
  class ChildrenObject final : public GameObject
  {
  public:
    GameObject *object;
    v3 &parentPosition;
    v3 previousParentPosition;
    ChildrenObject(GameObject *object, v3 &parentPosition);
    void update(std::vector<msg> msgs);
  };
  class ParentObject : public ComponentGameObject
  {
  public:
    std::vector<GameObject *> children;
    ParentObject();
    ParentObject(const ParentObject&base);
  };

  struct EngineEvent
  {
    std::string name;
    std::vector<EngineObject *> *objects;
    std::function<void(std::vector<EngineObject *> objects)> event;
  };
  void addEvent(EngineEvent event);
  void callEvent(std::string name);

  template <class T = EngineObject>
  std::vector<T *> findObject(std::vector<EngineObject *> objects,
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
          if (s == "_CHILDREN_")
          {
            for (std::string schild : ((ChildrenObject *)obj)->object->labels)
            {
              if (schild == "ALL" || schild == label)
              {
                valid.push_back(((ChildrenObject *)obj)->object);
              }
            }
          }
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
  template <class T = EngineObject>
  std::vector<T *> findObject(std::vector<EngineObject *> objects,
                              objectFlags flag)
  {
    std::vector<EngineObject *> valid = std::vector<EngineObject *>();
    if (flag == objectFlags::ALL)
    {
      valid = objects;
    }
    else
    {
      for (EngineObject *obj : objects)
      {
        for (objectFlags f : obj->flags)
        {
          if (f == objectFlags::CHILD)
          {
            for (objectFlags fchild : ((ChildrenObject *)obj)->object->flags)
            {
              if (fchild == objectFlags::ALL || fchild == flag)
              {
                valid.push_back(((ChildrenObject *)obj)->object);
              }
            }
          }
          if (f == objectFlags::ALL || f == flag)
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
  template <class T = EngineObject>
  std::vector<T *> findObjectBlacklist(std::vector<EngineObject *> objects,
                                       objectFlags flag)
  {
    std::vector<EngineObject *> valid = std::vector<EngineObject *>();
    if (flag == objectFlags::ALL)
    {
      return std::vector<T *>();
    }
    else
    {
      for (EngineObject *obj : objects)
      {
        for (objectFlags f : obj->flags)
        {
          if (f == objectFlags::CHILD)
          {
            for (objectFlags fchild : ((ChildrenObject *)obj)->object->flags)
            {
              if (!(fchild == objectFlags::ALL || fchild == flag))
              {
                valid.push_back(((ChildrenObject *)obj)->object);
              }
            }
          }
          if (!(f == objectFlags::ALL || f == flag))
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
  template <class T = EngineObject>
  std::vector<T *> findObjectBlacklist(std::vector<EngineObject *> objects,
                                       std::string label)
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
          if (s == "_CHILDREN_")
          {
            for (std::string schild : ((ChildrenObject *)obj)->object->labels)
            {
              if (!(schild == "ALL" || schild == label))
              {
                valid.push_back(((ChildrenObject *)obj)->object);
              }
            }
          }
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

  Sprite importSprite(const char *filename);

  template <class T = GameObject>
  std::vector<T *> orderObjectsByZ(std::vector<GameObject *> objects)
  {
    if(objects.size()==0){
      return std::vector<T*>();
    }
    for (i32  i = 0; i < objects.size() - 1; i++)
    {
      for (i32  j = 0; j < objects.size() - 1 - i; j++)
      {
        if (objects[j]->position.z > objects[j + 1]->position.z)
        {
          GameObject *temp = objects[j];
          objects[j] = objects[j + 1];
          objects[j + 1] = temp;
        }
      }
    }
    std::vector<T *> res = std::vector<T *>(objects.size());
    for (i32  i = 0; i < objects.size(); i++)
    {
      res[i] = (T *)objects[i];
    }
    return res;
  };
} // namespace kNgine
