#include "PhysicsChipmunkRenderer.hpp"
#include "kRenderer.h"

namespace kNgine
{
  namespace physics
  {
    void renderHitBox(kPhysicsBodyComponent *obj)
    {

    }
    void cpPhysicsRenderer::render()
    {
      // std::vector<kPhysicsBodyComponent *> objects = findObject <kPhysicsBodyComponent>(*cam->engineInfo, "[k_physics_body");
      // for(u32 i=0;i<objects.size();i++){
      //   renderHitBox(objects[i]);
      // }
      // kRenderer_drawRectV4(v4(-1,0,1,1));
    }
  }
}