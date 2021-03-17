#pragma once

#include <vector>
#include <string>
#include <functional>
#include <random>
#include <ctime>
#include "utils.h"
#include "EngineObjects.hpp"
#include "kRenderer.h"
#include "Camera.hpp"
#include "SpriteUtils.hpp"

#include <iostream>
namespace kNgine
{
  extern std::vector<EngineObject *> objects;
  extern u64 maxWorkingObjectsLength;
  extern u64 workingObjectsLength;
  extern EngineObject **workingObjects;
  extern std::string window_name;
  extern v2 window_size;
  extern LayerOrder renderingLayerOrder; // layer order must have a DEFAULT_LAYER layer at index 0

  namespace{
    static void includeChildren()
    {
      bool addedParent = false;
      for (ParentObject *parent : findObject<ParentObject>(objects, ObjectFlags::PARENT))
      {
        for (GameObject *child : parent->children)
        {
          bool included = false;
          for (ChildrenObject *current :
               findObject<ChildrenObject>(objects, ObjectFlags::CHILD))
          {
            included = included || child == current->object;
          }
          if (!included)
          {
            objects.insert(objects.begin(),
                           new ChildrenObject(child, parent->position));
            for (std::string label : child->labels)
            {
              if (label == "_PARENT_")
              {
                addedParent = true;
              }
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
    for (u32 i = 0; i < workingObjectsLength; i++)
    {
      workingObjects[i]->unload(std::vector<EngineObject *>(workingObjects, workingObjects + workingObjectsLength));
    }
    workingObjectsLength = 0;
    for (EngineObject *obj : objects)
    {
      if (obj->isEnabled())
      {
        assert(workingObjectsLength < maxWorkingObjectsLength);
        workingObjects[workingObjectsLength] = obj;
        workingObjectsLength++;
      }
    }
    for (u32 i = 0; i < workingObjectsLength; i++)
    {
      workingObjects[i]->load(std::vector<EngineObject *>(workingObjects, workingObjects + workingObjectsLength));
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
      object->load(std::vector<EngineObject *>(workingObjects, workingObjects + workingObjectsLength));
      workingObjects[workingObjectsLength] = object;
      workingObjectsLength++;
    }
  }
  static void disableObject(EngineObject *object)
  {
    if (object->isEnabled())
    {
      object->disable();
      object->unload(std::vector<EngineObject *>(workingObjects, workingObjects + workingObjectsLength));
      bool wasInList = false;
      for (u32 i = 0; i < workingObjectsLength; i++)
      {
        if (workingObjects[i] == object)
        {
          workingObjects[i] = workingObjects[workingObjectsLength - 1];
          wasInList = true;
        }
      }
      if (wasInList)
        workingObjectsLength--;
    }
  }
  static void enableObject(u32 index)
  {
    enableObject(objects[index]);
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
    for (u32 i = 0; i < Key::KEY_LAST; i++)
    {
      if (kRenderer_keyStatusPressed((Key)i))
      {
        msg m = msg();
        m.msgType = msg::KEY;
        m.key = ((Key)i);
        msgs.push_back(m);
      }
    }
    for (u32 i = Key::KEY_LAST + 1; i < Key::MOUSE_LAST; i++)
    {
      if (kRenderer_mouseStatusPressed((Key)i))
      {
        msg m = msg();
        m.msgType = msg::KEY;
        m.key = ((Key)i);
        msgs.push_back(m);
      }
    }
    kRenderer_clear(v4(0, 0, 0, 0));
    for (u32 i = 0; i < workingObjectsLength; i++)
    {
      workingObjects[i]->update(msgs);
    }
    if (workingObjectsLength > 0)
    {
      iv2 windowSize = kRenderer_getWindowSize();
      std::vector<LayerRenderer *> renderers = findObject<LayerRenderer>(workingObjects, workingObjectsLength, ObjectFlags::RENDERER_LAYER);
      for (u32 i = 0; i < renderingLayerOrder.orderLength; i++)
      {
        std::vector<LayerRenderer *> layer = orderObjectsByZ<LayerRenderer>(getRenderersAtLayer(renderers, renderingLayerOrder.order[i]));
        for (LayerRenderer *r : layer)
        {
          r->updateWindowSize(windowSize.x, windowSize.y);
          r->render();
        }
      }
      // 60fps ~= 0.016
      // 1e-05 =  0.00001
      // if (1/time<60) {
      //   std::cout << 1/time << std::endl;
      //   std::cout << "time1" << std::endl;
      // }
    }
    msgs = std::vector<msg>();
  }
  static void start()
  {
    seedRandomNumberGenerator();
    includeChildren();
    kRenderer_WindowContext context;
    kRenderer_init(0, NULL, &context);
    kRenderer_setWindowName(window_name.c_str());
    kRenderer_setWindowSize(window_size.x, window_size.y);
    kRenderer_createWindow(&context);
    sleepMillis(10);
    kRenderer_setStartFunction(frameUpdate);
    kRenderer_setDrawFunction(frameUpdate);
    if (renderingLayerOrder.maxOrderLength <= 0)
    {
      renderingLayerOrder = LayerOrder(3);
      addLayerOrderDef(renderingLayerOrder, DEFAULT_LAYER);
      addLayerOrderDef(renderingLayerOrder, BACKGROUND);
      addLayerOrderDef(renderingLayerOrder, UI);
      u32 order[3] = {layerO(renderingLayerOrder, BACKGROUND), layerO(renderingLayerOrder, DEFAULT_LAYER), layerO(renderingLayerOrder, UI)};
      setLayerOrder(renderingLayerOrder,order);
    }
    workingObjects = new EngineObject *[maxWorkingObjectsLength];
    addEvent({"getEngineObjects", [](void *arg) -> void * {
                return workingObjects;
              }});
    addEvent({"getEngineObjectsSize", [](void *arg) -> void * {
                return &workingObjectsLength;
              }});
    addEvent({"getEngineRendererLayers", [](void *arg) -> void * {
                return &renderingLayerOrder;
              }});
    for (EngineObject *obj : objects)
    {
      obj->init(objects);
    }
    reloadObjects();
    kRenderer_launch();
    for (u32 i = 0; i < workingObjectsLength; i++)
    {
      workingObjects[i]->unload(std::vector<EngineObject *>(workingObjects, workingObjects + workingObjectsLength));
    }
    for (EngineObject *obj : objects)
    {
      obj->end(objects);
    }
    delete[] workingObjects;
  }
  static void cleanup()
  {
    for (EngineObject *obj : objects)
    {
      delete obj;
    }
  }
} // namespace kNgine