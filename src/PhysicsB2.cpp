#include <vector>
#include "utils.h"
#include "EngineObjects.hpp"
#include "PhysicsB2.hpp"
#include <box2d/box2d.h>

namespace kNgine
{
  namespace physics
  {
    b2PhysicsBodyComponent::b2PhysicsBodyComponent(ComponentGameObject *base, std::vector<b2FixtureDef> shape) : ObjectComponent(base)
    {
      this->label = "[b2_physics_body]";
      this->flags|=ObjectFlags::Physics;
      this->bodyDef.type = b2_dynamicBody;
      this->bodyDef.position.Set(object->position.x, object->position.y);
      this->shape = shape;
    }
    b2PhysicsBodyComponent::b2PhysicsBodyComponent(ComponentGameObject *base, b2FixtureDef shape) : ObjectComponent(base)
    {
      this->label = "[b2_physics_body]";
      this->flags|=ObjectFlags::Physics;
      this->bodyDef.type = b2_dynamicBody;
      this->bodyDef.position.Set(object->position.x, object->position.y);
      this->bodyDef.angle=object->rotation.z;
      this->shape = std::vector<b2FixtureDef>();
      this->shape.push_back(shape);
    }
    b2PhysicsBodyComponent::b2PhysicsBodyComponent(const b2PhysicsBodyComponent &base) : ObjectComponent(base)
    {
      this->label=base.label;
      this->object=base.object;
      this->bodyDef=base.bodyDef;
      this->shape=base.shape;
      this->body=base.body;
    }
    b2PhysicsBodyComponent::~b2PhysicsBodyComponent()
    {
    }
    void b2PhysicsBodyComponent::updatePos()
    {
      object->position = v3(body->GetPosition().x, body->GetPosition().y,0);
      object->rotation.z=body->GetAngle();
    }
    void b2PhysicsBodyComponent::setStatic(bool isAffectedByGravity)
    {
      if(isAffectedByGravity){
        this->bodyDef.type = b2_dynamicBody;
      }
      else
      {
        this->bodyDef.type = b2_staticBody;
      }
    }
    v2 b2PhysicsBodyComponent::getVelocity()
    {
      return v2(body->GetLinearVelocity().x, body->GetLinearVelocity().y);
    }
    void b2PhysicsBodyComponent::setVelocity(v2 newVel)
    {
      body->SetLinearVelocity(b2Vec2(newVel.x,newVel.y));
    }
    void b2PhysicsBodyComponent::addVelocity(v2 toAdd)
    {
      body->SetLinearVelocity(b2Vec2(toAdd.x + body->GetLinearVelocity().x, toAdd.y + body->GetLinearVelocity().y));
    }

    b2PhysicsEngine::b2PhysicsEngine() : box2dWorld(b2Vec2(gravity.x, gravity.y))
    {
      this->gravity = v2(0,0);
      this->flags|=ObjectFlags::Physics;
      this->labels.push_back("b2Engine");
    }
    b2PhysicsEngine::b2PhysicsEngine(v2 grav) : box2dWorld(b2Vec2(grav.x, grav.y))
    {
      this->gravity = v2(0,0);
      this->flags |= ObjectFlags::Physics;
      this->labels.push_back("b2Engine");
    }
    b2PhysicsEngine::b2PhysicsEngine(const b2PhysicsEngine &base):box2dWorld(base.box2dWorld){
      gravity=base.gravity;
      bodies=base.bodies;
    }
    b2PhysicsEngine::~b2PhysicsEngine()
    {
    }

    void b2PhysicsEngine::addBody(b2PhysicsBodyComponent *comp)
    {
      bodies.push_back(comp);
      comp->body = box2dWorld.CreateBody(&comp->bodyDef);
      for (i32 i = 0; i < comp->shape.size(); i++)
      {
        comp->body->CreateFixture(&comp->shape[i]);
      }
    }
    void b2PhysicsEngine::init(std::vector<EngineObject *> objects)
    {
      std::vector<ComponentGameObject *> bods = findObject<ComponentGameObject>(objects, "[b2_physics_body]");
      for (i32 i = 0; i < bods.size(); i++)
      {
        addBody(bods[i]->findComponent<b2PhysicsBodyComponent>("[b2_physics_body]"));
      }
    }
    void b2PhysicsEngine::update(std::vector<msg> msgs)
    {
      f32 timeElapsed;
      for (msg mess : msgs)
      {
        if (mess.msgType == msg::TIME_ELAPSED)
        {
          timeElapsed = mess.time;
          break;
        }
      }

      for(i32 i=0;i<bodies.size();i++){
        bodies[i]->body->SetEnabled(bodies[i]->object->isEnabled());
      }
      box2dWorld.Step(timeElapsed, 6, 2);
      for (b2PhysicsBodyComponent *bod : this->bodies)
      {
        bod->updatePos();
      }
    }
    void b2PhysicsEngine::end(std::vector<EngineObject *> objects)
    {
      for (i32 i = 0; i < bodies.size(); i++)
      {
        bodies[i]->body = NULL;
      }
    }

  } // namespace physics
} // namespace kNgine