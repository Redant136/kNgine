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

namespace kNgine
{
#ifdef global_engine//deprecated
  std::vector<EngineObject *> objects = std::vector<EngineObject *>();
  std::string window_name = "Game";
  std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();

  inline void includeChildren()
  {
    bool addedParent = false;
    for (ParentObject *parent : findObject<ParentObject>(objects, objectFlags::PARENT))
    {
      for (GameObject *child : parent->children)
      {
        bool included = false;
        for (ChildrenObject *current :
             findObject<ChildrenObject>(objects, objectFlags::CHILD))
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

  inline void frameUpdate()
  {
    f32 time = std::chrono::duration<double>(
                     std::chrono::high_resolution_clock::now() - currentTime)
                     .count();
    currentTime = std::chrono::high_resolution_clock::now();
    std::vector<msg> msgs = std::vector<msg>();
    msgs.push_back({msg::TIME_ELAPSED, time});
    for (i32 i = 0; i < Key::KEY_LAST; i++)
    {
      if (renderer::keyStatusPressed((Key)i))
      {
        msg m = msg();
        m.msgType = msg::KEY;
        m.msgBody.key = ((Key)i);
        msgs.push_back(m);
      }
    }
    for (EngineObject *obj : objects)
    {
      obj->update(msgs);
    }
    renderer::clear(0, 0, 0, 0);
    if (objects.size() > 0)
    {
      std::vector<Camera *> cameras = findObject<Camera>(objects, objectFlags::CAMERA);
      std::vector<ComponentGameObject *> sprites =
          findObject<ComponentGameObject>(objects, objectFlags::SPRITE);
      cameras = orderObjectsByZ<Camera>(
          std::vector<GameObject *>(cameras.begin(), cameras.end()));
      sprites = orderObjectsByZ<ComponentGameObject>(
          std::vector<GameObject *>(sprites.begin(), sprites.end()));
      for (i32 i = 0; i < cameras.size(); i++)
      {
        cameras[i]->showDebugHitBox = true;
        v2 windowSize = renderer::getWindowSize();
        cameras[i]->updateWindowSize(windowSize.x,
                                     windowSize.y);
        for (i32 j = 0; j < sprites.size(); j++)
        {
          if (cameras[i]->position.z <= sprites[j]->position.z)
          {
            break;
          }
          cameras[i]->renderObject(sprites[j]);
        }
      }
      // 60fps ~= 0.016
      // 1e-05 =  0.00001
      // if (1/time<60) {
      //   std::cout << 1/time << std::endl;
      //   std::cout << "time1" << std::endl;
      // }
    }
  }

  inline void start(i32 argc, const char **argv)
  {
    includeChildren();
    renderer::init(argc, argv);
    renderer::createWindow(1920, 1080, window_name.c_str());
    sleepMillis(10);
    renderer::setupWindow(frameUpdate);
    renderer::setDrawFunction(frameUpdate);
    for (EngineObject *obj : objects)
    {
      obj->init(objects);
    }
    renderer::launch();
    for (EngineObject *obj : objects)
    {
      obj->end(objects);
    }
  }

  inline void cleanup()
  {
    for (EngineObject *obj : objects)
    {
      delete obj;
    }
  }
#endif
  class engine
  {
  private:
    std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
    std::vector<EngineObject *> objects = std::vector<EngineObject *>();
  public:
    u64 maxWorkingObjectsSize=64;
    u64 workingObjectsSize=0;
    EngineObject**workingObjects;
    // std::vector<EngineObject*>workingObjects=std::vector<EngineObject*>();
    std::string window_name="Game";
    v2 window_size={1920.0f,1080.0f};

    void includeChildren()
    {
      bool addedParent = false;
      for (ParentObject *parent : findObject<ParentObject>(objects, objectFlags::PARENT))
      {
        for (GameObject *child : parent->children)
        {
          bool included = false;
          for (ChildrenObject *current :
               findObject<ChildrenObject>(objects, objectFlags::CHILD))
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
      for (u32 i=0;i<workingObjectsSize;i++)
      {
        workingObjects[i]->update(msgs);
      }
      if (workingObjectsSize > 0)
      {
        std::vector<LayerRenderer *> background = findObject<LayerRenderer>(workingObjects,workingObjectsSize, objectFlags::BACKGROUND);
        for (u32 i = 0; i < background.size(); i++)
        {
          if (background[i]->isEnabled())
            background[i]->render();
        }
        std::vector<Camera *> cameras = findObject<Camera>(workingObjects,workingObjectsSize, objectFlags::CAMERA);
        std::vector<ComponentGameObject *> sprites =
            findObject<ComponentGameObject>(workingObjects, workingObjectsSize, objectFlags::SPRITE);
        cameras = orderObjectsByZ<Camera>(
            std::vector<GameObject *>(cameras.begin(), cameras.end()));
        sprites = orderObjectsByZ<ComponentGameObject>(
            std::vector<GameObject *>(sprites.begin(), sprites.end()));
        for (u32 i = 0; i < cameras.size(); i++)
        {
          v2 windowSize = renderer::getWindowSize();
          cameras[i]->updateWindowSize(windowSize.x,
                                       windowSize.y);
          for (u32 j = 0; j < sprites.size(); j++)
          {
            if (cameras[i]->position.z <= sprites[j]->position.z)
            {
              break;
            }
            cameras[i]->renderObject(sprites[j]);
          }
        }
        std::vector<LayerRenderer *> UI = findObject<LayerRenderer>(workingObjects, workingObjectsSize, objectFlags::UI);
        for(i32 i=0;i<UI.size();i++){
          if(UI[i]->isEnabled())UI[i]->render();
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

    void addObject(EngineObject* object){
      objects.push_back(object);
    }

    void reloadObjects() // slower, called during screen transition or stuff
    {
      for(u32 i=0;i<workingObjectsSize;i++){
        workingObjects[i]->unload(std::vector<EngineObject*>(workingObjects,workingObjects+workingObjectsSize));
      }
      workingObjectsSize=0;
      for (EngineObject *obj : objects)
      {
        if (obj->isEnabled())
        {
          assert(workingObjectsSize+1<maxWorkingObjectsSize);
          workingObjects[workingObjectsSize]=obj;
          workingObjectsSize++;
        }
      }
      for(u32 i=0;i<workingObjectsSize;i++){
        workingObjects[i]->load(std::vector<EngineObject *>(workingObjects, workingObjects + workingObjectsSize));
      }
    }

    void start(i32 argc, const char **argv)
    {
      seedRandomNumberGenerator();
      includeChildren();
      renderer::init(argc, argv);
      renderer::createWindow(window_size.x, window_size.y, window_name.c_str());
      sleepMillis(10);
      renderer::setupWindow(std::bind(&engine::frameUpdate,this));
      renderer::setDrawFunction(std::bind(&engine::frameUpdate, this));
      workingObjects = new EngineObject *[maxWorkingObjectsSize];
      for (EngineObject *obj : objects)
      {
        obj->init(objects);
      }
      reloadObjects();
      currentTime = std::chrono::high_resolution_clock::now();
      renderer::launch();
      for (EngineObject *obj : objects)
      {
        obj->end(objects);
      }
      delete[] workingObjects;
    }

    void enableObject(u32 index) // called during runtime of engine
    {
      enableObject(objects[index]);
    }

    void enableObject(EngineObject*object) // called during runtime of engine
    {
      if(!object->isEnabled()){
        object->enable();
        object->load(std::vector<EngineObject *>(workingObjects, workingObjects + workingObjectsSize));
        workingObjects[workingObjectsSize]=object;
        workingObjectsSize++;
      }
    }

    void disableObject(EngineObject* object)
    {
      if(object->isEnabled()){
        object->disable();
        object->unload(std::vector<EngineObject *>(workingObjects, workingObjects + workingObjectsSize));
        bool wasInList=false;
        for(u32 i=0;i<workingObjectsSize;i++){
          if(workingObjects[i]==object){
            workingObjects[i]=workingObjects[workingObjectsSize-1];
            wasInList=true;
          }
        }
        if(wasInList)workingObjectsSize--;
      }
    }

    void bindOnPress(std::function<void(void)>func,Key e){
      renderer::onKeyPress(func,e);
    }

    void cleanup()
    {
      for (EngineObject *obj : objects)
      {
        delete obj;
      }
    }
  };
} // namespace kNgine