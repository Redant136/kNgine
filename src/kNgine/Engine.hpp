#pragma once

#include <vector>
#include <string>
#include <functional>
#include <random>
#include <ctime>
#include "../kFramework/utils.h"
#include "EngineObjects.hpp"
#include "../kFramework/kRenderer.h"
#include "Camera.hpp"
#include "SpriteUtils.hpp"

namespace kNgine
{
  extern std::vector<EngineObject *> objects;
  extern size_t maxWorkingObjectsLength;
  extern Array<EngineObject*>workingObjects;
  extern std::string window_name;
  extern v2 window_size;
  extern LayerOrder renderingLayerOrder; // layer order must have a DEFAULT_LAYER layer at index 0

  extern kRenderer_WindowContext context;

  extern bool DEBUG;

  static u32 object;
  static struct
  {
    v3 pos;
    f32 isTex;
    v4 color;
  } points[3];

  namespace {
    static void includeChildren()
    {
      bool addedParent = false;
      for (ComponentGameObject *parent : findObject<ComponentGameObject>(objects, ObjectFlags::PARENT))
      {
        for (ObjectComponent*compn:parent->components)
        {
          if(compn->flags&ObjectFlags::PARENT){
            GameObject *child = ((NodeObjectComponent*)compn)->child;
            bool included=false;
            for(EngineObject*obj:objects){
              included|=child==obj;
            }
            if(!included){
              objects.push_back(child);
              addedParent|=child->flags&ObjectFlags::PARENT;
            }
          }
        }
      }
      if (addedParent)
      {
        includeChildren();
      }
    }
  }

  // runtime execution functions
  static void reloadObjects() // slower, called during screen transition or stuff
  {
    for (u32 i = 0; i < workingObjects.length; i++)
    {
      workingObjects[i]->unload();
    }
    workingObjects.length = 0;
    for (EngineObject *obj : objects)
    {
      if (obj->isEnabled())
      {
        assert(workingObjects.length < maxWorkingObjectsLength);
        workingObjects[workingObjects.length] = obj;
        workingObjects.length++;
      }
    }
    for (u32 i = 0; i < workingObjects.length; i++)
    {
      workingObjects[i]->load();
    }
  }
  static void addObject(EngineObject *object)
  {
    objects.push_back(object);
  }
  static void enableObject(EngineObject *object)
  {
    if (!object->isEnabled())
    {
      object->enable();
      object->load();
      workingObjects[workingObjects.length] = object;
      workingObjects.length++;
    }
  }
  static void disableObject(EngineObject *object)
  {
    if (object->isEnabled())
    {
      object->disable();
      if(callEvent("isRunning")){
        object->unload();
      }
      bool wasInList = false;
      for (u32 i = 0; i < workingObjects.length; i++)
      {
        if (workingObjects[i] == object)
        {
          workingObjects[i] = workingObjects[workingObjects.length - 1];
          wasInList = true;
        }
      }
      if (wasInList)
        workingObjects.length--;
    }
  }
  static void enableObject(u32 index)
  {
    enableObject(objects[index]);
  }
  static void setupDefaultLayers()
  {
    renderingLayerOrder = LayerOrder();
    addLayerOrderDef(renderingLayerOrder, DEFAULT_LAYER);
    addLayerOrderDef(renderingLayerOrder, BACKGROUND);
    addLayerOrderDef(renderingLayerOrder, UI);
    u32 order[3] = {layerO(renderingLayerOrder, BACKGROUND), layerO(renderingLayerOrder, DEFAULT_LAYER), layerO(renderingLayerOrder, UI)};
    setLayerOrder(renderingLayerOrder, order);
  }

  static void frameUpdate()
  {
    f32 time=kRenderer_getTimeSinceLastFrame();
    std::vector<msg> msgs;
    msgs.push_back({msg::TIME_ELAPSED, time});
    { //mouse location
      v2 rendPos = kRenderer_cursorPosition();
      msg m = msg();
      m.msgType = msg::CURSOR;
      m.cursorPos = rendPos;
      msgs.push_back(m);
    }
    { //window size
      iv2 windowSize=kRenderer_getWindowSize();
      msg m=msg();
      m.msgType=msg::WINDOW_SIZE;
      m.window_size=windowSize;
      msgs.push_back(m);
    }
    { // key msgs
      u64 ascii_key=0;
      u64 nonAscii_key=0;
      for (u32 i = 0; i < Key::ASCII_KEY_LAST; i++)
      {
        if (kRenderer_keyStatusPressed((Key)i)){
          ascii_key |= KeyBitmap((Key)i);
        }
      }
      msg ascii_msg;
      ascii_msg.msgType = msg::ASCII_KEY;
      ascii_msg.key = ascii_key;
      msgs.push_back(ascii_msg);

      for(u32 i=Key::ASCII_KEY_LAST;i<Key::KEY_LAST;i++){
        if (kRenderer_keyStatusPressed((Key)i))
        {
          nonAscii_key |= KeyBitmap((Key)i);
        }
      }
      for (u32 i = Key::KEY_LAST; i < Key::MOUSE_LAST; i++)
      {
        if (kRenderer_mouseStatusPressed((Key)i)){
          nonAscii_key |= KeyBitmap((Key)i);
        }
      }
      msg non_ascii_msg;
      non_ascii_msg.msgType = msg::NONASCII_KEY;
      non_ascii_msg.key = nonAscii_key;
      msgs.push_back(non_ascii_msg);
    }
    kRenderer_clear(v4(0, 0, 0, 0));
    Array<EngineObject*>tempWorking=workingObjects.clone();
    for (u32 i = 0; i < tempWorking.length; i++)
    {
      tempWorking[i]->update(msgs);
    }
    if (tempWorking.length > 0)
    {
      std::vector<LayerRenderer *> renderers = findObject<LayerRenderer>(tempWorking, ObjectFlags::RENDERER_LAYER);
      for (u32 i = 0; i < renderingLayerOrder.length; i++)
      {
        std::vector<LayerRenderer *> layer = orderObjectsByZ<LayerRenderer>(getRenderersAtLayer(renderers, renderingLayerOrder.order[i]));
        for (LayerRenderer *r : layer)
        {
          kRenderer_setDrawColor(v4(1, 1, 1, 1));
          r->render();
        }
      }
    }
    tempWorking.free();

    if(DEBUG){
      std::string fps = std::to_string(1.0/time);
      kRenderer_displayText(v3(-1,1,1),v3(0,0,0),fps.c_str(),0.5);
      // kRenderer_drawObject(object);
    }
  }
  static void frameStart()
  {
    reloadObjects();
    frameUpdate();
  }
  static void start()
  {
#ifdef kNgine_DEBUG
    DEBUG=true;
#endif
    seedRandomNumberGenerator();
    kRenderer_init(0, NULL);
    kRenderer_createContext(&context);
    context.vSync = 1;
    kRenderer_setWindowName(window_name.c_str());
    kRenderer_setWindowSize(window_size.x, window_size.y);
    kRenderer_setWindowBoundsScale(v3(-1,-1,-1),v3(1,1,1));
    
    kRenderer_createWindow(&context);
    kRenderer_setStartFunction(frameStart);
    kRenderer_setDrawFunction(frameUpdate);
    if (renderingLayerOrder.length <= 0)
    {
      setupDefaultLayers();
    }
    workingObjects.arr = new EngineObject *[maxWorkingObjectsLength];
    addEvent({"getEngineObjects", [](void *arg) -> void * {
                return &workingObjects;
              }});
    addEvent({"getEngineRendererLayers", [](void *arg) -> void * {
                return &renderingLayerOrder;
              }});
    addEvent({"isRunning", [](void *arg) -> void * {
                return (void *)1;
              }});

    includeChildren();
    for (EngineObject *obj : objects)
    {
      obj->init(objects);
    }
    kRenderer_launch();
    for (u32 i = 0; i < workingObjects.length; i++)
    {
      workingObjects[i]->unload();
    }
    for (EngineObject *obj : objects)
    {
      obj->end(objects);
    }
    getEvent("isRunning").event = [](void *arg) -> void * {
      return NULL;
    };
    workingObjects.free();
  }
  static void cleanup()
  {
    for (EngineObject *obj : objects)
    {
      delete obj;
    }
  }
} // namespace kNgine