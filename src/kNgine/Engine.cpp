#include <vector>
#include <string>
#include <functional>
#include <random>
#include <ctime>
#include "../kFramework/utils.h"
#include "EngineObjects.hpp"
#include "../kFramework/kRenderer.h"
#include "Engine.hpp"

namespace kNgine
{
  std::vector<EngineObject *> objects = std::vector<EngineObject *>();
  size_t maxWorkingObjectsLength = 64;
  size_t workingObjectsLength = 0;
  EngineObject **workingObjects;
  std::string window_name = "Game";
  v2 window_size = {1920.0f, 1080.0f};
  LayerOrder renderingLayerOrder = LayerOrder();
  
  kRenderer_WindowContext context;
  bool DEBUG = false;
} // namespace kNgine