#pragma once

#include <vector>
#include <string>
#include "EngineObjects.hpp"
#include "utils.h"

namespace kNgine
{
  class Camera : public GameObject
  {
  public:
    mapper posMapper;
    f32 fov;
    std::vector<std::string> blackList = std::vector<std::string>();
    enum
    {
      MAX_WH,
      MIN_WH,
      WIDTH,
      HEIGHT,
      DIAGONAL
    } fovType = MAX_WH;
    bool showDebugHitBox = false;
    Camera();
    Camera(f32 fov, i32  windowWidth, i32  windowHeight);
    ~Camera();

    void updateWindowSize(i32  windowWidth, i32  windowHeight);

    void render(std::vector<ComponentGameObject *> objects);
    void renderObject(ComponentGameObject *object);

    bool isBlacklist(std::string label)
    {
      for (std::string s : blackList)
      {
        if (s == "ALL" || s == label)
        {
          return true;
        }
      }
      return false;
    }
    bool isBlacklist(std::vector<std::string> objectLabels)
    {
      if (blackList.size() == 0)
      {
        return false;
      }
      for (std::string s : blackList)
      {
        for (std::string label : labels)
        {
          if (s == "ALL" || s == label)
          {
            return true;
          }
        }
      }
      return false;
    }
  };
} // namespace kNgine
