#include <iostream>
#define utils_print
#include "kutils.h"
#include "PhysicsChipmunk.hpp"
#include <chipmunk/chipmunk_structs.h>

#include <iostream>
#define toCPV(v) cpv((v).x, (v).y)

#define GlobalCollisionCategory 1 << 0
#define HitBoxCollisionCategory 1 << 1

namespace kNgine
{
  kHitBox::kHitBox(std::vector<cpShape *> shapes)
  {
    this->shapes = shapes;
    for (u32 i = 0; i < this->shapes.size(); i++)
    {
      cpShapeSetCollisionType(this->shapes[i], GlobalCollisionCategory);
      cpShapeSetElasticity(this->shapes[i],0);
    }
  }

  kPhysicsBodyComponent::kPhysicsBodyComponent(ComponentGameObject *base, cpBody *body, kHitBox shape) : ObjectComponent(base)
  {
    this->flags |= ObjectFlags::Physics;
    this->label = "[k_physics_body]";
    shape.setShapesBody(body);
    this->body = body;
    this->hitbox = shape;
    cpBodySetUserData(this->body, this);
    for (u32 i = 0; i < this->hitbox.shapes.size(); i++)
    {
      cpShapeFilter filter = cpShapeGetFilter(this->hitbox.shapes[i]);
      filter.group = (uintptr_t)base;
      filter.categories = GlobalCollisionCategory;
      filter.mask ^= HitBoxCollisionCategory;
      cpShapeSetFilter(this->hitbox.shapes[i], filter);
    }
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
    cpBodySetVelocity(body, toCPV(vel));
  }
  void kPhysicsBodyComponent::addVelocity(v2 vel)
  {
    cpBodySetVelocity(body, cpBodyGetVelocity(body) + toCPV(vel));
  }
  f32 kPhysicsBodyComponent::getElasticity()
  {
    return cpShapeGetElasticity(this->hitbox.shapes[0]);
  }
  void kPhysicsBodyComponent::setElasticity(f32 elasticity)
  {
    for (u32 i = 0; i < this->hitbox.shapes.size(); i++)
    {
      cpShapeSetElasticity(this->hitbox.shapes[i],elasticity);
    }

  }

  v2 kPhysicsBodyComponent::getForce()
  {
    return toV2(cpBodyGetForce(body));
  }
  void kPhysicsBodyComponent::setForce(v2 f)
  {
    cpBodySetForce(body, toCPV(f));
  }
  void kPhysicsBodyComponent::applyForce(v2 f)
  {
    cpBodySetForce(body, cpBodyGetForce(body) + toCPV(f));
  }
  kPhysicsBodyComponent *kPhysicsBodyComponent::toIntangibleHitBox()
  {
    cpBodySetType(this->body, cpBodyType::CP_BODY_TYPE_KINEMATIC);
    for (u32 i = 0; i < this->hitbox.shapes.size(); i++)
    {
      cpShapeFilter filter = cpShapeGetFilter(this->hitbox.shapes[i]);
      filter.categories |= HitBoxCollisionCategory;
      filter.mask ^= HitBoxCollisionCategory;
      filter.mask ^= GlobalCollisionCategory;
      cpShapeSetFilter(this->hitbox.shapes[i], filter);
    }

    
    return this;
  }

  void kPhysicsBodyComponent::enable()
  {
    if (!enabled)
    {
      ObjectComponent::enable();
      cpSpace *space = body->space;
      cpSpaceRemoveBody(space, body);
      body->space = space;
    }
  }
  void kPhysicsBodyComponent::disable()
  {
    if (enabled)
    {
      ObjectComponent::disable();
      cpSpace *space = body->space;
      body->space = NULL;
      cpSpaceAddBody(space, body);
    }
  }
  void kPhysicsBodyComponent::init(std::vector<EngineObject *> objects)
  {
    cpBodySetPosition(body, toCPV(object->position));
    cpBodySetAngle(body, object->rotation.z);
  }
  void kPhysicsBodyComponent::update(std::vector<msg> msgs)
  {
    if (dependentPosition)
    {
      object->position = toV2(body->p);
      object->rotation.z = body->a;
    }
    else
    {
      body->p = toCPV(object->position);
      body->a = object->rotation.z;
    }
  }
  void kPhysicsBodyComponent::end(std::vector<EngineObject *> objects)
  {
  }

  cpPhysicsEngine::cpPhysicsEngine() : cpPhysicsEngine(v2(0, 0))
  {
  }
  static cpBool findPreCollider(cpArbiter *arb, cpSpace *space, cpDataPointer data)
  {
    ((kPhysicsBodyComponent *)arb->body_a->userData)->preCollision(((kPhysicsBodyComponent *)arb->body_b->userData)->object);
    ((kPhysicsBodyComponent *)arb->body_b->userData)->preCollision(((kPhysicsBodyComponent *)arb->body_a->userData)->object);
    return cpTrue;
  }
  static void findPostCollider(cpArbiter *arb, cpSpace *space, cpDataPointer data)
  {
    ((kPhysicsBodyComponent *)arb->body_a->userData)->preCollision(((kPhysicsBodyComponent *)arb->body_b->userData)->object);
    ((kPhysicsBodyComponent *)arb->body_b->userData)->preCollision(((kPhysicsBodyComponent *)arb->body_a->userData)->object);
  }
  cpPhysicsEngine::cpPhysicsEngine(v2 gravity)
  {
    space = cpSpaceNew();
    this->gravity = gravity;
    cpSpaceSetGravity(space, cpv(gravity.x, gravity.y));
    space->defaultHandler.preSolveFunc = findPreCollider;
    space->defaultHandler.postSolveFunc = findPostCollider;
  }
  void cpPhysicsEngine::init(std::vector<EngineObject *> objects)
  {
    // engineData=(Array<EngineObject*>*)callEvent("getEngineObjects");
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
