#pragma once

#include <vector>
#include "../kFramework/utils.h"
#include "EngineObjects.hpp"
#include <chipmunk/chipmunk.h>

namespace kNgine
{
  namespace physics
  {
    // [cp_physics_body]
    class cpPhysicsBodyComponent : public ObjectComponent
    {
    protected:
    public:
      std::vector<cpShape *> shapes;
      cpBody *body;
      cpPhysicsBodyComponent(ComponentGameObject *base, cpBody *body, std::vector<cpShape *> shapes);
      cpPhysicsBodyComponent(ComponentGameObject *base, cpBody *body, cpShape *shape);
      virtual ~cpPhysicsBodyComponent();
      void addShape(cpShape *shape);
      static cpPhysicsBodyComponent *staticBody(ComponentGameObject *base, std::vector<cpShape *> shapes);
      static cpPhysicsBodyComponent *staticBody(ComponentGameObject *base, cpShape *shape) { return staticBody(base, std::vector<cpShape *>(&shape, &shape+1)); }
      v2 getVelocity();// get velocity of object
      void setVelocity(v2 vel);// set velocity
      void addVelocity(v2 vel);// add to the velocity
      v2 getForce();// the force that will be applied at the time step
      void setForce(v2 f);   // set force
      void applyForce(v2 f); // add to the force

      virtual void init(std::vector<EngineObject *> objects);
      virtual void update(std::vector<msg>msgs);
      virtual void end(std::vector<EngineObject *> objects);
    };

    class cpRect : public cpPhysicsBodyComponent
    {
    public:
      cpRect(ComponentGameObject *base, v2 size, f32 weight = 1.f, f32 friction = 0.f) : cpPhysicsBodyComponent(base, cpBodyNew(weight, cpMomentForBox(weight, size.x, size.y)), cpBoxShapeNew(NULL, size.x, size.y, 0.001))
      {
        cpShapeSetFriction(shapes[0],friction);
      }
      static cpPhysicsBodyComponent *staticBody(ComponentGameObject *base, v2 size, f32 friction = 1.f);
    };

    class cpPhysicsEngine : public EngineObject
    {
    private:
      v2 gravity;
      cpSpace *space;

    public:
      cpPhysicsEngine();
      cpPhysicsEngine(v2 gravity);
      virtual ~cpPhysicsEngine()
      {
        free(space);
      }
      void init(std::vector<EngineObject *> objects);
      void update(std::vector<msg> msgs);
      void end(std::vector<EngineObject *> objects);
    };
  }
}
