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
#include "Engine.hpp"

#include <iostream>
namespace kNgine
{
  std::vector<EngineObject *> objects = std::vector<EngineObject *>();
  u64 maxWorkingObjectsLength = 64;
  u64 workingObjectsLength = 0;
  EngineObject **workingObjects;
  std::string window_name = "Game";
  v2 window_size = {1920.0f, 1080.0f};
  LayerOrder renderingLayerOrder = LayerOrder(); // layer order must have a DEFAULT_LAYER layer at index 0
} // namespace kNgine