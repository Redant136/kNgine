#pragma once

#include <vector>
#include <string>
#include <chrono>
#include <functional>
#include "utils.hpp"
#include "EngineObjects.hpp"
#include "Renderer.hpp"
#include "Camera.hpp"
#include "SpriteUtils.hpp"

#include <iostream>

namespace kNgine
{
#ifdef global_engine
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
    float time = std::chrono::duration<double>(
                     std::chrono::high_resolution_clock::now() - currentTime)
                     .count();
    currentTime = std::chrono::high_resolution_clock::now();
    std::vector<msg> msgs = std::vector<msg>();
    msgs.push_back({msg::TIME_ELAPSED, time});
    for (int i = 0; i < Key::KEY_LAST; i++)
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
      for (int i = 0; i < cameras.size(); i++)
      {
        cameras[i]->showDebugHitBox = true;
        v2 windowSize = renderer::getWindowSize();
        cameras[i]->updateWindowSize(windowSize.x,
                                     windowSize.y);
        for (int j = 0; j < sprites.size(); j++)
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

  inline void start(int argc, const char **argv)
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
    std::vector<EngineObject*>workingObjects=std::vector<EngineObject*>();
    std::string window_name="Game";

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
      float time = std::chrono::duration<double>(
                       std::chrono::high_resolution_clock::now() - currentTime)
                       .count();
      currentTime = std::chrono::high_resolution_clock::now();
      std::vector<msg> msgs;
      msgs.push_back({msg::TIME_ELAPSED, time});
      { //mouse location
        v2 rendPos = renderer::cursorPosition();
        msg m = msg();
        m.msgType = msg::CURSOR;
        m.msgBody.cursorPos[0] = rendPos.x;
        m.msgBody.cursorPos[1] = rendPos.y;
        msgs.push_back(m);
      }
      for (int i = 0; i < Key::KEY_LAST; i++)
      {
        if (renderer::keyStatusPressed((Key)i))
        {
          msg m = msg();
          m.msgType = msg::KEY;
          m.msgBody.key = ((Key)i);
          msgs.push_back(m);
        }
      }
      for(int i=Key::KEY_LAST+1;i<Key::MOUSE_LAST;i++){
        if(renderer::mouseStatusPressed((Key)i)){
          msg m = msg();
          m.msgType = msg::KEY;
          m.msgBody.key = ((Key)i);
          msgs.push_back(m);
        }
      }
      renderer::clear(0, 0, 0, 0);
      workingObjects.clear();
      for (EngineObject *obj : objects)
      {
        if(obj->enabled){
          obj->update(msgs);
          workingObjects.push_back(obj);
        }
      }
      if (workingObjects.size() > 0)
      {
        std::vector<Camera *> cameras = findObject<Camera>(workingObjects, objectFlags::CAMERA);
        std::vector<ComponentGameObject *> sprites =
            findObject<ComponentGameObject>(workingObjects, objectFlags::SPRITE);
        cameras = orderObjectsByZ<Camera>(
            std::vector<GameObject *>(cameras.begin(), cameras.end()));
        sprites = orderObjectsByZ<ComponentGameObject>(
            std::vector<GameObject *>(sprites.begin(), sprites.end()));
        for (int i = 0; i < cameras.size(); i++)
        {
          v2 windowSize = renderer::getWindowSize();
          cameras[i]->updateWindowSize(windowSize.x,
                                       windowSize.y);
          for (int j = 0; j < sprites.size(); j++)
          {
            if (cameras[i]->position.z <= sprites[j]->position.z)
            {
              break;
            }
            cameras[i]->renderObject(sprites[j]);
          }
        }
        std::vector<UIRenderer*>UI=findObject<UIRenderer>(workingObjects,objectFlags::UI);
        for(int i=0;i<UI.size();i++){
          UI[i]->render();
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

    void start(int argc, const char **argv)
    {
      includeChildren();
      renderer::init(argc, argv);
      renderer::createWindow(1920, 1080, window_name.c_str());
      sleepMillis(10);
      renderer::setupWindow(std::bind(&engine::frameUpdate,this));
      renderer::setDrawFunction(std::bind(&engine::frameUpdate, this));
      this->workingObjects=std::vector<EngineObject*>();
      for (EngineObject *obj : objects)
      {
        obj->init(objects);
        this->workingObjects.push_back(obj);
      }
      currentTime = std::chrono::high_resolution_clock::now();
      renderer::launch();
      for (EngineObject *obj : objects)
      {
        obj->end(objects);
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