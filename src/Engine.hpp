#pragma once

#include <vector>
#include <string>
#include <chrono>
#include <functional>
#include <random>
#include <ctime>
#include "utils.h"
#include "EngineObjects.hpp"
#include "Renderer.hpp"
#include "Camera.hpp"
#include "SpriteUtils.hpp"

#include <iostream>
namespace kNgine
{
  class engine
  {
  private:
    std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
    std::vector<EngineObject *> objects = std::vector<EngineObject *>();
    void includeChildren()
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
  public:
    u64 maxWorkingObjectsLength=64;
    u64 workingObjectsLength=0;
    EngineObject**workingObjects;
    std::string window_name="Game";
    v2 window_size={1920.0f,1080.0f};
    LayerOrder renderingLayerOrder;// layer order must have camera layer

    void frameUpdate()
    {
      f32 time = std::chrono::duration<double>(
                       std::chrono::high_resolution_clock::now() - currentTime)
                       .count();
      currentTime = std::chrono::high_resolution_clock::now();
      std::vector<msg> msgs;
      msgs.push_back({msg::TIME_ELAPSED, time});
      { //mouse location
        v2 rendPos = renderer::cursorPosition();
        msg m = msg();
        m.msgType = msg::CURSOR;
        m.cursorPos = rendPos;
        msgs.push_back(m);
      }
      for (u32 i = 0; i < Key::KEY_LAST; i++)
      {
        if (renderer::keyStatusPressed((Key)i))
        {
          msg m = msg();
          m.msgType = msg::KEY;
          m.key = ((Key)i);
          msgs.push_back(m);
        }
      }
      for(u32 i=Key::KEY_LAST+1;i<Key::MOUSE_LAST;i++){
        if(renderer::mouseStatusPressed((Key)i)){
          msg m = msg();
          m.msgType = msg::KEY;
          m.key = ((Key)i);
          msgs.push_back(m);
        }
      }
      renderer::clear(0, 0, 0, 0);
      for (u32 i=0;i<workingObjectsLength;i++)
      {
        workingObjects[i]->update(msgs);
      }
      if (workingObjectsLength > 0)
      {
        iv2 windowSize=renderer::getWindowSize();
        std::vector<LayerRenderer*>renderers=findObject<LayerRenderer>(workingObjects,workingObjectsLength,ObjectFlags::RENDERER_LAYER);
        for(u32 i=0;i<renderingLayerOrder.orderLength;i++){
          std::vector<LayerRenderer *> layer = orderObjectsByZ<LayerRenderer>(getRenderersAtLayer(renderers, renderingLayerOrder.order[i]));
          for(LayerRenderer*r:layer){
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
    void start()
    {
      seedRandomNumberGenerator();
      includeChildren();
      renderer::init(0, NULL);
      renderer::createWindow(window_size.x, window_size.y, window_name.c_str());
      sleepMillis(10);
      renderer::setupWindow(std::bind(&engine::frameUpdate,this));
      renderer::setDrawFunction(std::bind(&engine::frameUpdate, this));
      // std::cout<<renderingLayerOrder.maxOrderLength<<std::endl;
      if(renderingLayerOrder.maxOrderLength<=0){
        renderingLayerOrder = LayerOrder(32);
        addLayerOrderDef(renderingLayerOrder, BACKGROUND);
        addLayerOrderDef(renderingLayerOrder, CAMERA);
        addLayerOrderDef(renderingLayerOrder, UI);
        renderingLayerOrder.order = (u32[]){layerO(renderingLayerOrder, BACKGROUND), layerO(renderingLayerOrder, CAMERA), layerO(renderingLayerOrder, CAMERA)};
      }
      workingObjects = new EngineObject *[maxWorkingObjectsLength];
      addEvent({"getEngineObjects",[this](void*arg)->void*{
        return workingObjects;
      }});
      addEvent({"getEngineObjectsSize", [this](void *arg) -> void * {
        return &workingObjectsLength;
      }});
      addEvent({"getEngineRendererLayers", [this](void *arg) -> void * {
        return &renderingLayerOrder;
      }});
      for (EngineObject *obj : objects)
      {
        obj->init(objects);
      }
      reloadObjects();
      currentTime = std::chrono::high_resolution_clock::now();
      renderer::launch();
      for(u32 i=0;i<workingObjectsLength;i++)
      {
        workingObjects[i]->unload(std::vector<EngineObject *>(workingObjects, workingObjects + workingObjectsLength));
      }
      for (EngineObject *obj : objects)
      {
        obj->end(objects);
      }
      delete[] workingObjects;
    }
    void cleanup()
    {
      for (EngineObject *obj : objects)
      {
        delete obj;
      }
    }

    // runtime execution functions
    void reloadObjects() // slower, called during screen transition or stuff
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
          assert(workingObjectsLength + 1 < maxWorkingObjectsLength);
          workingObjects[workingObjectsLength] = obj;
          workingObjectsLength++;
        }
      }
      for (u32 i = 0; i < workingObjectsLength; i++)
      {
        workingObjects[i]->load(std::vector<EngineObject *>(workingObjects, workingObjects + workingObjectsLength));
      }
    }
    void addObject(EngineObject *object)
    {
      objects.push_back(object);
    }
    void enableObject(u32 index) // called during runtime of engine
    {
      enableObject(objects[index]);
    }
    void enableObject(EngineObject*object) // called during runtime of engine
    {
      if(!object->isEnabled()){
        object->enable();
        object->load(std::vector<EngineObject *>(workingObjects, workingObjects + workingObjectsLength));
        workingObjects[workingObjectsLength]=object;
        workingObjectsLength++;
      }
    }
    void disableObject(EngineObject* object)
    {
      if(object->isEnabled()){
        object->disable();
        object->unload(std::vector<EngineObject *>(workingObjects, workingObjects + workingObjectsLength));
        bool wasInList=false;
        for(u32 i=0;i<workingObjectsLength;i++){
          if(workingObjects[i]==object){
            workingObjects[i]=workingObjects[workingObjectsLength-1];
            wasInList=true;
          }
        }
        if(wasInList)workingObjectsLength--;
      }
    }
    void bindOnPress(std::function<void(void)>func,Key e){
      renderer::onKeyPress(func,e);
    }

  };
} // namespace kNgine