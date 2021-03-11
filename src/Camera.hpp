#pragma once

#include <vector>
#include <string>
#include "EngineObjects.hpp"
#include "utils.h"

namespace kNgine
{
  class Camera : public LayerRenderer
  {
  private:
    void render(ComponentGameObject *object);
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
    struct{u64*engineObjectLength;EngineObject**engineObjects;}engineInfo;
    Camera();
    Camera(f32 fov, i32 windowWidth, i32 windowHeight);
    ~Camera();

    void init(std::vector<EngineObject *> objects);
    void updateWindowSize(i32 windowWidth, i32 windowHeight);

    void render();
  };
} // namespace kNgine
