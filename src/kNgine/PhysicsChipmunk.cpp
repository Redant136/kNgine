#define utils_print
#include "kutils.h"
#include "PhysicsChipmunk.hpp"
#include <chipmunk/chipmunk_structs.h>

#include <iostream>
#define toCPV(v) cpv((v).x,(v).y)

namespace kNgine
{
  namespace physics
  {
    kPhysicsBodyComponent::kPhysicsBodyComponent(ComponentGameObject *base, cpBody *body, kHitBox shape) : ObjectComponent(base)
    {
      this->flags |= ObjectFlags::Physics;
      this->label = "[k_physics_body]";
      shape.setShapesBody(body);
      this->body = body;
      this->hitbox = shape;
    }
    kPhysicsBodyComponent::~kPhysicsBodyComponent()
    {
      hitbox.free();
      cpBodyFree(body);
    }
    kPhysicsBodyComponent *kPhysicsBodyComponent::kPhysRect(ComponentGameObject *base, v2 size, f32 weight, f32 friction)
    {
      return new kPhysicsBodyComponent(base, cpBodyNew(weight, cpMomentForBox(weight, size.x, size.y)), kHitBoxRect(size, weight, friction));
    }
    kPhysicsBodyComponent *kPhysicsBodyComponent::staticBody(ComponentGameObject *base, kHitBox hitbox)
    {
      return new kPhysicsBodyComponent(base, cpBodyNewStatic(), hitbox);
    }
    
    v2 kPhysicsBodyComponent::getVelocity()
    {
      return toV2(cpBodyGetVelocity(body));
    }
    void kPhysicsBodyComponent::setVelocity(v2 vel)
    {
      cpBodySetVelocity(body,toCPV(vel));
    }
    void kPhysicsBodyComponent::addVelocity(v2 vel)
    {
      cpBodySetVelocity(body, cpBodyGetVelocity(body) + toCPV(vel));
    }
    v2 kPhysicsBodyComponent::getForce()
    {
      return toV2(cpBodyGetForce(body));
    }
    void kPhysicsBodyComponent::setForce(v2 f)
    {
      cpBodySetForce(body,toCPV(f));
    }
    void kPhysicsBodyComponent::applyForce(v2 f)
    {
      cpBodySetForce(body,cpBodyGetForce(body)+toCPV(f));
    }

    void kPhysicsBodyComponent::init(std::vector<EngineObject *> objects)
    {
      cpBodySetPosition(body, toCPV(object->position));
      cpBodySetAngle(body, object->rotation.z);
    }
    void kPhysicsBodyComponent::update(std::vector<msg> msgs)
    {
      // std::cout<<cpShapeGetCollisionType(shapes[0])<<std::endl;
      object->position = v3(body->p.x, body->p.y, object->position.z);
      object->rotation.z = body->a;
    }
    void kPhysicsBodyComponent::end(std::vector<EngineObject *> objects)
    {

    }


    cpPhysicsEngine::cpPhysicsEngine() : cpPhysicsEngine(v2(0, 0))
    {
    }
    cpPhysicsEngine::cpPhysicsEngine(v2 gravity)
    {
      space = cpSpaceNew();
      this->gravity=gravity;
      cpSpaceSetGravity(space, cpv(gravity.x, gravity.y));
    }
    void cpPhysicsEngine::init(std::vector<EngineObject *> objects)
    {
      std::vector<ComponentGameObject *> list = findObject<ComponentGameObject>(objects, "[k_physics_body]");
      for (u32 i = 0; i < list.size(); i++)
      {
        kPhysicsBodyComponent *compn = list[i]->findComponent<kPhysicsBodyComponent>("[k_physics_body]");
        compn->body = cpSpaceAddBody(space, compn->body);
        for (u32 j = 0; j < compn->hitbox.shapes.size(); j++)
        {
          compn->hitbox.shapes[j] = cpSpaceAddShape(space, compn->hitbox.shapes[j]);
        }
      }
    }
    void cpPhysicsEngine::update(std::vector<msg> msgs)
    {
      f32 timeStep = 0;
      for (u32 i = 0; i < msgs.size(); i++)
      {
        if (msgs[i].msgType == msg::TIME_ELAPSED)
        {
          timeStep = msgs[i].time;
        }
      }
      cpSpaceStep(space, timeStep);
    }
    void cpPhysicsEngine::end(std::vector<EngineObject *> objects)
    {
      cpSpaceDestroy(space);
    }
  }
}
