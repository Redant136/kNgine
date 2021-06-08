#define utils_print
#include "PhysicsChipmunkRenderer.hpp"
#include "kutils.h"
#include "kRenderer.h"
#include <chipmunk/chipmunk_structs.h>

namespace kNgine
{
  void renderHitBox(kPhysicsBodyComponent *obj,Camera*cam)
  {
    for(u32 i=0;i<obj->hitbox.shapes.size();i++)
    {
      if (obj->hitbox.shapes[i]->klass->type == cpShapeType::CP_POLY_SHAPE)
      {
        cpPolyShape*poly=(cpPolyShape*)obj->hitbox.shapes[i];
        u32 numPoints=poly->count;
        v2*lineLoop=new v2[numPoints];
        for(u32 i=0;i<numPoints;i++){
          lineLoop[i] = cam->posMapper.map(toV2(poly->planes[i].v0) - toV2(cam->position));
        }
        kRenderer_drawLineLoop(lineLoop,numPoints);
        delete[]lineLoop;
      }
      else if (obj->hitbox.shapes[i]->klass->type == cpShapeType::CP_POLY_SHAPE)
      {
      }
    }
  }
  void kPhysicsRenderer::render()
  {
    kRenderer_setDrawColor(v4(1,0,0,1));
    std::vector<ComponentGameObject *> objects = findObject <ComponentGameObject>(*cam->engineInfo, "[k_physics_body]");
    for(u32 i=0;i<objects.size();i++)
    {
      kPhysicsBodyComponent*compn=objects[i]->findComponent<kPhysicsBodyComponent>("[k_physics_body]");
      for(u32 j=0;compn;j++,compn=objects[i]->findComponent<kPhysicsBodyComponent>(std::string("[k_physics_body]")+std::to_string(j)))
      {
        renderHitBox(compn,this->cam);
      }
    }
  }
}