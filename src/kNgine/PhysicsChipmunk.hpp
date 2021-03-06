#pragma once

#include "EngineObjects.hpp"
#include <functional>
#include <chipmunk/chipmunk.h>

namespace kNgine
{
  class kHitBox
  {
  public:
    std::vector<cpShape *> shapes;
    kHitBox() : kHitBox(std::vector<cpShape *>()) {}
    kHitBox(std::vector<cpShape *> shapes);
    kHitBox(cpShape *shape) : kHitBox(std::vector<cpShape *>(&shape, &shape + 1)) {}
    void setShapesBody(cpBody *body)
    {
      for (u32 i = 0; i < shapes.size(); i++)
      {
        cpShapeSetBody(shapes[i], body);
      }
    }
    void free()
    {
      for (u32 i = 0; i < shapes.size(); i++)
      {
        cpShapeFree(shapes[i]);
      }
    }
  };

  // [k_physics_body]
  class kPhysicsBodyComponent : public ObjectComponent
  {
  public:
    bool dependentPosition = true;
    kHitBox hitbox;
    cpBody *body;
    std::function<void(GameObject *)> preCollision = [](GameObject *obj) {};  // before physics calculation
    std::function<void(GameObject *)> postCollision = [](GameObject *obj) {}; // after physics calculation

    kPhysicsBodyComponent(ComponentGameObject *base, cpBody *body, kHitBox shape);
    kPhysicsBodyComponent(ComponentGameObject *base, cpBody *body, std::vector<cpShape *> shapes) : kPhysicsBodyComponent(base, body, kHitBox(shapes))
    {
    }
    kPhysicsBodyComponent(ComponentGameObject *base, cpBody *body, cpShape *shape) : kPhysicsBodyComponent(base, body, kHitBox(shape))
    {
    }
    virtual ~kPhysicsBodyComponent();
    // build funcs
    static kPhysicsBodyComponent *kPhysRect(ComponentGameObject *base, v2 size = v2(1, 1), f32 weight = 1.f, f32 friction = 0.f);
    static kPhysicsBodyComponent *staticBody(ComponentGameObject *base, kHitBox hitbot);
    kPhysicsBodyComponent *toIntangibleHitBox();

    // physics functions
    v2 getVelocity();         // get velocity of object
    void setVelocity(v2 vel); // set velocity
    void addVelocity(v2 vel); // add to the velocity
    f32 getElasticity();
    void setElasticity(f32 elasticity);
    v2 getForce();            // the force that will be applied at the time step
    void setForce(v2 f);      // set force
    void applyForce(v2 f);    // add to the force

    virtual void enable();
    virtual void disable();
    virtual void init(std::vector<EngineObject *> objects);
  };

  static inline kHitBox kHitBoxRect(v2 size = {1, 1}, f32 weight = 1.f, f32 friction = 0.f)
  {
    kHitBox h = kHitBox(cpBoxShapeNew(NULL, size.x, size.y, 0.001));
    cpShapeSetFriction(h.shapes[0], friction);
    return h;
  }

  cpBool findPreCollider(cpArbiter *arb, cpSpace *space, cpDataPointer data);
  void findPostCollider(cpArbiter *arb, cpSpace *space, cpDataPointer data);


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
