#include <vector>
#include "../kFramework/utils.h"
#include "EngineObjects.hpp"
#include "PhysicsChipmunk.hpp"
#include <chipmunk/chipmunk.h>
#include <chipmunk/chipmunk_structs.h>

#define toCPV(v) cpv((v).x,(v).y)

namespace kNgine
{
  namespace physics
  {
    cpPhysicsBodyComponent::cpPhysicsBodyComponent(ComponentGameObject *base, cpBody *body, std::vector<cpShape *> shapes) : ObjectComponent(base)
    {
      this->flags |= ObjectFlags::Physics;
      this->label = "[cp_physics_body]";
      for (u32 i = 0; i < shapes.size(); i++)
      {
        cpShapeSetBody(shapes[i],body);
      }
      this->body = body;
      this->shapes = shapes;
    }
    cpPhysicsBodyComponent::cpPhysicsBodyComponent(ComponentGameObject *base, cpBody *body, cpShape *shape) : cpPhysicsBodyComponent(base, body, std::vector<cpShape *>(&shape, &shape+1))
    {
    }
    cpPhysicsBodyComponent::~cpPhysicsBodyComponent()
    {
      for (u32 i = 0; i < shapes.size(); i++)
      {
        cpShapeFree(shapes[i]);
      }
      cpBodyFree(body);
    }
    void cpPhysicsBodyComponent::addShape(cpShape *shape)
    {
      shapes.push_back(shape);
      shape->body = this->body;
    }
    cpPhysicsBodyComponent *cpPhysicsBodyComponent::staticBody(ComponentGameObject *base, std::vector<cpShape *> shapes)
    {
      return new cpPhysicsBodyComponent(base, cpBodyNewStatic(), shapes);
    }
    v2 cpPhysicsBodyComponent::getVelocity()
    {
      return toV2(cpBodyGetVelocity(body));
    }
    void cpPhysicsBodyComponent::setVelocity(v2 vel)
    {
      cpBodySetVelocity(body,toCPV(vel));
    }
    void cpPhysicsBodyComponent::addVelocity(v2 vel)
    {
      cpBodySetVelocity(body, cpBodyGetVelocity(body) + toCPV(vel));
    }
    v2 cpPhysicsBodyComponent::getForce()
    {
      return toV2(cpBodyGetForce(body));
    }
    void cpPhysicsBodyComponent::setForce(v2 f)
    {
      cpBodySetForce(body,toCPV(f));
    }
    void cpPhysicsBodyComponent::applyForce(v2 f)
    {
      cpBodySetForce(body,cpBodyGetForce(body)+toCPV(f));
    }

    void cpPhysicsBodyComponent::init(std::vector<EngineObject *> objects)
    {
      cpBodySetPosition(body, toCPV(object->position));
      cpBodySetAngle(body, object->rotation.z);
    }
    void cpPhysicsBodyComponent::update(std::vector<msg> msgs)
    {
      object->position = v3(body->p.x, body->p.y, object->position.z);
      object->rotation.z = body->a;
    }
    void cpPhysicsBodyComponent::end(std::vector<EngineObject *> objects)
    {

    }

    cpPhysicsBodyComponent *cpRect::staticBody(ComponentGameObject *base, v2 size, f32 friction)
    {
      cpBody*bod=cpBodyNewStatic();
      cpPhysicsBodyComponent *r = new cpPhysicsBodyComponent(base, bod, cpBoxShapeNew(bod, size.x, size.y, 0.001));
      cpShapeSetFriction(r->shapes[0], friction);
      return r;
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
      std::vector<ComponentGameObject *> list = findObject<ComponentGameObject>(objects, "[cp_physics_body]");
      for (u32 i = 0; i < list.size(); i++)
      {
        cpPhysicsBodyComponent *compn = list[i]->findComponent<cpPhysicsBodyComponent>("[cp_physics_body]");
        compn->body = cpSpaceAddBody(space, compn->body);
        for (u32 j = 0; j < compn->shapes.size(); j++)
        {
          compn->shapes[j] = cpSpaceAddShape(space, compn->shapes[j]);
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
      // timeStep=1/60.0;
      cpSpaceStep(space, timeStep);
    }
    void cpPhysicsEngine::end(std::vector<EngineObject *> objects)
    {
      cpSpaceDestroy(space);
    }
  }
}
