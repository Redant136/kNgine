#pragma once

#include <vector>
#include "kutils.h"
#include "EngineObjects.hpp"
#include "../../extern/box2d/include/box2d/box2d.h"

namespace kNgine{
  //this cannot be used with custom physics engine
  namespace physics
  {
    //[b2_physics_body]
    class b2PhysicsBodyComponent : public ObjectComponent
    {
    public:
      b2BodyDef bodyDef;
      b2Body *body;
      std::vector<b2FixtureDef> shape;
      b2PhysicsBodyComponent(ComponentGameObject *base, std::vector<b2FixtureDef> shape);
      b2PhysicsBodyComponent(ComponentGameObject *base, b2FixtureDef shape);
      b2PhysicsBodyComponent(const b2PhysicsBodyComponent& base);
      virtual ~b2PhysicsBodyComponent();
      void updatePos();
      void setStatic(bool isAffectedByGravity);
      v2 getVelocity();
      void setVelocity(v2 newVel);
      void addVelocity(v2 toAdd);
    };

    class b2Rect : public b2PhysicsBodyComponent{
    public:
      b2PolygonShape *shapePoly;
      b2Rect(ComponentGameObject *base, f32 width=1.0f, f32 height=1.0f, f32 density = 1.0f, f32 friction = 1.0f) : b2PhysicsBodyComponent(base, b2FixtureDef())
      {
        shapePoly = new b2PolygonShape();
        shapePoly->SetAsBox(width / 2, height / 2);
        this->shape[0].shape = shapePoly;
        this->shape[0].density = density;
        this->shape[0].friction = friction;
      }
      ~b2Rect(){
        delete shapePoly;
      }
    };

    class b2PhysicsEngine : public EngineObject
    {
    private:
      v2 gravity;
      b2World box2dWorld = b2World(b2Vec2(0, 0));
      std::vector<b2PhysicsBodyComponent *> bodies;
    public:
      b2PhysicsEngine();
      b2PhysicsEngine(v2 grav);
      b2PhysicsEngine(const b2PhysicsEngine& base);
      ~b2PhysicsEngine();
      void addBody(b2PhysicsBodyComponent *comp);
      void init(std::vector<EngineObject*>objects);
      void update(std::vector<msg> msgs);
      void end(std::vector<EngineObject*>objects);
    };

  } // namespace physics
}