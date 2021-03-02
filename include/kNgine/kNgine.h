#pragma once


#include "../../src/utils.hpp"
#include "../../src/Renderer.hpp"

#include "../../src/EngineObjects.hpp"
  
#ifdef custom_engine
#define engine_2D
#include "../../src/Physics.hpp"
#else
#include "../../src/PhysicsB2.hpp"
#endif
#include "../../src/Camera.hpp"
#include "../../src/SpriteUtils.hpp"
#include "../../src/AudioEngine.hpp"
#include "../../src/Tilemap.hpp"

#include "../../src/TextBox.hpp"

#include "../../src/Engine.hpp"