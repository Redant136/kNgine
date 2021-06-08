#pragma once

#include "PhysicsChipmunk.hpp"
#include "Camera.hpp"

namespace kNgine
{
  class kPhysicsRenderer : public LayerRenderer
  {
  public:
    Camera *cam;
    kPhysicsRenderer(Camera *cam)
    {
      this->cam = cam;
      this->flags |= ObjectFlags::RENDERER_LAYER;
      this->layer = cam->layer;
      this->position = v3(0, 0, cam->position.z + 1);
    }
    void render();
    void update(std::vector<msg> msgs)
    {
      this->position.z = cam->position.z + 1;
    }
  };
}
